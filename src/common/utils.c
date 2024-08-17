#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/common/utils.h"

/**
 * Remove all blanks from the side including newline
 * @param line to clear the edges
 */
/**
 * Remove all blanks from the side, optionally including newline
 * @param line to clear the edges
 * @param remove_newline flag to determine whether to remove newline
 */
void clear_side_blanks(char** line, int remove_newline) {
    char* start, *end, *line_copy;
    size_t new_length;
    if (*line == NULL) {
        return;
    }
    start = *line;
    while (isspace((unsigned char)*start) && (!remove_newline || *start != '\n')) {
        /* Clear left edge from blanks */
        start++;
    }

    end = *line + strlen(*line) - 1;
    while (end > start && isspace((unsigned char)*end) && (!remove_newline || *end != '\n')) {
        /* Clear right edge from blanks */
        end--;
    }

    if (!remove_newline && *end == '\n') {
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

/**
 * Get the substring up to the first space or comma
 * @param line to pull the substring from
 * @return substring without commas/blanks
 */
char* get_word(char* line){
    int len;
    char* word;
    char* new_word = strpbrk(line, ", \t\n\v\f\r");
    /* If the string itself is a word */
    if(!new_word){
        return line;
    }

    len = new_word - line;
    word = (char*)malloc(len + 1);
    if(!word){
        printf("[ERROR] Failed to allocate memory");
        return NULL;
    }

    /* Save new substring */
    strncpy(word, line, len);
    word[len] = '\0';
    return word;
}

/**
 * Frees multiple dynamically allocated object at once
 * @param amount of objects to be freed
 * @param ...
 */
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


/**
 * Checks whether the line is ignorable, contains no data
 * @param line to check
 * @return 1 if ignorable, 0 otherwise
 */
int is_ignorable(char *line){
    char *copy = malloc(strlen(line) + 1);
    if(!copy){
        printf("[ERROR] Failed to allocate memory...\n");
        return 0;
    }
    /* makes a copy of the line to check */
    strcpy(copy, line);
    copy[strlen(line)] = '\0';
    if(*copy == ';'){
        free(copy);
        return 1;
    }
    clear_side_blanks(&copy, 0);
    /* if line is a comment or empty, it is ignorable */
    if(*copy == '\0'){
        free(copy);
        return 1;
    }
    free(copy);
    return 0;
}

/**
 * Makes a copy of the line and returns it
 * @param origin_line
 * @return copy
 */
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

/**
 * Concatenates the name of the file with a desired suffix
 * @param file_name
 * @param input_file output of the concatenation
 * @param suffix of the file
 */
void get_file(const char* file_name, char** input_file, const char* suffix){
    size_t name_len = strlen(file_name);
    size_t suffix_len = strlen(suffix);
    *input_file = (char*)malloc(name_len + suffix_len + 1);
    if(*input_file == NULL){
        printf("[ERROR] Failed to allocate memory,\n");
        return;
    }
    strcpy(*input_file, file_name);
    strcat(*input_file, suffix);
}