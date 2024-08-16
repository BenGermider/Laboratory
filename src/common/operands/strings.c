#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../../include/common/operands/strings.h"
#include "../../../include/common/utils.h"

/**
 * Checks whether the string is a valid string.
 * @param str to check
 * @return 0 if valid string, 1 otherwise.
 */
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

/**
 * Converts a string into integers, when every int represents the order of the character in the ascii table
 * @param line string
 * @param size the size of the string
 * @return array of int representing the ascii value of each character
 */
int* get_ascii(char* line, size_t *size){
    int curr_size = 1, *arr, i, *temp;
    clear_side_blanks(&line, 1);
    if(*line != '\"' || *(line + strlen(line) - 1) != '\"'){
        printf("BAD STRING TYPE");
        return NULL;
    }

    arr = (int*)malloc(curr_size * sizeof(int));
    if(arr == NULL){
        printf("FAILED TO ALLOCATE MEMORY\n");
        return NULL;
    }

    for (i = 1; i < strlen(line) - 1; i++) {
        if (line[i] == '\"') {
            /*TODO: CHECK IF ITS RIGHT */
            printf("BAD STRING\n");
            free(arr);
            return NULL;
        }

        arr[curr_size - 1] = (int)line[i];
        curr_size++;

        /* resize the array */
        temp = (int*)realloc(arr, curr_size * sizeof(int));
        if (temp == NULL) {
            printf("BAD\n");
            free(arr);
            return NULL;
        }
        arr = temp;
    }

    arr[curr_size - 1] = '\0';
    *size = curr_size;

    return arr;
}