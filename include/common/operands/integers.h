#ifndef LABORATORY_INTEGERS_H
#define LABORATORY_INTEGERS_H

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../data_types.h"
#include "../collections/linked_list.h"


/* Checks whether the number received is legal according to the protocol */
int is_num_legal(char *num, INT_BOUND, Node**, int);

/* Converts an integer into a code */
int integer_word(char* operand);

/* Converts an array as a string into integers */
int* pull_numbers(char* line, size_t* size, Node**, int);

#endif /* LABORATORY_INTEGERS_H */
