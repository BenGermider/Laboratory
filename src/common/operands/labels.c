#include <ctype.h>
#include <stdio.h>
#include "../../../include/common/utils.h"
#include "../../../include/common/collections/linked_list.h"

/**
 * Checks whether label declaration is valid
 * @param label to check if valid
 * @param is_decl is a label declaration
 * @return 0 if invalid, 1 if valid.
 */
int is_valid_label(char **label, int is_decl) {
    char *p;
    clear_side_blanks_remove_newline(label);

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

/**
 * Insert the label into the label table
 * @param database of labels
 * @param label to add to
 * @param lines the line the label declared
 * @return code of success or failure
 */
int insert_label_table(Node **database, char *label, int lines){
    if(!exists(*database, label, 0)){
        append(database, lines, label);
    } else {
        printf("[ERROR] Label already exists.\n");
        return 1;
    }
    return 0;
}

/**
 * Insert to a source label database
 * @param list of source database
 * @param label to add
 * @param line appears
 */
void insert_source_label(Node** list, char* label, int line){
    append(list, line, label);
}

