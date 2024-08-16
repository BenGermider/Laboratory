#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../../../include/common/operands/integers.h"
#include "../../../include/common/utils.h"

/**
 * Implementation of power but for int
 * @param base
 * @param exp
 * @return
 */
int power(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}

/**
 * Checks whether num is legal and between bounds
 * @param num to check
 * @param bounds to be between
 * @return 1 if illegal, 0 otherwise
 */
int is_num_legal(char *num, INT_BOUND bounds){
    char* num_copy;
    if(num == NULL){
        return 1;
    }
    num_copy = num;
    if(!isdigit(*num) || *num != '+' || *num != '-'){
        return 1;
    }
    num_copy++;
    while(*(num_copy) != '\0'){
        if(!isdigit(*(num_copy))){
            return 1;
        }
        (num_copy)++;
    }
    if(atoi(num) < power(-2, (bounds - 1)) || atoi(num) > (power(2, bounds - 1) - 1)){
        /* check whether num is in bounds */
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
int* pull_numbers(char* line, size_t* size) {
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
        clear_side_blanks(&token, 1);
        token_copy = token;
        while (*token_copy != '\0') {
            if (isspace(*token_copy)) {
                free(copy);
                free(arr);
                printf("[ERROR] Numbers are not separated properly\n");
                return NULL;
            }
            token_copy++;
        }
        if (is_num_legal(token, INST_BOUND)) {
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