#include <stdarg.h>

#ifndef LABORATORY_UTILS_H
#define LABORATORY_UTILS_H

/**
 * Prints the occurrences of integers in file.
 * @param num - integer in file.
 * @param occur - how many times it occurs.
 * @param file_name - name of file.
 */
void print(int num, int occur, char* file_name);

/**
 * Free numerous pointers at once.
 * @param first - first pointer to free.
 * @param ... - unlimited pointers to free.
 */
void free_all(void *first, ...);

#endif
