#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../include/common/consts.h"
#include "../../include/assembler/phase1_asm.h"
#include "../../include/assembler/phase2_asm.h"
#include "../../include/common/utils.h"
#include "../../include/common/data_types.h"
#include "../../include/common/library.h"
#include "../../include/common/collections/linked_list.h"
#include "../../include/common/collections/sentence_list.h"
#include "../../include/common/operands/registers.h"
#include "../../include/common/operands/labels.h"
#include "../../include/common/operands/integers.h"
#include "../../include/common/operands/strings.h"
#include "../../include/common/collections/hash_table.h"

/**
 * Initializes lists
 * @param code list holds commands
 * @param data list holds instructions
 */
void declare_lists(SentenceList* code, SentenceList* data){
    code->head = NULL;
    code->size = 0;
    data->head = NULL;
    data->size = 0;
}

/**
 * Differentiate if the instruction declares a data to store or a source of a label.
 * @param instruction instruction as string
 * @return 1 if instruction of data, -1 if not, 0 if invalid.
 */
int store_or_src(char *instruction){
    int i;
    for(i = 0; i < 2; i++){
        if(strcmp(instruction, INSTRUCTIONS[i].name) == 0){
            return 1;
        }
    }
    for(i = 2; i < 4; i++){
        if(strcmp(instruction, INSTRUCTIONS[i].name) == 0){
            return -1;
        }
    }
    return 0;
}

/**
 * Initializes an instruction
 * @param sen instruction to initialize
 */
void declare_sentence(InstructionSentence *sen){
    sen->label = NULL;
    sen->data = NULL;
    sen->src = 0;
    sen->size = 0;
    sen->pos = 0;
}

/**
 * Receives an instruction and converts it into an InstructionSentence object of storing data.
 * @param line instruction as string
 * @return InstructionSentence instance holding data.
 */
InstructionSentence* store_data(char* line, int line_num, Node** errors){
    InstructionSentence *sen;
    char *label = NULL;
    size_t data_size;
    char* text;
    size_t pre_label_len;

    sen = (InstructionSentence*)malloc(sizeof(InstructionSentence));
    if(!sen){
        return NULL;
    }

    /* Initialize sentence */
    declare_sentence(sen);

    /* Handle what's before the instruction, suspected as label */
    pre_label_len = strchr(line, '.') - line;

    if(pre_label_len > 0){
        label = (char*)malloc(pre_label_len + 1);
        if(!label){
            free(sen);
            return NULL;
        }
        strncpy(label, line, pre_label_len);
        label[pre_label_len] = '\0';
        if(is_valid_label(&label, 1)){
            /* Is label, save it */
            sen->label = label;
        } else {
            append(errors, line_num, "Illegal declaration of label");
            return sen;
        }
    }

    /* Store the data in the Instruction Sentence */
    if(strcmp(get_word(strchr(line, '.')), INSTRUCTIONS[0].name) == 0){
        text = strstr(line, INSTRUCTIONS[0].name) + strlen(INSTRUCTIONS[0].name);
        sen->data = pull_numbers(text, &data_size, errors, line_num);
        sen->size = data_size;
    } else {
        text = strstr(line, INSTRUCTIONS[1].name) + strlen(INSTRUCTIONS[1].name);
        sen->data = get_ascii(text, &data_size, line_num, errors);
        sen->size = data_size;
    }

    if(sen->data == NULL){
        free_space(2, sen, label);
        /* Failed to allocate memory */
        return NULL;
    }

    return sen;
}

/**
 * Handle the instruction of declaring the source of the label
 * @param line instruction as string
 * @return
 */
InstructionSentence* src_handling(char* line){
    InstructionSentence* i_s;
    char* label;
    i_s = (InstructionSentence*) malloc(sizeof(InstructionSentence));
    if(!i_s){
        return NULL;
    }

    declare_sentence(i_s);

    /* Get the relevant source declaration and store the label */
    if(strcmp(get_word(strchr(line, '.')), INSTRUCTIONS[2].name) == 0){
        label = strstr(line, INSTRUCTIONS[2].name) + strlen(INSTRUCTIONS[2].name);
        if(is_valid_label(&label, 0)) {
            i_s->label = label;
        }
    } else {
        label = strstr(line, INSTRUCTIONS[3].name) + strlen(INSTRUCTIONS[3].name);
        if(is_valid_label(&label, 0)){
            i_s->label = label;
            i_s->src = 1;
        }
    }
    return i_s;
}

/**
 * Initializes a command object
 * @param c_s command to initialize.
 */
void generate_command(CommandSentence *c_s){
    c_s->label = NULL;
    c_s->pos = 0;
    c_s->src = NULL;
    c_s->dest = NULL;
    c_s->operation = -1;
    c_s->word_count = 0;
}

/**
 * Get the code of the operation given and store it.
 * @param c_s command-sentence to store the operation
 * @param command string suspected to represent an operation
 */
