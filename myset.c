#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "set.h"

#define bad_set_member "Invalid set member - "
#define range "value out of range.\n"
#define not_int "not an integer.\n"
#define bad_cmd_name "Undefined command name.\n"
#define bad_set_ending "List of set members is not terminated correctly.\n"
#define ext_txt "Extraneous text after end of command.\n"
#define cons_c "Multiple consecutive commas.\n"
#define missing_c "Missing comma.\n"

// Function to get prefix and substring
char* get_prefix_plus_substring(const char* input, char** func) {
    // Find the substring in the input string
    char* found = strstr(input, "_set");
    int i;
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

    // Compare the result with the array of valid commands
    for (i = 0; i < 7; i++) {
        if (strcmp(result, arr[i]) == 0) {
            *func = result;
            return result;
        }
    }

    printf(bad_cmd_name);
    free(result); // Free allocated memory before returning NULL
    return NULL;
}

int is_integer(const char* str) {
    char* endptr;
    strtol(str, &endptr, 10);
    while (isspace((unsigned char)*endptr)) {
        endptr++;
    }
    return (*endptr == '\0');
}

// Function to extract numbers from command
char* get_arr_as_string(char* command) {
    char* comma_pos = strchr(command, ',');
    if (comma_pos == NULL) {
        return NULL;
    }
    return comma_pos + 2;
}

