#ifndef LABORATORY_PRE_ASSEMBLER_H
#define LABORATORY_PRE_ASSEMBLER_H
#include <stdio.h>
#include "../common/collections/hash_table.h"

int is_valid(HashTable*, char*, char*);

char* analyze(char*, int*, HashTable*, char*, char*);

int gen(FILE*, FILE*);

void file_names(const char*, char**, char**);

int pre_assembler(char*);

#endif