void get_command(CommandSentence* c_s, char* command){
    int i;
    clear_side_blanks(&command, 1);
    for(i = 0; i < 16 ; i++){
        if(strcmp(get_word(command), OPERATIONS[i].name) == 0){
            c_s->operation = i;
            return;
        }
    }
}

/**
 * Check whether argument received is a legal argument.
 * @param arg
 * @return 1 if true, 0 otherwise.
 */
int is_valid_arg(char* arg, Node** errors, int line){
    if(*arg == '#' && !is_num_legal(arg + 1, COM_BOUND, errors, line)){
        return 1;
    }
    else if(reg_arg(arg) ||  is_valid_label(&arg, 0)){
        return 1;
    }
    return 0;
}

/**
 * Saves the arguments of the operation in the command-sentence.
 * @param c_s command-sentence to store the arguments.
 * @param command line holding the arguments.
 */
void args(CommandSentence* c_s, char* command, int line, Node** errors){
    char *sus_as_arg;
    int comma = 0;
    char* txt = command;
    /* Get the argument amount according to the operation code */
    int args_count = OPERATIONS[c_s->operation].arg_count;
    clear_side_blanks(&txt, 0);

    if(!args_count){
        /* If no arguments, make sure there is no redundant characters */
        if(*txt != '\0'){
            append(errors, line, "Redundant string.");
        }
    } else if (args_count == 1) {
        txt = get_word(txt);
        /* If 1 argument, make sure there is no redundant characters */
        if(strpbrk(txt, ",: \t\n\v\f\r") != NULL){
            append(errors, line, "Redundant string.");
        }
        c_s->dest = txt;
    } else {
        /* Pull 2 arguments */
        sus_as_arg = get_word(txt);
        /* Get the first argument */
        c_s->src = sus_as_arg;

        txt += strlen(sus_as_arg);
        clear_side_blanks(&txt, 1);
        if(*txt == ','){
            comma = 1;
        }
        txt++;
        clear_side_blanks(&txt, 0);
        if(*txt == ',' && comma){
            append(errors, line, "Consecutive commas.");
        }

        if(!comma){
            append(errors, line, "No commas between arguments.");
        }
        /* Get the second argument */
        c_s->dest = get_word(txt);
        if(!is_valid_arg(c_s->dest, errors, line) || !is_valid_arg(c_s->src, errors, line)){
            append(errors, line, "Invalid argument.");
        }
        txt += strlen(c_s->dest);
        /* Check if there are redundant characters */
        if(*txt != '\0'){
            append(errors, line, "Redundant string.");
        }
    }
}

/**
 * Calculates the number of words in storage the current command will take.
 * @param c_s command checked how many words it takes.
 */
void word_count(CommandSentence* c_s){
    if(OPERATIONS[c_s->operation].arg_count == 0){
        c_s->word_count = 1;
    } else if(OPERATIONS[c_s->operation].arg_count == 1){
        c_s->word_count = 2;
    } else {
        if(reg_arg(c_s->src) && reg_arg(c_s->dest)){
            c_s->word_count = 2;
        } else {
            c_s->word_count = 3;
        }
    }
}

/**
 * When a command is received, store all data required
 * @param c_s object to store the data of the command
 * @param command string representing the command
 */
void analyze_command(CommandSentence* c_s, char* command, int line, Node** errors){
    get_command(c_s, command);
    if(c_s->operation == -1){
        return;
    }
    args(c_s,
         strstr(command, OPERATIONS[c_s->operation].name) + strlen(OPERATIONS[c_s->operation].name),
         line,
         errors);
    word_count(c_s);
}

/**
 * Checks if a macro has the label's name
 * @param macros database of macros
 * @param label to check
 * @param line of the code
 * @param errors database of errors
 * @return 1 if there's such a macro, 0 otherwise.
 */
int is_label_macro(HashTable* macros, char* label, int line, Node** errors){
    if(get(macros, label)){
        append(errors, line, "Label and macro cannot share names.");
        return 1;
    }
    return 0;
}

/**
 * When found a command, pull out all data inside the line, including the label if there is one.
 * @param command string represents a command
 * @param line number of line in the code.
 * @return command-sentence holding all data of the command.
 */
CommandSentence *pull_command(char *command, int line, Node** errors, HashTable* macros){
    CommandSentence *c_s = NULL;
    size_t pre_label_len;
    char* label;
    char* txt;
    txt = (char*)malloc(strlen(command) + 1);
    strcpy(txt, command);
    /* Initialize the command */
    c_s = (CommandSentence*)malloc(sizeof(CommandSentence));
    if(!c_s){
        return NULL;
    }
    generate_command(c_s);

    /* If label is found, validate and store it in the command-sentence */
    if(strchr(command, ':')){
        pre_label_len = strchr(command, ':') - command;
        label = (char*)malloc(pre_label_len + 1);
        if(!label){
            return NULL;
        }
        strncpy(label, command, pre_label_len);
        label[pre_label_len] = '\0';
        /* TODO: SPACE AFTER COMMA */

        if(!valid_label_decl(&label, macros) && !is_label_macro(macros, label, line, errors)){
            c_s->label = label;
        }
        command += pre_label_len + 1;
        if(!isspace(*command)){
            append(errors, line, "Missing space after label declaration.");
        }
    }

    /* Store the rest of the data in the command-sentence */
    analyze_command(c_s, command, line, errors);
    if(c_s->operation == -1){
        printf("[%s]\n",command);
        append(errors, line, "No such operation");
        return NULL;
    }

    return c_s;
}

