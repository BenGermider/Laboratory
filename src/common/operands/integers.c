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
    int min_bound = -(1 << (bounds - 1));
    int max_bound = (1 << (bounds - 1)) - 1;
    if(num == NULL){
        return 1;
    }
    num_copy = num;
    if(!isdigit(*num) && *num != '+' && *num != '-'){
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
 * @return array of integers
 */
 /* TODO: REFACTOR */
int* pull_numbers(char* line, size_t* size, Node** errors, int line_num) {
    char* token;
    char* token_copy;
    size_t i = 0;
    size_t capacity = 10;
    char* copy;
    int* arr, *final_arr, *temp;

    copy = (char*)malloc(strlen(line) + 1);
    arr = (int*)malloc(capacity * sizeof(int));
    if (copy == NULL || arr == NULL) {
        printf("[ERROR] Failed to allocate memory\n");
        free(copy);
        free(arr);
        return NULL;
    }
    strcpy(copy, line);
    token = strtok(copy, ",");

    while (token != NULL) {
        clear_side_blanks(&token, 0);
        token_copy = token;
        while (*token_copy != '\0') {
            if (isspace(*token_copy)) {
                free(copy);
                free(arr);
                append(errors, line_num, "Numbers are not separated properly");
                return NULL;
            }
            token_copy++;
        }
        if (!is_num_legal(token, INST_BOUND, errors, line_num)) {
            if (i >= capacity) {
                capacity *= 2;
                temp = (int*)realloc(arr, capacity * sizeof(int));
                if (temp == NULL) {
                    printf("[ERROR] Failed to allocate memory\n");
                    free(copy);
                    free(arr);
                    return NULL;
                }
                arr = temp;
            }
            arr[i++] = atoi(token);
        }
        token = strtok(NULL, ",");
    }

    *size = i;
    free(copy);

    final_arr = (int*)realloc(arr, i * sizeof(int));
    return final_arr ? final_arr : arr;
}