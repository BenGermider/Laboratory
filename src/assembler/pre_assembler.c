#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../include/assembler/pre_assembler.h"
#include "../../include/common/collections/hash_table.h"
#include "../../include/common/library.h"
#include "../../include/common/utils.h"
#include "../../include/common/consts.h"
#include "../../include/common/collections/linked_list.h"

/**
 * Check validity of macro name
 * @param ht collections of macros
 * @param macro_line line where macro name is written
 * @param macro_name pointer for the macro name to be stored
 * @return if valid macro name.
 */
int is_valid(HashTable *ht, char* macro_line, char* macro_name){
    int i;
    clear_side_blanks_remove_newline(&macro_line);
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

char* analyze(
        char* line,
        int* in_macro,
        HashTable* ht,
        char* macro_name,
        char* macro_content,
        Node** errors,
        int current_line
        ){

    char* macro;
    char* macro_name_start;
    char *macro_line = (char*)malloc(strlen(line) + 1);

    if (macro_line == NULL) {
        return NULL;
    }

    strcpy(macro_line, line);
    clear_side_blanks_remove_newline(&macro_line);
    if (strncmp(macro_line, MACRO_START, strlen(MACRO_START)) == 0) {

        macro_name_start = macro_line + strlen(MACRO_START);

        if (!is_valid(ht, macro_name_start, macro_name)) {
            if(append(errors, current_line, "Bad macro name")){
                free(macro_line);
                printf("got something bad");
                return NULL;
            }
            free(macro_line);
            return "";
        }

        *in_macro = 1;
        free(macro_line);
        return "";
    }

    else if (strncmp(macro_line, MACRO_END, strlen(MACRO_END)) == 0) {
        insert(ht, macro_name, macro_content);
        *in_macro = 0;
        macro_name[0] = '\0';
        macro_content[0] = '\0';

        free(macro_line);
        return "";
    }

    else if (*in_macro) {
        strcat(macro_content, line);
        free(macro_line);
        return "";
    }


    else if ((macro = get(ht, macro_line))) {
        free(macro_line);
        return macro;
    }

    free(macro_line);
    return line;
}


int order_as_file(FILE *input_file, FILE *output_file, Node** errors_list, HashTable* macros) {
    int in_macro = 0;
    int current_line = 1;
    char* to_output;

    char *line = (char*) malloc(BUFFER);
    char *macro_name = (char*) malloc(MAX_LINE_LEN);
    char *macro_content = (char*)malloc(BUFFER);

    if(!line || !macro_name || !macro_content){
        free_space(3, line, macro_content, macro_name);
        return 1;
    }

    while (fgets(line, BUFFER, input_file)) {
        to_output = analyze(line, &in_macro, macros, macro_name, macro_content, errors_list, current_line);
        if (to_output == NULL) {
            free_space(3, line, macro_name, macro_content);
            return 1;
        }
        if(to_output[0] != '\0'){
            fprintf(output_file, "%s", to_output);
        }
        current_line++;
    }
    return 0;
}

int terminate(Node* errors_list, FILE* f_output, FILE* f_input, char* output_name, char* input_name, int heap_err){
    fclose(f_input);
    fclose(f_output);
    remove(output_name);
    if(heap_err){
        printf("Failed to allocate memory, terminating.\n");
    }
    free_space(3, output_name, input_name, errors_list);
    return 1;
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

int pre_assembler(char* name_of_file, HashTable* macros) {
    int result;
    char *input_file_name = NULL, *output_file_name = NULL;
    FILE *file_to_scan, *file_to_write;
    Node* errors_list = NULL;
    file_names(name_of_file, &input_file_name, &output_file_name);
    if (input_file_name == NULL || output_file_name == NULL) {
        return 1;
    }
    file_to_scan = fopen(input_file_name, "r+");
    if (file_to_scan == NULL) {
        printf("[ERROR] Failed to open file %s\n", input_file_name);
        free(input_file_name);
        free(output_file_name);
        return 1;
    }
    file_to_write = fopen(output_file_name, "w");
    if (file_to_write == NULL) {
        printf("[ERROR] Failed to open file %s\n", output_file_name);
        fclose(file_to_scan);
        free(input_file_name);
        free(output_file_name);
        return 1;
    }

    result = order_as_file(file_to_scan, file_to_write, &errors_list, macros);
    if(result || errors_list != NULL){
        print_list(errors_list);
        return terminate(errors_list, file_to_write, file_to_scan, output_file_name, input_file_name, result);
    }

    fclose(file_to_scan);
    fclose(file_to_write);

    free_space(3, input_file_name, output_file_name, errors_list);

    return 0;
}
