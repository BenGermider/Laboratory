#include <ctype.h>
#include "../../../include/common/consts.h"
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
    char *copy;
    clear_side_blanks(label, 1);
    copy = *label;
    if(!isalpha(*copy)){
        return 0;
    }
    copy++;
    while(*copy != '\0' && isalnum(*copy)) {
        copy++;
    }
    if(is_decl && *copy == ':' && *(copy + 1) == '\0') {
        *copy = '\0';
        return 1;
    } else if (!is_decl && (*copy == '\0' || isspace(*copy))) {
        return 1;
    }
    return 0;
}

/**
 * Checks whether label holds a name it is not allowed to hold
 * @param label to check
 * @param errors database of errors
 * @param current_line current line converting
 * @return 0 if holds a bad name, 1 otherwise
 */
int label_valid_name(char** label, Node** errors, int current_line){
    int i;
    if(reg_arg(*label)){
        append(errors, current_line, "Label cannot be a register operand.");
        return 0;
    }
    for(i = 0; i < OP_COUNT; i++){
        if(strcmp(OPERATIONS[i].name, *label) == 0){
            append(errors, current_line, "Label cannot be a command.");
            return 0;
        }
    }
    for(i = 0; i < INST_COUNT; i++){
        if(strcmp(INSTRUCTIONS[i].name, *label) == 0){
            append(errors, current_line, "Label cannot be an instruction.");
            return 0;
        }
    }
    if(strlen(*label) > MAX_LABEL_LEN){
        append(errors, current_line, "Label is too long.");
        return 0;
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
    if(!exists(*database, label, 0, 0)){
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
