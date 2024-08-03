#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../include/assembler/pre_assembler.h"
#include "../../include/common/collections/hash_table.h"
#include "../../include/common/library.h"
#include "../../include/common/utils.h"
#include "../../include/common/consts.h"

/**
 * Check validity of macro name
 * @param ht collections of macros
 * @param macro_line line where macro name is written
 * @param macro_name pointer for the macro name to be stored
 * @return if valid macro name.
 */
int is_valid(HashTable *ht, char* macro_line, char* macro_name){
    int i;
    clear_side_blanks(&macro_line);
    if(strcmp(strtok(macro_line, " "), macro_line) != 0){
        return 0;
    }
    if(isdigit(*macro_line)){
        return 0;
    }
    if(strlen(macro_line) > MAX_MACRO_NAME_LEN){
        return 0;
    }
    if(get(ht, macro_line)){
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
    char *macro_line = (char*)malloc(strlen(line) + 1);
    char *macro_check = (char*)malloc(strlen(line) + 1);
    if(macro_line == NULL || macro_check == NULL){
        printf("[ERROR] Failed to allocate memory\n");
        return NULL;
    }
    strcpy(macro_line, line);
    clear_side_blanks(&macro_line);
    size_t line_len;
    strcpy(macro_check, macro_line);
    if (strcmp(strtok(macro_check, " "), MACRO_START) == 0) {
        macro_check += 5; /* Got macro declaration, now check name */
        if (!is_valid(ht, macro_check, macro_name)) {
            printf("[ERROR] Bad macro name\n");
            return NULL;
        }
        *in_macro = 1;
        return "";
    } else if (strncmp(macro_line, MACRO_END, strlen(MACRO_END)) == 0){
        insert(ht, macro_name, macro_content);
        *in_macro = 0;
        macro_name[0] = '\0';
        macro_content[0] = '\0';
        return "";
    } else if (*in_macro){
        strcat(macro_content, line);
        line_len = strlen(macro_content);
        macro_content[line_len] = '\0';
        return "";
    } else if ((macro = get(ht, macro_line))) {
        return macro;
    }
    return line;
}

int gen(FILE *input_file, FILE *output_file) {
    HashTable *ht = createHashTable();
    int in_macro = 0;
    char* to_output;
    char* line = (char*)malloc(MAX_LINE_LEN);
    char* macro_name = (char*)malloc(MAX_LINE_LEN);
    char* macro_content = (char*)malloc(1000);
    if(line == NULL || macro_content == NULL || macro_name == NULL){
        printf("[ERROR] Failed to allocate memory\n");
        return 1;
    }

    while (!feof(input_file)) {
        if (fgets(line, MAX_LINE_LEN, input_file) == NULL) {
            break;
        }
        to_output = analyze(line, &in_macro, ht, macro_name, macro_content);
        if (to_output == NULL) {
            free_space(3, line, macro_name, macro_content);
            return 1;
        }
        if(to_output[0] != '\0'){
            fprintf(output_file, "%s", to_output);
        }
    }
    free_space(3, line, macro_name, macro_content);
    return 0;
}

void file_names(const char* name, char** input, char** output) {
    size_t name_size = strlen(name);

    *input = (char*)malloc(name_size + 4);
    *output = (char*)malloc(name_size + 4);

    if (*input == NULL || *output == NULL) {
        printf("[ERROR] Failed to allocate memory\n");
        return;
    }

    strcpy(*input, name);
    strcat(*input, ".as");

    strcpy(*output, name);
    strcat(*output, ".am");
}

int pre_assembler(char* name_of_file) {
    int result;
    char *input_file_name = NULL, *output_file_name = NULL;

    file_names(name_of_file, &input_file_name, &output_file_name);

    if (input_file_name == NULL || output_file_name == NULL) {
        return 1;
    }

    FILE *file_to_scan = fopen(input_file_name, "r+");
    if (file_to_scan == NULL) {
        printf("[ERROR] Failed to open file %s\n", input_file_name);
        free(input_file_name);
        free(output_file_name);
        return 1;
    }

    FILE *file_to_write = fopen(output_file_name, "w");
    if (file_to_write == NULL) {
        printf("[ERROR] Failed to open file %s\n", output_file_name);
        fclose(file_to_scan);
        free(input_file_name);
        free(output_file_name);
        return 1;
    }

    result = gen(file_to_scan, file_to_write);
    if(result){
        remove(output_file_name);
    }

    fclose(file_to_scan);
    fclose(file_to_write);

    // Free allocated memory
    free(input_file_name);
    free(output_file_name);

    return 0;
}