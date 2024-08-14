#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/common/utils.h"


void clear_side_blanks_remove_newline(char** line) {
    char* start, *end, *line_copy;
    size_t new_length;
    if (*line == NULL) {
        return;
    }
    start = *line;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    end = *line + strlen(*line) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }
    new_length = end - start + 1;
    line_copy = (char*)malloc(new_length + 1);
    if (line_copy == NULL) {
        printf("[ERROR] Memory allocation failed.\n");
        return;
    }
    strncpy(line_copy, start, new_length);
    line_copy[new_length] = '\0';

    *line = line_copy;
}

void clear_side_blanks(char** line){
    char* start, *end, *line_copy;
    size_t new_length;
    if (*line == NULL) {
        return;
    }

    start = *line;
    while (isspace((unsigned char)*start) && *start != '\n') {
        start++;
    }

    end = *line + strlen(*line) - 1;
    while (end > start && isspace((unsigned char)*end) && *end != '\n') {
        end--;
    }

    if (*end == '\n') {
        end++;
    }

    new_length = end - start + 1;

    line_copy = (char*)malloc(new_length + 1);
    if (line_copy == NULL) {
        printf("[ERROR] Memory allocation failed.\n");
        return;
    }

    strncpy(line_copy, start, new_length);
    line_copy[new_length] = '\0';

    *line = line_copy;
}

char* get_word(char* line){
    int len;
    char* word;
    char* new_word = strpbrk(line, ", \t\n\v\f\r");
    if(!new_word){
        return line;
    }

    len = new_word - line;
    word = malloc(len + 1);
    if(!word){
        printf("[ERROR] Failed to allocate memory");
        return NULL;
    }

    strncpy(word, line, len);
    word[len] = '\0';
    return word;
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


int is_ignorable(char *line){
    char *copy = malloc(strlen(line) + 1);
    if(!copy){
        printf("[ERROR] Failed to allocate memory...\n");
        return 0;
    }
    strcpy(copy, line);
    copy[strlen(line)] = '\0';
    clear_side_blanks(&copy);
    if(*copy == ';' || *copy == '\0'){
        free(copy);
        return 1;
    }
    free(copy);
    return 0;
}

char* get_line_copy(const char* origin_line){
    char* copy = malloc(strlen(origin_line) + 1);
    if(!copy){
        printf("[ERROR] Failed to allocate memory");
        return NULL;
    }
    strcpy(copy, origin_line);
    copy[strlen(origin_line)] = '\0';
    return copy;
}

void get_file(const char* file_name, char** input_file, const char* suffix){
    size_t name_len = strlen(file_name);
    size_t suffix_len = strlen(suffix);;
    *input_file = (char*)malloc(name_len + suffix_len + 1);
    if(*input_file == NULL){
        printf("[ERROR] Failed to allocate memory,\n");
        return;
    }
    strcpy(*input_file, file_name);
    strcat(*input_file, suffix);
}