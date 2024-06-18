#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "utils.h"

char* read_file(char* file_name){
    FILE *file;
    char *buffer;
    long file_size;
    size_t read_size;

    file = fopen(file_name, "r");
    if(file == NULL){
        fprintf(stderr, "[ERROR] Failed to open file, terminating...\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    buffer = (char*)malloc(sizeof(char) * (file_size + 1));
    if(buffer == NULL){
        fprintf(stderr, "[ERROR] Failed to allocate memory, terminating...\n");
        fclose(file);
        exit(1);
    }

    read_size = fread(buffer, sizeof(char), file_size, file);
    if (read_size != file_size) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        exit(1);
    }

    buffer[file_size] = '\0';
    return buffer;
}

int* get_nums(char* file_name, size_t* size){
    int *arr = NULL;
    size_t numbers = 0;
    size_t num_len;
    char *content, *copy, *num_holder, *start;

    content = read_file(file_name);

    copy = content;

    while (*copy != '\0') {
        while (isspace(*copy)) {
            copy++;
        }

        if (!isdigit(*copy)) {
            copy++;
            continue;
        }

        start = copy;

        while (isdigit(*copy)) {
            copy++;
        }

        num_len = copy - start;
        num_holder = (char*)malloc((num_len + 1) * sizeof(char));
        if (!num_holder) {
            fprintf(stderr, "[ERROR] Failed to allocate memory, terminating...\n");
            free_all(content, arr);
            return NULL;
        }

        strncpy(num_holder, start, num_len);
        num_holder[num_len] = '\0';

        arr = (int*)realloc(arr, sizeof(int) * (numbers + 1));
        if (!arr) {
            fprintf(stderr, "[ERROR] Failed to allocate memory, terminating...\n");
            free_all(content, num_holder);
            return NULL;
        }

        arr[numbers] = atoi(num_holder);
        free(num_holder);
        numbers++;
    }
    if(numbers){
        *size = numbers;
    }
    free(content);
    return arr;
}
