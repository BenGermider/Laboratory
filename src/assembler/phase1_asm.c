#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/common/consts.h"
#include "../../include/assembler/phase1_asm.h"


int generate_file(FILE* src_file){
    int IC = 0, DC = 0, L = 0, error_flag = 0;
    char* line = (char*)malloc(MAX_LINE_LEN);
    char* copy;
    while (!feof(src_file)) {
        if (fgets(line, MAX_LINE_LEN, src_file) == NULL) {
            break;
        }
        if(strcmp(line, "\n") == 0){
            continue;
        }

    }
    return error_flag;
}

void get_line_copy(const char* origin_line, char** copy_line){

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
    get_file(file_name, &src_file_name);

    return 0;
}