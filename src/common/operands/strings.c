#include <string.h>
#include <stdlib.h>

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
int* get_ascii(char* line, size_t *size, int line_num, Node** errors){
    int curr_size = 1, *arr, i, *temp;
    if(line == NULL){ return NULL; }
    clear_side_blanks(&line, 0);
    if(is_legal_string(line)){
        append(errors, line_num, "Bad string format.");
        return NULL;
    }

    arr = (int*)malloc(curr_size * sizeof(int));
    if(arr == NULL){
        return NULL;
    }

    for (i = 1; i < strlen(line) - 1; i++) {
        arr[curr_size - 1] = (int)line[i];
        curr_size++;

        /* resize the array */
        temp = (int*)realloc(arr, curr_size * sizeof(int));
        if (temp == NULL) {
            free(arr);
            return NULL;
        }
        arr = temp;
    }

    arr[curr_size - 1] = '\0';
    *size = curr_size;

    return arr;
}