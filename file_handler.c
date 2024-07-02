#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "file_handler.h"
#include "utils.h"

/**
 * Returns a file in char* format.
 * @param file_name name of the file to analyze.
 * @return file as a string.
 */
char* read_file(char* file_name) {
    FILE *file;
    char *file_as_str, *tmp_bfr;
    int input;
    size_t bfr_size = 100, str_len = 0;

    /* Open the file, handle error */
    file = fopen(file_name, "r");
    if (file == NULL) {
        fprintf(stderr, "[ERROR] Failed to open file, terminating...\n");
        exit(1);
    }

    /* Create the string var, handle error */
    file_as_str = (char*)malloc(bfr_size * sizeof(char));
    if (file_as_str == NULL) {
        fprintf(stderr, "[ERROR] Failed to allocate memory, terminating...\n");
        fclose(file);
        exit(1);
    }

    /* Read characters from file and put them in the string var */
    while ((input = fgetc(file)) != EOF) {
        file_as_str[str_len] = (char)input;
        str_len++;

        if (str_len == bfr_size) {
            /* If we reached the amount of memory allocated, gain some more space */
            bfr_size *= 2;
            tmp_bfr = (char*)realloc(file_as_str, bfr_size * sizeof(char));
            if (tmp_bfr == NULL) {
                fprintf(stderr, "[ERROR] Failed to reallocate memory, terminating...\n");
                free(file_as_str);
                fclose(file);
                exit(1);
            }
            file_as_str = tmp_bfr;
        }
    }

    /* Close the string and the file */
    file_as_str[str_len] = '\0';
    fclose(file);

    return file_as_str;
}

/**
 * Convert the file as a string to an array of integers.
 * @param file_name name of file to get the integers from.
 * @param size pointer to save the amount of numbers.
 * @return an array of integers in the file.
 */
int* get_nums(char* file_name, size_t* size){
    int *arr = NULL;
    size_t numbers = 0;
    size_t num_len;
    char *content, *copy, *num_holder, *start;

    /* Get the file as a string */
    content = read_file(file_name);
    copy = content;

    /* Put numbers in the array like in the sliding window algorithm. */
    while (*copy != '\0') {
        while (isspace(*copy)) {
            /* Ignore blanks */
            copy++;
        }
        start = copy;

        /* Read the number */
        while (isdigit(*copy)) {
            copy++;
        }

        num_len = copy - start;
        num_holder = (char*)malloc((num_len + 1) * sizeof(char));
        /* Get the space and save the number as a string */
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

        /* Add the number in to the numbers array */
        arr[numbers] = atoi(num_holder);
        free(num_holder);
        numbers++;
    }

    /* Save the size of the array in the pointer sent to the function */
    if(numbers){
        *size = numbers;
    }
    free(content);
    return arr;
}
