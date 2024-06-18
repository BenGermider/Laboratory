#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

/**
 * Prints the occurrences of integers in file.
 * @param num - integer in file.
 * @param occur - how many times it occurs.
 * @param file_name - name of file.
 */
void print(int num, int occur, char* file_name){
    if(occur && num > 1){
        printf("%s - %d times, ", file_name, num);
    } else if (occur && num == 1){
        printf("%s - %d time, ", file_name, num);
    } else if (!occur && num > 1){
        printf("%s - %d times\n", file_name, num);
    } else {
        printf("%s - %d time\n", file_name, num);
    }
}

/**
 * Free numerous pointers at once.
 * @param first - first pointer to free.
 * @param ... - unlimited pointers to free.
 */
void free_all(void *first, ...) {
    va_list args;
    void *ptr;

    va_start(args, first);

    free(first);

    while ((ptr = va_arg(args, void *)) != NULL) {
        free(ptr);
    }

    va_end(args);
}