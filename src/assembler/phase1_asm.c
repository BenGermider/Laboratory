#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../include/common/consts.h"
#include "../../include/assembler/phase1_asm.h"
#include "../../include/common/utils.h"
#include "../../include/common/data_types.h"
#include "../../include/common/library.h"
#include "../../include/common/collections/linked_list.h"
#include "../../include/common/collections/sentence_list.h"

int is_reg(char *str){
    int i;
    if(str == NULL){
        return 0;
    }
    for(i = 0; i < 8; i++){
        if(strcmp(str, REGISTERS[i].name) == 0){
            return 1;
        }
    }
    return 0;
}

int is_operation(char *str){
    int i;
    for(i = 0; i < 8; i++){
        if(strcmp(str, OPERATIONS[i].name) == 0){
            return 1;
        }
    }
    return 0;
}

int is_valid_instruction(char* str){
    int i;
    for(i = 0; i < 4; i++){
        if(strcmp(str, INSTRUCTIONS[i].name) == 0){
            return 1;
        }
    }
    return 0;
}

int is_valid_label(char **label, int is_decl) {
    clear_side_blanks_remove_newline(label);
    char *p = *label;

    if(!isalpha(*p)){
        return 0;
    }
    p++;
    while(*p != '\0' && isalnum(*p)) {
        p++;
    }
    if(is_decl && *p == ':' && *(p + 1) == '\0') {
        *p = '\0';
        return 1;
    } else if (!is_decl && (*p == '\0' || isspace(*p))) {
        return 1;
    }
    return 0;
}

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

void instruction(char* line, char* line_copy){
    char *inst_start, *instruction;
    line_copy = get_line_copy(line);
    if((inst_start = strchr(line_copy, '.'))){
        instruction = get_word(inst_start);
        if(!is_valid_instruction(instruction)){
            printf("[ERROR] Invalid instruction.\n");
            return;
        }
    }
}


void declare_sentence(InstructionSentence *sen){
    sen->label = NULL;
    sen->data = NULL;
    sen->src = 0;
    sen->size = 0;
    sen->pos = 0;
}

int* pull_numbers(char* line, size_t* size){
    char* token;
    char* token_copy;
    int i = 0;
    char* copy = (char*)malloc(sizeof(line));
    int* arr = (int*)malloc(sizeof(int));
    if(copy == NULL || arr == NULL){
        printf("[ERROR] Failed to allocate memory\n");
        return NULL;
    }
    strcpy(copy, line);
    copy[strlen(line)] = '\0';
    token = strtok(copy, ",");
    while(token != NULL){
        clear_side_blanks_remove_newline(&token);
        token_copy = token;
        while(*token_copy != '\0'){
            if(isspace(*token_copy)){
                free(copy);
                free(arr);
                printf("[ERROR] Numbers are not seperated properly\n");
                return NULL;
            }
            token_copy++;
        }
        if(is_num_legal(token)){
            arr[i] = atoi(token);
            arr = (int*)realloc(arr, (1 + i++)*sizeof(int));
            if(arr == NULL){
                printf("[ERROR] Failed to allocate memory\n");
                free(copy);
                return NULL;
            }
        }
        token = strtok(NULL, ",");
    }
    *size = i;
    free(copy);
    return arr;
}

int* get_ascii(char* line, size_t *size){
    int curr_size = 1, *arr, i, *temp;
    clear_side_blanks_remove_newline(&line);
    if(*line != '\"' || *(line + strlen(line) - 1) != '\"'){
        printf("BAD STRING TYPE");
        return NULL;
    }

    arr = (int*)malloc(curr_size * sizeof(int));
    if(arr == NULL){
        printf("FAILED TO ALLOCATE MEMORY\n");
        return NULL;
    }

    for (i = 1; i < strlen(line) - 1; i++) {
        if (line[i] == '\"') {
            printf("BAD STRING\n");
            free(arr);
            return NULL;
        }

        arr[curr_size - 1] = (int)line[i];
        curr_size++;

        temp = (int*)realloc(arr, curr_size * sizeof(int));
        if (temp == NULL) {
            printf("BAD\n");
            free(arr);
            return NULL;
        }
        arr = temp;
    }

    arr[curr_size - 1] = '\0';
    *size = curr_size;

    return arr;
}