// Function to calculate the size of an array of numbers
int calc_size(const char* arr) {
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

int* get_arr_as_int(char* arr) {
    int *nums = NULL;
    int bound_test;
    int count = 0;
    int size = calc_size(arr);
    char* copy_arr = (char*)malloc(strlen(arr) + 1);
    if (copy_arr == NULL) {
        return NULL;
    }
    strcpy(copy_arr, arr);
    char *token = strtok(copy_arr, ",");
    while (token != NULL) {
        if (is_integer(token)) {
            int* temp = realloc(nums, (count + 1) * sizeof(int));
            if (temp == NULL) {
                free(nums);
                free(copy_arr);
                return NULL;
            }
            nums = temp;
            bound_test = atoi(token);
            if ((bound_test < 0 || bound_test > 127) && (size - 1) != count) {
                printf(bad_set_member);
                printf(range);
                free(nums);
                free(copy_arr);
                return NULL;
            }
            nums[count++] = bound_test;
        } else {
            printf(bad_set_member);
            printf(not_int);
            free(nums);
            free(copy_arr);
            return NULL;
        }
        token = strtok(NULL, ",");
    }
    if (nums[count - 1] != -1) {
        printf(bad_set_ending);
        free(nums);
        free(copy_arr);
        return NULL;
    }

    free(copy_arr);
    return nums;
}

int* get_sets(const char* command, const char* func, int* size) {
    const char* set_prefix = "SET";
    const char* delimiter = ", ";
    char* set_type[6] = {"SETA", "SETB", "SETC", "SETD", "SETE", "SETF"};
    int* sets = NULL;
    int num_sets = 0;
    int i, j;

    // Find the first occurrence of "SET" in the command
    const char* found = strstr(command, set_prefix);
    if (!found) {
        printf("Error: No valid SET found.\n");
        return NULL;
    }

    // Check if the function is "print_set" or "read_set"
    if (strcmp(func, "print_set") == 0 || strcmp(func, "read_set") == 0) {
        *size = 1;
    } else {
        *size = 3;
    }

    // Allocate memory for the sets
    sets = (int*)malloc((*size) * sizeof(int));
    if (!sets) {
        printf("Error: Memory allocation failed.\n");
        free(sets);
        return NULL;
    }

    for (i = 0; i < (*size); i++) {
        // Check if the next set exists
        if (!found) {
            printf("Error: Insufficient number of sets.\n");
            free(sets);
            return NULL;
        }

        // Extract the set name (e.g., SETA, SETB, etc.)
        char set_name[5]; // Assuming maximum length of 3 (SETX)
        strncpy(set_name, found, 4);
        set_name[4] = '\0'; // Null-terminate the string

        // Validate the set name
        int valid_set = 0;
        for (j = 0; j < 6; j++) {
            if (strcmp(set_name, set_type[j]) == 0) {
                valid_set = 1;
                sets[i] = j;
                break;
            }
        }

        if (!valid_set) {
            printf("Error: Invalid set name: %s\n", set_name);
            free(sets);
            return NULL;
        }

        // Move to the next set (if any)
        found = strstr(found + 1, delimiter);
        if (found) {
            found += strlen(delimiter); // Skip the delimiter
        }
    }

    return sets;
}

// Function to read command from input
char* read_command(char* command, char** func) {
    int buffer = 1, txt_len = 0, found_comma = 0;
    char* temp;
    char* test;

    do {
        if (txt_len == buffer) {
            buffer += 1;
            temp = realloc(command, buffer);
            if (temp == NULL) {
                printf("Memory allocation failed\n");
                free(command);
                return NULL;
            }
            command = temp;
        }
        command[txt_len] = getchar();
        if (command[txt_len] == ',') {
            found_comma = 1;
            if (txt_len > 0 && command[txt_len - 1] == ',') {
                printf(cons_c);
                free(command);
                return NULL;
            }
        }
        txt_len++;
    } while (command[txt_len - 1] != '\n' && command[txt_len - 1] != EOF);
    command[txt_len - 1] = '\0';

    if (!(isupper(command[txt_len - 2]) || isdigit(command[txt_len - 2]))) {
        printf(ext_txt);
        free(command);
        return NULL;
    }

    test = get_prefix_plus_substring(command, func);
    if (test == NULL) {
        free(command);
        return NULL;
    }

    if (!found_comma && strcmp(test, "print_set") != 0) {
        printf(missing_c);
        free(command);
        return NULL;
    }

    return command;
}

void analyze_command(char* command, const char* func, const int* set_pointers, Set sets[]) {
    if (strcmp(func, "print_set") == 0) {
        print_set(&sets[set_pointers[0]]);
    } else if (strcmp(func, "read_set") == 0) {
        char* array_as_string = get_arr_as_string(command);
        int* nums = get_arr_as_int(array_as_string);
        int arr_size = calc_size(array_as_string);
        read_set(&sets[set_pointers[0]], nums, arr_size);
        free(nums);
        free(array_as_string);
    } else if (strcmp(func, "union_set") == 0) {
        union_set(&sets[set_pointers[0]], &sets[set_pointers[1]], &sets[set_pointers[2]]);
    } else if (strcmp(func, "intersect_set") == 0) {
        intersect_set(&sets[set_pointers[0]], &sets[set_pointers[1]], &sets[set_pointers[2]]);
    } else if (strcmp(func, "sub_set") == 0) {
        sub_set(&sets[set_pointers[0]], &sets[set_pointers[1]], &sets[set_pointers[2]]);
    } else if (strcmp(func, "symdiff_set") == 0) {
        symdiff_set(&sets[set_pointers[0]], &sets[set_pointers[1]], &sets[set_pointers[2]]);
    } else if (strcmp(func, "stop") == 0) {
        stop();
    } else {
        printf("Unknown command: %s\n", func);
    }
}

// Function to check if a string represents an integer


// Function to convert array of numbers in string format to int array


int main() {
    Set set_arr[6];
    char* func = NULL;
    int i;
    int *sets;
    int size;
    char* command = (char*)malloc(sizeof(char));
    if (command == NULL) {
        printf("Failed to allocate memory");
        return 1;
    }
    memset(set_arr, 0, 6 * sizeof(Set));

    do {
        command = read_command(command, &func);
        sets = get_sets(command, func, &size);
        if(command == NULL || sets == NULL){
            free(sets);
            free(command);
            return 1;
        }

        analyze_command(command, func, sets, set_arr);
        free(sets);
    } while (strcmp(command, "stop") == 0);
    free(command);


    return 0;
}
