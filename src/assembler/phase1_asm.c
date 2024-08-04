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
    if(is_valid_instruction(label) || is_operation(label) || is_reg(label) || strlen(label) > MAX_LABEL_LEN){
        return 0;
    }
    if(!isalpha(*label)){
        return 0;
    }
    label++;
    while(*label != '\0' && isalnum(*(label))){ label++; }
    if(*label == ':' && *(label + 1) == '\0'){
        *label = '\0';
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
        clear_side_blanks_remove_newline(&token);
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

instruction_sentence* store_data(char* line){
    int i;
    instruction_sentence *sen;
    char *label;
    size_t data_size;
    char* text;
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
        text = strstr(line, ".data") + strlen(".data");
        sen->data = pull_numbers(text, &data_size);
        sen->size = data_size;
    } else {
        text = strstr(line, ".string") + strlen(".string");
        sen->data = get_ascii(text, &data_size);
        sen->size = data_size;
    }

    if(sen->data){
        for(i = 0; i < sen->size; i++){
            printf("[DATA] %d\n", *(sen->data + i));
        }
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
    char* x;
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
            x = get_word(strchr(line, '.'));
            if(store_or_src(get_word(strchr(line, '.'))) > 0){
                i_s = store_data(copy);
                free(i_s);
            } else if (store_or_src(get_word(strchr(line, '.'))) < 0){
                // instruction of src
            }
            // add label into the label table
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