int insert_label_table(Node **database, char *label, int lines){
    if(!exists(*database, label, 0)){
        append(database, lines, label);
    } else {
        printf("[ERROR] Label already exists.\n");
        return 1;
    }

    return 0;
}

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

    declare_sentence(sen);

    pre_label_len = strchr(line, '.') - line;

    if(pre_label_len > 0){
        label = malloc(pre_label_len + 1);
        if(!label){
            printf("[ERROR] Failed to allocate memory\n");
            return NULL;
        }
        strncpy(label, line, pre_label_len);
        if(is_valid_label(&label, 1)){
            sen->label = label;
        } else {
            free(label);
            free(sen);
            return NULL;
        }
    }

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

InstructionSentence* src_handling(char* line, int src_index){
    InstructionSentence* i_s;
    char* label;
    i_s = (InstructionSentence*) malloc(sizeof(InstructionSentence));
    if(!i_s){
        printf("[ERROR] Failed to allocate memory\n");
        return NULL;
    }

    declare_sentence(i_s);

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

void insert_source_label(Node** list, char* label, int line){
    append(list, line, label);
}

void generate_command(CommandSentence *c_s){
    c_s->label = NULL;
    c_s->pos = 0;
    c_s->src = NULL;
    c_s->dest = NULL;
    c_s->operation = -1;
    c_s->word_count = 0;
}

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

void args(CommandSentence* c_s, char* command){
    char *sus_as_arg;
    int args_count = OPERATIONS[c_s->operation].arg_count;
    if(c_s->operation == -1){
        /* TODO: ILLEGAL COMMAND HANDLING */
        return;
    }
    clear_side_blanks(&command);
    if(!args_count){ // ZERO ARGS
        if(*command != '\0'){
            printf("YOU GAY\n");
        }
    } else if (args_count == 1) { // ONE ARG
        if(strpbrk(command, ",: \t\n\v\f\r") != NULL){
            printf("BAD SHET\n");
        }
        c_s->dest = command;
    } else { // TWO ARGS
        sus_as_arg = get_word(command);
        if(sus_as_arg[strlen(sus_as_arg) - 1] == ','){
            sus_as_arg[strlen(sus_as_arg) - 1] = '\0';
        }
        c_s->src = sus_as_arg;
        command += strlen(sus_as_arg) + 1;
        clear_side_blanks(&command);
        c_s->dest = get_word(command);
        command += strlen(c_s->dest) + 1;
        if(*command != '\0'){
            printf("WELL THATS A PROBLEM NIGGA\n");
        }
    }
}

int reg_arg(char* operand){
    char *copy;

    if(!operand){
        return 0;
    }

    if(is_reg(operand)){
        return 1;
    } else {
        if(*operand == '*'){
            copy = operand + 1;
            if(is_reg(copy)){
                return 1;
            }
        } else {
            return 0;
        }
    }
    return 0;
}

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

void analyze_command(CommandSentence* c_s, char* command){
    get_command(c_s, command);
    if(c_s->operation == -1){
        /* TODO: ILLEGAL COMMAND HANDLING */
        return;
    }
    args(c_s,  strstr(command, OPERATIONS[c_s->operation].name) + strlen(OPERATIONS[c_s->operation].name));
    word_count(c_s);
}

CommandSentence *pull_command(char *command, int line){
    CommandSentence *c_s = NULL;
    size_t pre_label_len;
    char* label;
    c_s = (CommandSentence*)malloc(sizeof(CommandSentence));
    if(!c_s){
        return NULL;
    }
    generate_command(c_s);

    if(strchr(command, ':')){
        pre_label_len = strchr(command, ':') - command;
        label = (char*)malloc(pre_label_len + 1);
        if(!label){
            printf("[ERROR] Failed to allocate memory\n");
            return NULL;
        }
        strncpy(label, command, pre_label_len);

        /* TODO: SPACE AFTER COMMA */

        if(is_valid_label(&label, 0)){
            c_s->label = label;
        }

        command += pre_label_len + 1;
    }

    analyze_command(c_s, command);
    if(c_s->operation == -1){
        /* TODO: ILLEGAL COMMAND HANDLING */
        return NULL;
    }

    return c_s;
}

int first_pass(FILE* src_file, Node** labels, Node** externals, Node** entries, SentenceList* code){
    int IC = 0, DC = 0, L = 0, error_flag = 0;
    char* line = (char*)malloc(MAX_LINE_LEN);
    InstructionSentence *i_s;
    CommandSentence *c_s;
    char* copy;
    char* x;
    while (!feof(src_file) && DC + IC <= LAST_ADDRESS - FIRST_ADDRESS) {

        if (fgets(line, MAX_LINE_LEN, src_file) == NULL) {
            error_flag = 1;
            break;
        }

        if(is_ignorable(line)){
            continue;
        }

        copy = get_line_copy(line);
        clear_side_blanks_remove_newline(&copy);
        if(strchr(line, '.')) {
            if (store_or_src(get_word(strchr(line, '.'))) > 0) {
                i_s = store_data(copy);
                if (i_s->label) {
                    insert_label_table(labels, i_s->label, IC + 100 + DC);
                }
                DC += i_s->size;
                add_code(code, i_s, INSTRUCTION);
            } else if (store_or_src(get_word(strchr(line, '.'))) < 0) {
                i_s = src_handling(copy, L);
                if (i_s == NULL) {
                    printf("BAD NAME\n");
                    continue;
                }
                if (i_s->src == 0 && !exists(*externals, i_s->label, 1)) {
                    insert_source_label(entries, i_s->label, L);
                } else if (i_s->src == 1 && !exists(*entries, i_s->label, 1)) {
                    insert_source_label(externals, i_s->label, L);
                }
            }
        } else {
            c_s = pull_command(copy, L);
            if(c_s != NULL){
                if(c_s->label != NULL){
                    insert_label_table(labels, c_s->label, IC + 100);
                }
                IC += c_s->word_count;
                add_code(code, c_s, COMMAND);
            }
        }
    }

    return error_flag;
}

int get_operand_type(char* operand){
    if(operand == NULL) {
        return -1;
    } else if (*operand == '#' && is_num_legal(operand + 1)){
        return 0;
    } else if (is_reg(operand)) {
        return 3;
    } else if (*operand == '*' && is_reg(operand + 1)){
        return 2;
    } else {
        return 1;
    }
}


short int operation_as_num(CommandSentence* c_s, Node** labels,  Node** entries, Node** externals){
    int op = 0;
    int padding;
    op |= c_s->operation << 11;
    padding = get_operand_type(c_s->src);
    if(padding >= 0){
        op |= (1 << (7 + padding));
    }
    padding = get_operand_type(c_s->dest);
    if(padding >= 0){
        op |= (1 << (3 + padding));
    }
    op |= 4;
    return op;
}

int get_reg(char* reg){
    int i;
    char* reg_cpy;
    if(*reg == '*'){
        reg_cpy = reg + 1;
    } else {
        reg_cpy = reg;
    }
    for(i = 0; i < 8; i++){
        if(strcmp(reg_cpy, REGISTERS[i].name) == 0){
            return i;
        }
    }
    return -1;
}

short int two_regs(char* reg1, char* reg2){
    int src, dest;
    src = get_reg(reg1);
    dest = get_reg(reg2);
    return ((src << 6) | (dest << 3) | 4);
}

short int operand_as_code(char* operand, Node** labels, Node** externals, OPERAND path){
    int operand_as_int;
    Node* operand_node;
    if(*operand == '#' && is_num_legal(operand + 1)){
        operand_as_int = atoi(operand + 1);
        if(operand_as_int < 0){
            operand_as_int &= 0b111111111111;
        }
        return (operand_as_int << 3) | 4;
    } else if(exists(*externals, operand, 0)){
        return 1;
    } else if (exists(*labels, operand, 0)){
        operand_node = get_node(*labels,operand);
        return ((operand_node->data->line) << 3) | 2;
    } else if(reg_arg(operand)){
        if(path){
            return (get_reg(operand) << 6) | 4;
        } else {
            return (get_reg(operand) << 3) | 4;
        }
    }
    return 0;
}

void printBinary(short int num) {
    int bits = 15; // Calculate the number of bits in a short int

    for (int i = bits - 1; i >= 0; i--) {
        putchar((num & (1 << i)) ? '1' : '0');
    }
    printf(" | %d \n", num);
}

void handle_operands(short int** machine_code, CommandSentence* c_s, Node** labels, Node** externals){
    int last_index = c_s->word_count - 1;
    if(c_s->word_count == 1) {
        return;
    }
    if(reg_arg(c_s->src) && reg_arg(c_s->dest)){
        (*machine_code)[last_index] = two_regs(c_s->src, c_s->dest);
        return;
    } else {
        (*machine_code)[last_index] = operand_as_code(c_s->dest, labels, externals, DESTINATION);
    }
    if(c_s->src){
        (*machine_code)[1] = operand_as_code(c_s->src, labels, externals, SOURCE);
    }
}

short int* get_command_code(CommandSentence* c_s, Node** labels,  Node** entries, Node** externals) {
    short int* machine_code = (short int*)calloc(c_s->word_count, sizeof(short int));
    if (!machine_code) {
        printf("FAILED TO ALLOCATE MEMORY.\n");
        exit(1);
    }
    machine_code[0] = operation_as_num(c_s, labels, entries, externals);
    handle_operands(&machine_code, c_s, labels, externals);
    return machine_code;
}

char* short_to_5_digit_octal(short int num) {
    // Handle negative numbers by converting them to their 2's complement representation
    unsigned short int abs_num = (num < 0) ? (unsigned short int)(num + 0x10000) : (unsigned short int)num;

    // Allocate memory for the output string (5 digits + null terminator)
    char* result = (char*)malloc(6 * sizeof(char));
    if (result == NULL) {
        return NULL; // Allocation failed
    }

    // Convert the number to octal and store in the result buffer
    int i = 4; // Start from the last digit
    do {
        result[i--] = (abs_num % 8) + '0';
        abs_num /= 8;
    } while (abs_num > 0 && i >= 0);

    // Pad the result with '0' if necessary
    while (i >= 0) {
        result[i--] = '0';
    }

    // Null-terminate the string
    result[5] = '\0';

    return result;
}

short int* get_instruction_code(InstructionSentence* i_s){
    int i;
    short int* machine_code = (short int*)calloc(i_s->size, sizeof(short int));
    if (!machine_code) {
        printf("FAILED TO ALLOCATE MEMORY.\n");
        exit(1);
    }
    for(i = 0; i < i_s->size; i++){
        machine_code[i] = i_s->data[i];
    }
    return machine_code;
}

short int* calc_code(SentenceList* code, int* IC, int* DC, Node** labels, Node** entries, Node** externals, Node** ext_file){
    int i, L = 0;
    SenNode* current_line;
    short int* translation;
    short int* machine_code = (short int*)calloc(code->size, sizeof(short int));
    if(!machine_code){ printf("FAILED TO ALLOCATE MEMORY.\n"); exit(1); }
    current_line = code->head;
    while (current_line != NULL && L <= LAST_ADDRESS - FIRST_ADDRESS) {
        if (current_line->type == COMMAND) {
            translation = get_command_code(current_line->sentence.command, labels, entries, externals);
            if (translation != NULL) {
                for (i = 0; i < current_line->sentence.command->word_count; i++) {
                    if(translation[i] == 1){
                        if(exists(*externals, current_line->sentence.command->src, 0)){
                            append(ext_file, L, current_line->sentence.command->src);
                        } else if (exists(*externals, current_line->sentence.command->dest, 0)){
                            append(ext_file, L, current_line->sentence.command->dest);
                        }
                    }
                    machine_code[L] = translation[i];
                    (*IC)++;
                    L++;
                }
                free(translation);
            }
        } else if (current_line->type == INSTRUCTION) {
            translation = get_instruction_code(current_line->sentence.instruction);
            if (translation != NULL) {
                for (i = 0; i < current_line->sentence.instruction->size; i++) {
                    machine_code[L] = translation[i];
                    (*DC)++;
                    L++;
                }
                free(translation);
            }
        }
        current_line = (SenNode *) current_line->next;
    }
    return machine_code;
}


void write_obj_file(const char* file_name, short int* code, int IC, int DC){
    char* obj_file, *octal;
    int i;
    get_file(file_name, &obj_file, ".ob");
    FILE* f = fopen(obj_file, "w");
    if(f == NULL){
        printf("FAILED TO OPEN FILE.\n");
        return;
    }
    fprintf(f, "  %d %d  \n", IC, DC);
    for(i = 0; i < DC + IC; i++){
        octal = short_to_5_digit_octal(code[i]);
        fprintf(f, "%d\t%s\n", FIRST_ADDRESS + i, octal);
    }
    free(obj_file);
    fclose(f);
}

void write_entry_file(const char* file_name, Node** labels, Node** entries){
    char* entry_file;
    Node* label_node, *entry;
    get_file(file_name, &entry_file, ".ent");
    entry = *entries;
    if(entry == NULL){
        return;
    }
    FILE* f = fopen(entry_file, "w");
    if(!f){
        printf("FAILED TO OPEN FILE\n");
        return;
    }
    while(entry != NULL){
        label_node = get_node(*labels, entry->data->text);
        entry->data->line = label_node->data->line;
        fprintf(f, "%s %d\n", entry->data->text, entry->data->line);
        entry = entry->next;
    }
    free(entry_file);
    fclose(f);
}

//void write_extern_file(const char* file_name, Node** ext_file){
//    char* ext_name;
//    Node* ext_label;
//    get_file(file_name, &ext_name, ".ext");
////    ext_label = *ext_file;
////    if(ext_label == NULL){
////        return;
////    }
////    FILE* f = fopen(ext_name, "w");
////    if(!f){
////        printf("FAILED TO OPEN FILE\n");
////        return;
////    }
//////    print_list(ext_label);
////    while(ext_label != NULL){
////        fprintf(f, "%s %d\n", ext_label->data->text, ext_label->data->line);
////        ext_label = ext_label->next;
////    }
//    free(ext_name);
////    fclose(f);
//}

int second_pass(const char* file_name, Node** labels,  Node** entries, Node** externals, SentenceList* code){
    int IC = 0, i, L = 0, DC = 0;
    Node* ext_file = NULL;
    short int* machine_code = calc_code(code, &IC, &DC, labels, entries, externals, &ext_file);
    write_obj_file(file_name, machine_code, IC, DC);
    write_entry_file(file_name, labels, entries);
    free(machine_code);
    return 0;
}

char* get_line_copy(const char* origin_line){
    char* copy = malloc(strlen(origin_line) + 1);
    if(!copy){
        printf("[ERROR] Failed to allocate memory");
        return NULL;
    }
    strcpy(copy, origin_line);
    copy[strlen(origin_line)] = '\0';
    return copy;
}

void get_file(const char* file_name, char** input_file, const char* suffix){
    size_t name_len = strlen(file_name);
    *input_file = (char*)malloc(name_len + 4);
    if(*input_file == NULL){
        printf("[ERROR] Failed to allocate memory,\n");
        return;
    }
    strcpy(*input_file, file_name);
    strcat(*input_file, suffix);
}

int assembler(const char* file_name){
    char* src_file_name;
    FILE *obj, *ext, *ent, *assembly;
    Node* externals = NULL;
    Node* entries = NULL;

    Node* labels = NULL;
    SentenceList* code = NULL;

    get_file(file_name, &src_file_name, ".am");

    assembly = fopen(src_file_name, "r");
    if (assembly == NULL) {
        printf("[ERROR] Failed to open file %s\n", src_file_name);
        free(src_file_name);
        return 1;
    }

    code = (SentenceList*)malloc(sizeof(SentenceList));
    if (code == NULL) {
        printf("[ERROR] Failed to allocate memory for SentenceList\n");
        return 1;
    }
    code->head = NULL;
    code->size = 0;

    first_pass(assembly, &labels, &externals, &entries, code);
    second_pass(file_name, &labels, &entries, &externals , code);

    freeSentenceList(code);
    free(code);

    return 0;
}