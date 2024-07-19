#ifndef LABORATORY_PRE_ASSEMBLER_H
#define LABORATORY_PRE_ASSEMBLER_H
#include <stdio.h>
#include "common/hash_table.h"

int is_valid(char*, char*);

char* analyze(char* line, int* in_macro, HashTable*, char*, char*);

FILE* generate_file(FILE*, FILE*);

int pre_assembler(char* name_of_file);
#endif
