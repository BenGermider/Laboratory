#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../include/assembler/pre_assembler.h"
#include "../../include/common/operands/registers.h"
#include "../../include/common/library.h"
#include "../../include/common/utils.h"
#include "../../include/common/consts.h"

/**
 * Check validity of macro name
 * @param h_table collections of macros
 * @param macro_line line where macro name is written
 * @param macro_name pointer for the macro name to be stored
 * @return if valid macro name.
 */
int is_valid(char* macro_line, char* macro_name){
    int i;
    clear_side_blanks(&macro_line, 0);
    if(strcmp(strtok(macro_line, " "), macro_line) != 0){
        /* Redundant characters in macro declaration */
        return 0;
    }
    if(isdigit(*macro_line)){
        /* Macro name starts with digit */
        return 0;
    }
    if(strlen(macro_line) > MAX_MACRO_NAME_LEN){
        return 0;
    }
    /* Checking for forbidden macro names */
    for(i = 0; i < OP_COUNT; i++){
        if(strcmp(macro_line, OPERATIONS[i].name) == 0){
            return 0;
        }
    }
    for(i = 0; i < 4; i++){
        if(strcmp(macro_line, INSTRUCTIONS[i].name) == 0){
            return 0;
        }
    }
    if(reg_arg(macro_line)){
        return 0;
    }
    strcpy(macro_name, macro_line);
    return 1;
}

/**
 * Receives a line and adds it to the macro if it is part of it, and make sure you don't write it into
 * the new file.
 * @param line to analyze if part of a macro and handle it
 * @param in_macro flag if inside a macro
 * @param h_table database of macros
 * @param macro_name name of macro
 * @param macro_content commands inside the macro
 * @param errors if there are any errors
 * @param current_line number of line in the file currently analyzing.
 * @return string to put in the file
 */
char* analyze(
        char* line,
        int* in_macro,
        hash_table* h_table,
        char* macro_name,
        char* macro_content,
        Node** errors,
        int current_line
        ){

    char* macro;
    char* macro_name_start;
    char* extra_char;
    char *macro_line = (char*)malloc(strlen(line) + 1);

    if (macro_line == NULL) {
        /* Failed to allocate memory - critical error */
        return NULL;
    }
    strcpy(macro_line, line);
    clear_side_blanks(&macro_line, 0);
    if(macro_line[0] == ';'){
        free(macro_line);
        return line;
    }
    if ((extra_char = strstr(macro_line, MACRO_END))) {
        /* Found macro ending, add it into the macro database */
        if(extra_char - macro_line != 0){
            append(errors, current_line, "Label declaration did not close properly.");
        }
        if(*(extra_char + strlen(MACRO_END)) != '\0'){
            append(errors, current_line, "Label declaration did not close properly.");
        }
        insert(h_table, macro_name, macro_content);
        *in_macro = 0;
        macro_name[0] = '\0';
        macro_content[0] = '\0';

        free(macro_line);
        return "";
    } else if ((extra_char = strstr(macro_line, MACRO_START))) {
        if(extra_char - macro_line != 0){
            append(errors, current_line, "Label declaration did not open properly.");
        }
        /* Found a macro declaration */
        macro_name_start = extra_char + strlen(MACRO_START);
        if(*macro_name_start == '\0'){
            append(errors, current_line, "Missing macro declaration.");
            free(macro_line);
            return "";
        }
        clear_side_blanks(&macro_name_start, 0);
        if (!is_valid(macro_name_start, macro_name)) {
            /* Handle macro name declaration, alert if errors */
            if(append(errors, current_line, "Bad macro name")){
                free(macro_line);
                return "";
            }
            free(macro_line);
            /* Macro is not part of .am file, return empty string to not write it to .am */
            return "";
        }
        if(get(h_table, macro_name)){
            append(errors, current_line, "Macro already exists");
        }
        if(*(macro_name_start + strlen(macro_name)) != '\0'){
            append(errors, current_line, "Label declaration did not open properly.");
        }
        *in_macro = 1;
        free(macro_line);
        return "";
    } else if (*in_macro) {
        /* add line into the macro */
        strcat(macro_content, line);
        free(macro_line);
        return "";
    }

    else if ((macro = get(h_table, macro_line))) {
        /* Found a macro call, write it's content into the file */
        free(macro_line);
        return macro;
    }

    free(macro_line);
    /* Nothing to do with macro, write the line as is in the .am file */
    return line;
}


/**
 * Reads the file, saves macros in macro DB, writes the .am file without macros and alert if any errors occurred
 * while declaring macros.
 * @param input_file .as file to read.
 * @param output_file .am file to write.
 * @param errors_list list of errors found while saving macros.
 * @param macros database of macros.
 * @return code of success or failure.
 */
int order_as_file(FILE *input_file, FILE *output_file, Node** errors_list, hash_table* macros) {
    int in_macro = 0;
    int current_line = 1;
    char* to_output;
    char *line = (char*)malloc(BUFFER);
    char *macro_name = (char*)malloc(MAX_LINE_LEN);
    char *macro_content = (char*)malloc(BUFFER);

    if(!line || !macro_name || !macro_content){
         /*Memory allocation error*/
        free_space(3, line, macro_content, macro_name);
        return 1;
    }

    while (fgets(line, BUFFER, input_file)) {
         /*Read from a file */
        to_output = analyze(line, &in_macro, macros, macro_name, macro_content, errors_list, current_line);
        if (to_output == NULL) {
             /*Encountered memory allocation error*/
            free_space(3, line, macro_name, macro_content);
            return 1;
        }
        if(to_output[0] != '\0'){
             /*Lines of coding writing into the output file*/
            fprintf(output_file, "%s", to_output);
        }
        current_line++;
    }
    free_space(3, line, macro_name, macro_content);
    return 0;
}

/**
 * Function to shutdown the run and free all dynamically allocated memory.
 * @param errors_list errors to print until now.
 * @param f_output output file to close.
 * @param f_input input file to close.
 * @param output_name string to free
 * @param input_name string to free
 * @param heap_err flag of memory allocation failure.
 * @return code of success or failure.
 */
int terminate(FILE* f_output, FILE* f_input, char* output_name, char* input_name, int heap_err){
    fclose(f_input);
    fclose(f_output);
    remove(output_name);
    if(heap_err){
        printf("Failed to allocate memory, terminating.\n");
    }
    free_space(2, output_name, input_name);
    return 1;
}

/**
 * Pre-assembler main function. Opens two files, to read and to write, and copy the assembly content
 * into the file without macro declaration and calls converted into macros.
 * @param name_of_file to analyze
 * @param macros database of macros
 * @return code of success or failure.
 */
int pre_assembler(char* name_of_file, hash_table* macros, Node** errors) {
    int result = 0;
    char *input_file_name = NULL, *output_file_name = NULL;

    FILE *file_to_scan, *file_to_write;
    /* Get the name of the files */
    if(get_file(name_of_file, &input_file_name, ".as")){
        return 1;
    }

    if(get_file(name_of_file, &output_file_name, ".am")){
        free(input_file_name);
        return 1;
    }

    /* Open the files */
    file_to_scan = fopen(input_file_name, "r");
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

    /* Get the file converted */
    result = order_as_file(file_to_scan, file_to_write, errors, macros);
    /* If encountered errors, let the user know */

    if(result || *errors != NULL){
        print_list(*errors);
        return terminate(file_to_write, file_to_scan, output_file_name, input_file_name, result);
    }

    fclose(file_to_scan);
    fclose(file_to_write);

    free_space(2, input_file_name, output_file_name);

    return 0;
}
