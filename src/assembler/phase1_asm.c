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
InstructionSentence* store_data(char* line){
    InstructionSentence *sen;
    char *label;
    size_t data_size;
    char* text;
    size_t pre_label_len;

    sen = (InstructionSentence*)malloc(sizeof(InstructionSentence));
    if(!sen){
        printf("[ERROR] Failed to allocate memory\n");
        return NULL;
    }

    /* Initialize sentence */
    declare_sentence(sen);

    /* Handle what's before the instruction, suspected as label */
    pre_label_len = strchr(line, '.') - line;

    if(pre_label_len > 0){
        label = malloc(pre_label_len + 1);
        if(!label){
            printf("[ERROR] Failed to allocate memory\n");
            return NULL;
        }
        strncpy(label, line, pre_label_len);
        if(is_valid_label(&label, 1)){
            /* Is label, save it */
            sen->label = label;
        } else {
            /* Invalid data given in line, quit */
            free(label);
            free(sen);
            return NULL;
        }
    }

    /* Store the data in the Instruction Sentence */
    if(strcmp(get_word(strchr(line, '.')), INSTRUCTIONS[0].name) == 0){
        text = strstr(line, INSTRUCTIONS[0].name) + strlen(INSTRUCTIONS[0].name);
        sen->data = pull_numbers(text, &data_size);
        sen->size = data_size;
    } else {
        text = strstr(line, INSTRUCTIONS[1].name) + strlen(INSTRUCTIONS[1].name);
        sen->data = get_ascii(text, &data_size);
        sen->size = data_size;
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
        printf("[ERROR] Failed to allocate memory\n");
        return NULL;
    }

    declare_sentence(i_s);

    /* Get the relevant source declaration and store the label */
    if(strcmp(get_word(strchr(line, '.')), INSTRUCTIONS[2].name) == 0){
        label = strstr(line, INSTRUCTIONS[2].name) + strlen(INSTRUCTIONS[2].name);
        if(is_valid_label(&label, 0)){
            i_s->label = label;
        } else {
            return NULL;
        }
    } else {
        label = strstr(line, INSTRUCTIONS[3].name) + strlen(INSTRUCTIONS[3].name);
        if(is_valid_label(&label, 0)){
            i_s->label = label;
            i_s->src = 1;
        } else {
            return NULL;
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
    clear_side_blanks_remove_newline(&command);
    for(i = 0; i < 16 ; i++){
        if(strcmp(get_word(command), OPERATIONS[i].name) == 0){
            c_s->operation = i;
            return;
        }
    }
}

/**
 * Saves the arguments of the operation in the command-sentence.
 * @param c_s command-sentence to store the arguments.
 * @param command line holding the arguments.
 */
void args(CommandSentence* c_s, char* command){
    char *sus_as_arg;
    /* Get the argument amount according to the operation code */
    int args_count = OPERATIONS[c_s->operation].arg_count;
    if(c_s->operation == -1){
        /* TODO: ILLEGAL COMMAND HANDLING */
        return;
    }
    clear_side_blanks(&command);

    if(!args_count){
        /* If no arguments, make sure there is no redundant characters */
        if(*command != '\0'){
            printf("YOU GAY\n");
        }
    } else if (args_count == 1) {
        /* If 1 argument, make sure there is no redundant characters */
        if(strpbrk(command, ",: \t\n\v\f\r") != NULL){
            printf("BAD SHET\n");
        }
        c_s->dest = command;
    } else {
        /* Pull 2 arguments */
        sus_as_arg = get_word(command);
        /* Get the first argument */
        if(sus_as_arg[strlen(sus_as_arg) - 1] == ','){
            sus_as_arg[strlen(sus_as_arg) - 1] = '\0';
        }
        c_s->src = sus_as_arg;
        command += strlen(sus_as_arg) + 1;
        clear_side_blanks_remove_newline(&command);
        /* Get the second argument */
        c_s->dest = get_word(command);
        command += strlen(c_s->dest);
        /* Check if there are redundant characters */
        if(*command != '\0'){
            printf("WELL THATS A PROBLEM NIGGA\n");
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
void analyze_command(CommandSentence* c_s, char* command){
    get_command(c_s, command);
    if(c_s->operation == -1){
        /* TODO: ILLEGAL COMMAND HANDLING */
        return;
    }
    args(c_s,  strstr(command, OPERATIONS[c_s->operation].name) + strlen(OPERATIONS[c_s->operation].name));
    word_count(c_s);
}

/**
 * When found a command, pull out all data inside the line, including the label if there is one.
 * @param command string represents a command
 * @param line number of line in the code.
 * @return command-sentence holding all data of the command.
 */
CommandSentence *pull_command(char *command, int line){
    CommandSentence *c_s = NULL;
    size_t pre_label_len;
    char* label;

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
            printf("[ERROR] Failed to allocate memory\n");
            return NULL;
        }
        strncpy(label, command, pre_label_len);
        label[pre_label_len] = '\0';
        /* TODO: SPACE AFTER COMMA */

        if(is_valid_label(&label, 0)){
            c_s->label = label;
        }

        command += pre_label_len + 1;
    }

    /* Store the rest of the data in the command-sentence */
    analyze_command(c_s, command);
    if(c_s->operation == -1){
        /* TODO: ILLEGAL COMMAND HANDLING */
        return NULL;
    }

    return c_s;
}

/**
 * Main function of the first pass
 * @param src_file file to open and read
 * @param labels database of labels
 * @param externals database holding extern labels.
 * @param entries database holding entry labels.
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
        SentenceList* code,
        SentenceList* data,
        HashTable* macros
        ){
    int IC = 0, DC = 0, L = 0, error_flag = 0;
    char* line = (char*)malloc(MAX_LINE_LEN);
    InstructionSentence *i_s;
    CommandSentence *c_s;
    char* copy;

    while (!feof(src_file) && DC + IC <= LAST_ADDRESS - FIRST_ADDRESS) {
        /* Read each line of the file */
        if (fgets(line, MAX_LINE_LEN, src_file) == NULL) {
            break;
        }

        /* Skip the line if no code info inside. */
        if(is_ignorable(line)){
            continue;
        }

        copy = get_line_copy(line);
        clear_side_blanks_remove_newline(&copy);
        if(strchr(line, '.')) {
            /* Get line suspected as an instruction, and run the relevant function */
            if (store_or_src(get_word(strchr(line, '.'))) > 0) {
                i_s = store_data(copy);
                if (i_s->label) {
                    insert_label_table(labels, i_s->label, IC + FIRST_ADDRESS + DC);
                }
                DC += i_s->size;
                /* save the data in the data storage */
                add_code(data, i_s, INSTRUCTION);
            } else if (store_or_src(get_word(strchr(line, '.'))) < 0) {
                i_s = src_handling(copy);
                if (i_s == NULL) {
                    printf("BAD NAME\n");
                    continue;
                }
                /* get the source of the label and store it in the relevant database */
                if (i_s->src == 0 && !exists(*externals, i_s->label, 1)) {
                    insert_source_label(entries, i_s->label, 0);
                } else if (i_s->src == 1 && !exists(*entries, i_s->label, 1)) {
                    insert_source_label(externals, i_s->label, 0);
                }
            }
        } else {
            /* Get the command, and store the label and the command in the database */
            c_s = pull_command(copy, 0);
            if(c_s != NULL){
                if(c_s->label != NULL){
                    insert_label_table(labels, c_s->label, IC + FIRST_ADDRESS);
                }
                IC += c_s->word_count;
                add_code(code, c_s, COMMAND);
            }
        }
    }
    return error_flag;
}
