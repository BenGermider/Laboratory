#ifndef LABORATORY_PRE_ASSEMBLER_H
#define LABORATORY_PRE_ASSEMBLER_H
#include <stdio.h>
#include "common/hash_table.h"

char* is_valid(char* macro_name);

char* handle_line(char* line, int* in_macro, HashTable*);

FILE* analyze(FILE*, FILE*);

int pre_assembler(char* name_of_file);
#endif
