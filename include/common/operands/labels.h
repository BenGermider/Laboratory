#ifndef LABORATORY_LABELS_H
#define LABORATORY_LABELS_H

#include "../collections/linked_list.h"
#include "../collections/hash_table.h"

/* Checks whether label declaration is valid */
int is_valid_label(char **label, int is_decl);

int valid_label_decl(char**, hash_table*);

/* Insert a label into the labels table */
int insert_label_table(Node **database, char *label, int lines);

/* Insert a label into a database of labels from a specific source */
int insert_source_label(Node** list, char* label, int line);

#endif /* LABORATORY_LABELS_H */
