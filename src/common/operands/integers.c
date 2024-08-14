#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../../../include/common/operands/integers.h"
#include "../../../include/common/utils.h"
#include "../../../include/common/data_types.h"

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
    if(atoi(num) < pow(-2, (bounds - 1)) || atoi(num) > (pow(2, bounds - 1) - 1)){
        return 1;
    }
    return 0;
}

int integer_word(char* operand){
    int operand_as_int = atoi(operand + 1);
    if(operand_as_int < 0){
        operand_as_int &= 0xFFF;
    }
    return (operand_as_int << DESTINATION) | ABSOLUTE;
}

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
        clear_side_blanks_remove_newline(&token);
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
        if (is_num_legal(token)) {
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