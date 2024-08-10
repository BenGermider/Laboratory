#ifndef LABORATORY_INTEGERS_H
#define LABORATORY_INTEGERS_H

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int is_num_legal(char *num);

int integer_word(char* operand);

int* pull_numbers(char* line, size_t* size);

#endif /* LABORATORY_INTEGERS_H */
