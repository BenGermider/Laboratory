#ifndef LABORATORY_PRE_ASSEMBLER_H
#define LABORATORY_PRE_ASSEMBLER_H
#include <stdio.h>
#include "../common/collections/hash_table.h"
#include "../common/collections/linked_list.h"

/* Checks whether macro name declaration is valid */
int is_valid(HashTable*, char*, char*);

/* Searches for macro content in the file and saves it in the collection of macros. */
char* analyze(char*, int*, HashTable*, char*, char*, Node**, int);

/* Reads a .as file and re-organizes it without macro declarations and calls */
int order_as_file(FILE*, FILE*, Node**, HashTable*);

/* Terminates the pre-assembler process safely. */
int terminate(FILE*, FILE*, char*, char*, int);

/* Main pre-assembler function which exchanges macro into a code */
int pre_assembler(char*, HashTable*);

#endif
