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

int is_valid_label(char *label, int is_decl){
    clear_side_blanks_remove_newline(&label);

    int i = 1;
    if(is_valid_instruction(label) || is_operation(label) || is_reg(label) || strlen(label) > MAX_LABEL_LEN){
        return 0;
    }
    if(!isalpha(*label)){
        return 0;
    }
    label++;
    while(*label != '\0' && isalnum(*(label))){ label++; i++; }
    if(is_decl && *label == ':' && *(label + 1) == '\0') {
        *(label) = '\0';  /* TODO: FIX */
        return 1;
    } else if (!is_decl && *label == '\0' || isspace(*label)){
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
    int i;
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

    pre_label_len= strchr(line, '.') - line;

    if(pre_label_len > 0){
        label = malloc(pre_label_len + 1);
        if(!label){
            printf("[ERROR] Failed to allocate memory\n");
            return NULL;
        }
        strncpy(label, line, pre_label_len);
        if(is_valid_label(label, 1)){
            sen->label = label;
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
        if(is_valid_label(label, 0)){
            i_s->label = label;
        } else {
            return NULL;
        }
    } else {
        label = strstr(line, INSTRUCTIONS[3].name) + strlen(INSTRUCTIONS[3].name);
        if(is_valid_label(label, 0)){
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
    c_s->src = NULL;
    c_s->dest = NULL;
    c_s->operation = -1;
    c_s->ARE = 0;
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
        if(is_valid_label(label, 0)){
            c_s->label = label;
        }

    }

    return c_s;

}

int generate_file(FILE* src_file, Node* labels, Node* externals, Node* entries){
    int IC = 0, DC = 0, L = 1, error_flag = 0;
    char* line = (char*)malloc(MAX_LINE_LEN);
    InstructionSentence *i_s;
    CommandSentence *c_s;
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

        copy = get_line_copy(line);
        clear_side_blanks_remove_newline(&copy);
        if(strchr(line, '.')){
            if(store_or_src(get_word(strchr(line, '.'))) > 0){
                i_s = store_data(copy);
                if(i_s->label){
                    insert_label_table(&labels, i_s->label, L);
                }
                free(i_s);
            } else if (store_or_src(get_word(strchr(line, '.'))) < 0){
                i_s = src_handling(copy, L);
                if(i_s == NULL){
                    printf("BAD NAME\n");
                    continue;
                }
                if(i_s->src == 0 && !exists(externals, i_s->label, 1)){
                    insert_source_label(&entries, i_s->label, L);
                } else if(i_s->src == 1 && !exists(entries, i_s->label, 1)){
                    insert_source_label(&externals, i_s->label, L);
                }
            }
        }

        else {
            c_s = pull_command(copy, L);
            if(c_s != NULL){
                IC++;
                if(c_s->label != NULL){
                    // ADD TO LABEL TABLE
                    printf("ACTUALLY GOT LABEL %s\n", c_s->label);
                }
            }
        }
        L++;
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
    Node* externals = NULL;
    Node* entries = NULL;
    Node* labels = NULL;
    get_file(file_name, &src_file_name);

    assembly = fopen(src_file_name, "r");
    if (assembly == NULL) {
        printf("[ERROR] Failed to open file %s\n", src_file_name);
        free(src_file_name);
        return 1;
    }

    generate_file(assembly, labels, externals, entries);

    return 0;
}