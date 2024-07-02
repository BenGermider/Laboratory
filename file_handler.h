#include <stdlib.h>

#ifndef LABORATORY_FILE_HANDLER_H
#define LABORATORY_FILE_HANDLER_H

/**
 * Returns a file in char* format.
 * @param file_name name of the file to analyze.
 * @return file as a string.
 */
char* read_file(char* file_name);

/**
 * Convert the file as a string to an array of integers.
 * @param file_name name of file to get the integers from.
 * @param size pointer to save the amount of numbers.
 * @return an array of integers in the file.
 */
int* get_nums(char* file_name, size_t* size);

#endif /*LABORATORY_FILE_HANDLER_H */
