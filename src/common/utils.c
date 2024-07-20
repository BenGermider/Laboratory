#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/common/utils.h"


void clear_side_blanks(char** line) {
    if (*line == NULL) {
        return;
    }

    char *start = *line;
    while (isspace((unsigned char)*start) && *start != '\n') {
        start++;
    }

    char *end = *line + strlen(*line) - 1;
    while (end > start && isspace((unsigned char)*end) && *end != '\n') {
        end--;
    }

    if (*end == '\n') {
        end++;
    }

    size_t new_length = end - start + 1;

    char *line_copy = (char*)malloc(new_length + 1);
    if (line_copy == NULL) {
        printf("[ERROR] Memory allocation failed.\n");
        return;
    }

    strncpy(line_copy, start, new_length);
    line_copy[new_length] = '\0';

    *line = line_copy;
}

char* spaces_and_strings(char* line){

}

void free_space(int amount, ...) {
    int i;
    va_list args;
    va_start(args, amount);

    for (i = 0; i < amount; i++) {
        void* ptr = va_arg(args, void*);
        if (ptr != NULL) {
            free(ptr);
        }
    }

    va_end(args);
}