#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/common/utils.h"


void clear_side_blanks_remove_newline(char** line) {
    char* start;
    char* end;
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

    char* line_copy = (char*)malloc(new_length + 1);
    if (line_copy == NULL) {
        printf("[ERROR] Memory allocation failed.\n");
        return;
    }

    strncpy(line_copy, start, new_length);
    line_copy[new_length] = '\0';

    *line = line_copy;
}

void clear_side_blanks(char** line){
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

int is_num_legal(char *num){
    if(num == NULL){
        return 0;
    }
    if(!isdigit(*num) || *num != '+' || *num != '-'){
        return 1;
    }
    num++;
    while(*num != '\0'){
        if(!isdigit(*num)){
            return 1;
        }
        num++;
    }
    return 0;
}

int is_legal_string(char* str){
    char *end;
    if (*str != '"') {
        return 1;
    }
    str++;
    end = strchr(str, '"');
    if (end == NULL) {
        return 1;
    }
    if (*(end + 1) != '\0') {
        return 1;
    }
    return 0;
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

