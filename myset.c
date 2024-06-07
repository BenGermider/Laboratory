#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "set.h"
#include "validation.h"

#define not_int "not an integer.\n"
#define bad_cmd_name "Undefined command name.\n"

char* read_command(char* command){
    int buffer = 1, txt_len = 0;
    char* temp;

    do {
        if (txt_len == buffer) {
            /* Make space for another character. */
            buffer += 1;
            temp = realloc(command, buffer);
            if (temp == NULL) {
                printf("Memory allocation failed\n");
                return command;
            }
            command = temp;
        }
        command[txt_len] = getchar();
        txt_len++;
    } while (command[txt_len - 1] != '\n' && command[txt_len - 1] != EOF);
    command[txt_len - 1] = '\0';
    return command;
}

int is_integer(const char* str) {
    char* endptr;
    strtol(str, &endptr, 10);
    while (isspace((unsigned char)*endptr)) {
        endptr++;
    }

    return (*endptr == '\0');
}

char* get_arr_as_string(char* command){
    char* comma_pos = strchr(command, ',');
    memory_allocation(comma_pos);
    char* substring = comma_pos + 2;
    return substring;
}

int calc_size(const char* arr){
    int size = 1;
    const char* temp = arr;
    while (*temp) {
        if (*temp == ',') {
            size++;
        }
        temp++;
    }
    return size;
}

int* get_arr_as_int(char* arr){
    int *nums = NULL;
    int count = 0;
    char* copy_arr = (char*)malloc(strlen(arr) + 1);
    if (copy_arr == NULL) {
        return NULL;
    }
    strcpy(copy_arr, arr);
    char *token = strtok(copy_arr, ",");
    while(token != NULL){
        if(is_integer(token)){
            int* temp = realloc(nums, (count + 1) * sizeof(int));
            if(temp == NULL){
                free(arr);
                free(copy_arr);
                return NULL;
            }
            nums = temp;
            nums[count++] = atoi(token);
        }
//        else {
//            printf(not_int);
//            free(arr);
//            free(copy_arr);
//            return NULL;
//        }
        token = strtok(NULL, ",");
    }
    free(copy_arr);
    return nums;
}

char* get_prefix_plus_substring(const char* input) {
    // Find the substring in the input string
    int i;
    int func_flag = 0;
    char* found = strstr(input, "_set");
    char* arr[] = {"read_set", "print_set", "union_set", "intersect_set", "sub_set", "symdiff_set", "stop"};
    if (!found) {
        printf(bad_cmd_name);
        return NULL; // Substring not found
    }

    // Calculate the length to copy (end of the substring within the input)
    int length_to_copy = found - input + strlen("_set");

    // Allocate memory for the result
    char* result = (char*)malloc(length_to_copy + 1); // +1 for null terminator
    if (result == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    // Copy the required portion of the string
    strncpy(result, input, length_to_copy);

    // Null-terminate the result
    result[length_to_copy] = '\0';
    for(i = 0; i < 7; i++){
        if(strcmp(result, arr[i]) == 0){
            return result;
        }
    }
    printf(bad_cmd_name);
    return NULL;
}

int main(){
    Set SETA = {0};
    Set SETB = {0};
    Set SETC = {0};
    Set SETD = {0};
    Set SETE = {0};
    Set SETF = {0};

    return 0;
}
