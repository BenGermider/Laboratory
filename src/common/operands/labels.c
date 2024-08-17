#include <ctype.h>
#include <stdio.h>
#include "../../../include/common/utils.h"
#include "../../../include/common/operands/labels.h"
#include "../../../include/common/operands/registers.h"

/**
 * Checks whether label declaration is valid
 * @param label to check if valid
 * @param is_decl is a label declaration
 * @return 0 if invalid, 1 if valid.
 */
int is_valid_label(char **label, int is_decl) {
    char *p;
    clear_side_blanks(label, 1);

    p = *label;
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

int valid_label_decl(char** label, HashTable *macros){
    int i;
    if(!is_valid_label(label, 1)){
        return 0;
    }
    /*if(get(macros, *label)){
        return 0;
    }*/
    if(reg_arg(*label)){
        return 0;
    }
    for(i = 0; i < 16; i++){
        if(strcmp(OPERATIONS[i].name, *label) == 0){
            return 0;
        }
    }
    for(i = 0; i < 4; i++){
        if(strcmp(INSTRUCTIONS[i].name, *label) == 0){
            return 0;
        }
    }
    return 1;
}

/**
 * Insert the label into the label table
 * @param database of labels
 * @param label to add to
 * @param lines the line the label declared
 * @return code of success or failure
 */
int insert_label_table(Node **database, char *label, int lines){
    if(!exists(*database, label, 0)){
        return append(database, lines, label);
    } else {
        return -1;
    }
}

/**
 * Insert to a source label database
 * @param list to insert
 * @param label string to insert
 * @param line to insert
 * @return code of success or failure
 */
int insert_source_label(Node** list, char* label, int line){
    return append(list, line, label);
}

