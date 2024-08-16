#ifndef LABORATORY_STRINGS_H
#define LABORATORY_STRINGS_H

#include <stdlib.h>

/* Checks whether the string received is legal */
int is_legal_string(char* str);

/* Get an array of integers representing each character of the string */
int* get_ascii(char* line, size_t *size);

#endif /* LABORATORY_STRINGS_H */
