#ifndef LABORATORY_LABELS_H
#define LABORATORY_LABELS_H

#include "../collections/linked_list.h"
#include "../collections/hash_table.h"

/* Checks if label includes only legit characters */
int is_valid_label(char**, int);

/* Checks if label declared according to the rules */
int label_valid_name(char**, Node**, int);

/* Insert a label into the labels table */
int insert_label_table(Node**, char*, int);

/* Insert a label into a database of labels from a specific source */
int insert_source_label(Node**, char*, int);

#endif /* LABORATORY_LABELS_H */
