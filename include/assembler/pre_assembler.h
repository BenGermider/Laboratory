#ifndef LABORATORY_PRE_ASSEMBLER_H
#define LABORATORY_PRE_ASSEMBLER_H
#include <stdio.h>
#include "../common/collections/hash_table.h"
#include "../common/collections/linked_list.h"

int is_valid(HashTable*, char*, char*);

char* analyze(char*, int*, HashTable*, char*, char*, Node**, int);

int order_as_file(FILE*, FILE*, Node**, HashTable*);

int terminate(Node*, FILE*, FILE*, char*, char*, int);

void file_names(const char*, char**, char**);

int pre_assembler(char*, HashTable*);

#endif
