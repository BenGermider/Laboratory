#include <ctype.h>
#include <stdio.h>
#include "../../../include/common/utils.h"
#include "../../../include/common/collections/linked_list.h"

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

int insert_label_table(Node **database, char *label, int lines){
    if(!exists(*database, label, 0)){
        append(database, lines, label);
    } else {
        printf("[ERROR] Label already exists.\n");
        return 1;
    }
    return 0;
}

void insert_source_label(Node** list, char* label, int line){
    append(list, line, label);
}

