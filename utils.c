#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

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