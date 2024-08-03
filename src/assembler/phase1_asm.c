#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../include/common/consts.h"
#include "../../include/assembler/phase1_asm.h"
#include "../../include/common/utils.h"
#include "../../include/common/data_types.h"
#include "../../include/common/library.h"

int is_reg(char *str){
    int i;
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

int is_valid_label(char *label){
    char *str, *str_copy;
    str = get_line_copy(label);
    str_copy = str;

    if(is_valid_instruction(str) || is_operation(str) || is_reg(str) || strlen(str) > MAX_LABEL_LEN){
        free(str_copy);
        return 0;
    }
    if(!isalpha(*str)){
        free(str_copy);
        return 0;
    }
    str++;
    while(*str != '\0' && isalnum(*(str))){ str++; }

    if(*str == ':' && *(str + 1) == '\0'){
        free(str_copy);
        return 1;
    }
    free(str_copy);
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


void declare_sentence(instruction_sentence *sen){
    sen->label = NULL;
    sen->data = NULL;
    sen->src = 0;
    sen->size = 0;
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
        clear_side_blanks(&token);
        token_copy = token;
        while(*token_copy != '\0'){
            if(isspace(*token_copy)){
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
                return NULL;
            }
        }
        token = strtok(NULL, ",");
    }
    *size = i;
    return arr;
}

int* pull_ascii(char* line, size_t size){

    return NULL;
}

instruction_sentence* store_data(char* line){
    instruction_sentence *sen;
    char *label;
    size_t data_size;
    char* string;
    size_t pre_label_len = strchr(line, '.') - line;

    sen = (instruction_sentence*)malloc(sizeof(instruction_sentence));
    if(!sen){
        printf("[ERROR] Failed to allocate memory\n");
        return NULL;
    }

    declare_sentence(sen);

    if(pre_label_len > 0){
        label = malloc(pre_label_len + 1);
        if(!label){
            printf("[ERROR] Failed to allocate memory\n");
            return NULL;
        }
        strncpy(label, line, pre_label_len);
        label[pre_label_len] = '\0';
        clear_side_blanks(&label);
        if(is_valid_label(label)){
            sen->label = label;
        }
    }

    if(strcmp(get_word(strchr(line, '.')), ".data") == 0){
        sen->data = pull_numbers((line+5), &data_size);
        sen->size = data_size;
    } else {

    }

    if(sen->label){
       printf("[LABEL] %s\n", sen->label);
    }

    return sen;
}

int generate_file(FILE* src_file){
    int IC = 0, DC = 0, L = 0, error_flag = 0;
    char* line = (char*)malloc(MAX_LINE_LEN);
    instruction_sentence *i_s;
    command_sentence *c_s;
    char* copy;
    while (!feof(src_file)) {

        if (fgets(line, MAX_LINE_LEN, src_file) == NULL) {
            error_flag = 1;
            break;
        }

        if(is_ignorable(line)){
            continue;
        }

        if(strchr(line, '.')){
            copy = get_line_copy(line);
            clear_side_blanks(&copy);
            if(store_or_src(get_word(strchr(line, '.'))) > 0){
                i_s = store_data(copy);
                free(i_s);
            } else if (store_or_src(get_word(strchr(line, '.'))) < 0){
                // instruction of src
            }

        }

        // read_command

    }
    return error_flag;
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

void get_file(const char* file_name, char** input_file){
    size_t name_len = strlen(file_name);
    *input_file = (char*)malloc(name_len + 4);
    if(*input_file == NULL){
        printf("[ERROR] Failed to allocate memory,\n");
        return;
    }
    strcpy(*input_file, file_name);
    strcat(*input_file, ".am");
}

int assembler(const char* file_name){
    char* src_file_name;
    FILE *obj, *ext, *ent, *assembly;
    get_file(file_name, &src_file_name);

    assembly = fopen(src_file_name, "r");
    if (assembly == NULL) {
        printf("[ERROR] Failed to open file %s\n", src_file_name);
        free(src_file_name);
        return 1;
    }

    generate_file(assembly);

    return 0;
}