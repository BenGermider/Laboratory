#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../../../include/common/operands/integers.h"
#include "../../../include/common/utils.h"

/**
 * Checks whether num is legal and between bounds
 * @param num to check
 * @param bounds to be between
 * @return 1 if illegal, 0 otherwise
 */
int is_num_legal(char *num, INT_BOUND bounds, Node** errors, int line){
    char* num_copy;
    /* Calculate bounds for word */
    int min_bound = -(1 << (bounds - 1));
    int max_bound = (1 << (bounds - 1)) - 1;
    if(num == NULL){
        return 1;
    }
    num_copy = num;
    if(!isdigit(*num) && *num != '+' && *num != '-'){
        /* illegal integer prefix */
        return 1;
    }
    num_copy++;
    while(*(num_copy) != '\0'){
        if(!isdigit(*(num_copy))){
            return 1;
        }
        (num_copy)++;
    }
    if(atoi(num) < min_bound || atoi(num) > max_bound){
        /* check whether num is in bounds */
        append(errors, line, "Integer out of bounds.");
        return 1;
    }
    return 0;
}

/**
 * Converts an int into a machine code
 * @param operand int to be converted
 * @return integer represented as a machine code
 */
int integer_word(char* operand){
    int operand_as_int = atoi(operand + 1);
    if(operand_as_int < 0){
        operand_as_int &= 0xFFF;
    }
    return (operand_as_int << DESTINATION) | ABSOLUTE;
}

/**
 * Converts an array of integers, as a string into an array.
 * @param line array of integers as a string
 * @param size to count the number of integers
 * @param errors database of errors occurred
 * @param line_num line in code currently parsing
 * @return array of integers
 */
int* pull_numbers(char* line, size_t* size, Node** errors, int line_num) {
    char* sus_as_int;
    char* token_copy;
    size_t i = 0;
    size_t capacity = 10;
    char* copy;
    int* arr, *temp;
    if(line == NULL){ return NULL; }
    copy = (char*)malloc(strlen(line) + 1);
    if(!copy){ return NULL; }
    arr = (int*)malloc(capacity * sizeof(int));
    if (arr == NULL) {
        free(copy);
        return NULL;
    }

    strcpy(copy, line);
    sus_as_int = strtok(copy, ",");

    while (sus_as_int != NULL) {
        clear_side_blanks(&sus_as_int, 0);
        token_copy = sus_as_int;
        if(*token_copy == '\0' || strpbrk(sus_as_int, " \t\n\v\f\r") != NULL){
            /* Too many or no commas between ints */
            free(copy);
            free(arr);
            append(errors, line_num, "Numbers are not separated properly");
            return NULL;
        }

        if (!is_num_legal(sus_as_int, INST_BOUND, errors, line_num)) {
            if (i >= capacity) {
                /* Efficiently resize the array to hold more ints */
                capacity *= 2;
                temp = (int*)realloc(arr, capacity * sizeof(int));
                if (temp == NULL) {
                    free(copy);
                    free(arr);
                    return NULL;
                }
                arr = temp;
            }
            arr[i++] = atoi(sus_as_int);
        }
        sus_as_int = strtok(NULL, ",");
    }
    *size = i;
    free(copy);
    return arr;
}