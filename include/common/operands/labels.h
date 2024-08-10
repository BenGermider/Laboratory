#ifndef LABORATORY_LABELS_H
#define LABORATORY_LABELS_H

#include "../collections/linked_list.h"

int is_valid_label(char **label, int is_decl);

int insert_label_table(Node **database, char *label, int lines);

void insert_source_label(Node** list, char* label, int line);

#endif /* LABORATORY_LABELS_H */