/**
 * Frees generically two strings and an object
 * @param sentence object to free
 * @param str1 string to free
 * @param str2 string to free
 * @return 1 as a failure code.
 */
int free_at_error(void* sentence, char* str1, char* str2){
    free(sentence);
    free_space(2, str1, str2);
    return 1;
}

void update_line(char* label, int line, Node** externals, Node** entries){
    Node* test;
    if((test = get_node(*externals, label))){
        test->data->line = line;
    } else if((test = get_node(*entries, label))){
        test->data->line = line;
    }
}

/**
 * Main function of the first pass
 * @param src_file file to open and read
 * @param labels database of labels
 * @param externals database holding extern labels.
 * @param entries database holding entry labels.
 * @param errors database holding errors occurred.
 * @param code database holding the command lines
 * @param data database holding the data.
 * @param macros database holding the macros.
 * @return code of success or failure.
 */
int first_pass(
        FILE* src_file,
        Node** labels,
        Node** externals,
        Node** entries,
        Node** errors,
        SentenceList* code,
        SentenceList* data,
        HashTable* macros
        ){
    int IC = 0, DC = 0, L = 1, handled = 0;
    char* line = (char*)malloc(BUFFER);
    InstructionSentence *i_s;
    CommandSentence *c_s;
    char* copy;

    while (fgets(line, BUFFER, src_file) && IC + DC + FIRST_ADDRESS <= LAST_ADDRESS) {
        /* Read each line of the file */

        if(is_ignorable(line)){
            continue;
        }

        if (strlen(line) > MAX_LINE_LEN) {
            append(errors, L, "Line too long");
            continue;
        }
        line[strlen(line)] = '\0';
        /* Skip the line if no code info inside. */


        copy = get_line_copy(line);
        clear_side_blanks(&copy, 0);

        if(strchr(line, '.')) {
            /* Get line suspected as an instruction, and run the relevant function */
            if (store_or_src(get_word(strchr(line, '.'))) > 0) {
                i_s = store_data(copy, L, errors);
                if(i_s == NULL){
                    free_space(2, copy, line);
                    return 1;
                }
                if (i_s->label && !is_label_macro(macros, i_s->label, L, errors)) {
                    insert_label_table(labels, i_s->label, IC + FIRST_ADDRESS + DC);
                }
                DC += i_s->size;
                i_s->pos = L;
                /* save the data in the data storage */
                if(add_code(data, i_s, INSTRUCTION)){
                    return free_at_error((void*)i_s, copy, line);
                }
                handled = 1;
            } else if (store_or_src(get_word(strchr(line, '.'))) < 0) {
                i_s = src_handling(copy);
                if (i_s == NULL) {
                    free_space(2, copy, line);
                    return 1;
                } else if (i_s->label == NULL){
                    append(errors, L, "Bad label declaration.");
                    free(i_s);
                    continue;
                }
                /* get the source of the label and store it in the relevant database */
                if (i_s->src == 0 && !is_label_macro(macros, i_s->label, L, errors)) {
                    if(exists(*externals, i_s->label, 1)){
                        append(errors, L, "Label already declared as extern");
                    }
                    if(insert_source_label(entries, i_s->label, 0)){
                        return free_at_error((void*)i_s, copy, line);
                    }
                } else if (i_s->src == 1 && !is_label_macro(macros, i_s->label, L, errors)) {
                    if(exists(*externals, i_s->label, 1)){
                        append(errors, L, "Label already declared as entry");
                    }
                    if(insert_source_label(externals, i_s->label, 0)){
                        return free_at_error((void*)i_s, copy, line);
                    }
                }
                handled = 1;
            }
        }
        if (!handled){
            /* Get the command, and store the label and the command in the database */
            c_s = pull_command(copy, L, errors, macros);
            if(c_s != NULL){
                if(c_s->label != NULL && !is_label_macro(macros, c_s->label, L, errors)){
                    if(exists(*externals, c_s->label, 0)){
                        append(errors, L, "Cannot define extern label here.");
                    }
                    insert_label_table(labels, c_s->label, IC + FIRST_ADDRESS);
                }
                IC += c_s->word_count;
                c_s->pos = L;
                if(add_code(code, c_s, COMMAND)){
                    return free_at_error((void*)c_s, line, copy);
                }
            } else {
                free_space(2, line, copy);
                return 1;
            }
        }
        L++;
        handled = 0;
        print_list(*errors);
    }
    free_space(2, copy, line);
    if(!feof(src_file)){
        append(errors, 0, "Not enough space, file too long.");
    }

    if(*errors != NULL){
        return 1;
    } else {
        return 0;
    }
}
