#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/pre_assembler.h"
#include "../include/common/hash_table.h"
#include "../include/common/library.h"
#include "../include/common/utils.h"
#include "../include/common/consts.h"

int is_valid(char* macro_line, char* macro_name){
    int i;
    clear_side_blanks(&macro_line);
    if(strcmp(strtok(macro_line, " "), macro_line) != 0){
        return 0;
    }
    for(i = 0; i < 16; i++){
        if(strcmp(macro_line, OPERATIONS[i].name) == 0){
            return 0;
        }
    }
    for(i = 0; i < 4; i++){
        if(strcmp(macro_line, INSTRUCTIONS[i].name) == 0){
            return 0;
        }
    }
    strcpy(macro_name, macro_line);
    return 1;
}

char* analyze(char* line, int* in_macro, HashTable* ht, char* macro_name, char* macro_content) {
    char* macro;
    clear_side_blanks(&line);
    size_t line_len;
    char *macro_line = (char*)malloc(strlen(line) + 2);
    if(macro_line == NULL){
        printf("[ERROR] Failed to allocate memory\n");
        return NULL;
    }
    strcpy(macro_line, line);
    if (strcmp(strtok(macro_line, " "), MACRO_START) == 0) {
        macro_line += 5; /* Got macro declaration, now check name */
        if (!is_valid(macro_line, macro_name)) {
            printf("[ERROR] Bad macro name\n");
            return NULL;
        }
        *in_macro = 1;
    } else if (strcmp(line, MACRO_END) == 0){
        insert(ht, macro_name, macro_content);
        *in_macro = 0;
        macro_name[0] = '\0';
        macro_content[0] = '\0';
        return NULL;
    } else if (*in_macro){
        strcat(macro_content, line);
        line_len = strlen(macro_content);
        macro_content[line_len] = '\n';
        macro_content[line_len + 1] = '\0';
        return NULL;
    } else if ((macro = get(ht, line))) {
        return macro;
    } else {
        return macro_line;
    }
}


FILE* generate_file(FILE *input_file, FILE *output_file) {
    HashTable *ht = createHashTable();
    int in_macro = 0;
    char* to_output;
    char* line = (char*)malloc(MAX_LINE_LEN);
    char* macro_name = (char*)malloc(MAX_LINE_LEN);
    char* macro_content = (char*)malloc(1000);
    if(line == NULL || macro_content == NULL || macro_name == NULL){
        printf("[ERROR] Failed to allocate memory\n");
        return NULL;
    }

    while (!feof(input_file)) {
        if (fgets(line, MAX_LINE_LEN, input_file) == NULL) {
            break;
        }
        to_output = analyze(line, &in_macro, ht, macro_name, macro_content);
    }
//    printf("%s\n", macro_content);

    free(line);
    free(macro_name);
    free(macro_content);
    return output_file;
}

int pre_assembler(char* name_of_file){

    FILE *file_to_scan, *post_analysis;
    file_to_scan = fopen(name_of_file, "r+");
    file_to_write = fopen()
    if(!file_to_scan){
        printf("[ERROR] Failed to open file %s\n", name_of_file);
        return 1;
    }
    post_analysis = generate_file(file_to_scan, post_analysis);
    fclose(file_to_scan);
    return 0;
}