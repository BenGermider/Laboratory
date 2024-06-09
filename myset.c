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
#define num_sep "Numbers aren't seperated properly\n"
#define bad_c "Illegal comma.\n"
#define miss_param "Missing parameter.\n"
#define bad_set_name "Undefined set name.\n"
#define invalid_input "Undefined character.\n"

char* trim_spaces(char* str) {
    char* end;

    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)
        return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end + 1) = 0;

    return str;
}

int is_valid_set_name(char* set_name){
    if(strlen(set_name) != 4){
        return 0;
    }
    if (strncmp(set_name, "SET", 3) != 0){
        return 0;
    }
    if (set_name[3] < 'A' || set_name[3] > 'F'){
        return 0;
    }
    return 1;
}

int get_set_index(char* set_name){
    return set_name[3] - 'A';
}


void remove_spaces(char* s) {
    char* d = s;
    *s++ = *d++;
    do {
        while (*d == ' ') {
            ++d;
        }
        *s++ = *d++;
    } while (*s++);
}

char* get_prefix_plus_substring(const char* input, char** func) {
    char* result;
    char* found = strstr(input, "_set");
    int i, length;
    char* arr[] = {"read_set", "print_set", "union_set", "intersect_set", "sub_set", "symdiff_set", "stop"};
    if (!found) {
        printf(bad_cmd_name);
        return NULL;
    }

    length = found - input + strlen("_set");

    result = (char*)malloc(length + 1);
    if (result == NULL) {
        perror("Failed to allocate memory");
        return NULL;
    }

    strncpy(result, input, length);

    result[length] = '\0';

    result = trim_spaces(result);

    for (i = 0; i < 7; i++) {
        if (strcmp(result, arr[i]) == 0) {
            *func = result;
            return result;
        }
    }

    printf(bad_cmd_name);
    free(result);
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

char* get_arr_as_string(char* command) {
    char* comma_pos = strchr(command, ',');
    if (comma_pos == NULL) {
        return NULL;
    }
    return comma_pos + 1;
}

int calc_size(const char *str) {
    int size = 0, on_num = 0, comma_flag = 0, space_flag = 0;
    while (*str) {
        if (isdigit(*str) || (*str == '-' && isdigit(*(str + 1)))) {
            if (!on_num) {
                on_num = 1;
                size++;
            }
            comma_flag = 0;
        } else if (*str == ',') {
            if (on_num) {
                on_num = 0;
            } else if (comma_flag) {
                printf(cons_c);
                return -1;
            }
            comma_flag = 1;
            space_flag = 0;
        } else if (*str == ' ') {
            if (on_num) {
                on_num = 0;
            }
            if (!comma_flag) {
                space_flag = 1;
            }
        } else {
            printf(bad_set_member);
            printf(not_int);
            return -1;
        }
        str++;
    }

    if (space_flag) {
        printf(num_sep);
        return -1;
    }

    return size;
}


int* get_arr_as_int(char* arr, int* size) {
    int *nums = NULL;
    int bound_test;
    int count = 0;
    char *copy_arr, *token;
    *size = calc_size(arr);
    if(*size == -1){
        return NULL;
    }
    copy_arr = (char*)malloc(strlen(arr) + 1);
    if (copy_arr == NULL) {
        return NULL;
    }

    strcpy(copy_arr, arr);
    token = strtok(copy_arr, ",");
    while (token != NULL) {
        token = trim_spaces(token);
        if (is_integer(token)) {
            int* temp = realloc(nums, (count + 1) * sizeof(int));
            if (temp == NULL) {
                free(nums);
                free(copy_arr);
                return NULL;
            }
            nums = temp;
            bound_test = atoi(token);
            if ((bound_test < 0 || bound_test > 127) && (*size - 1) != count) {
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

int* get_sets(char* command, const char* func, int* size) {
    char* cmd_copy;
    int* sets;
    char set_name[5];
    int set_count, len;

    cmd_copy = command;
    while(*cmd_copy && isspace(*cmd_copy)){
        cmd_copy++;
    }
    cmd_copy += strlen(func);

    while(*cmd_copy != 'S' && *cmd_copy){
        if(*cmd_copy == ','){
            printf(bad_c);
            return NULL;
        } else if (*cmd_copy == '-' || *cmd_copy == '_'){
            printf("Illegal character.");
            return NULL;
        }
        cmd_copy++;
    }

    if(strcmp(func, "print_set") == 0 || strcmp(func, "read_set") == 0){
        *size = 1;
    } else {
        *size = 3;
    }

    sets = (int*)malloc(sizeof(int) * (*size));
    if(!sets){
        printf("Memory allocation failed.\n");
        return NULL;
    }

    set_count = 0;
    while(*cmd_copy && set_count < *size){
        while(*cmd_copy && isspace(*cmd_copy)){
            cmd_copy++;
        }
        len = 0;
        while(*cmd_copy && !isspace(*cmd_copy) && *cmd_copy != ',' && len < 5){
            set_name[len++] = *cmd_copy++;
        }
        set_name[len] = '\0';
        if(!is_valid_set_name(set_name)){
            printf(bad_set_name);
            free(sets);
            return NULL;
        }
        if(set_count < *size){
            sets[set_count++] = get_set_index(set_name);
        } else {
            printf(ext_txt);
            free(sets);
            return NULL;
        }

        while(*cmd_copy && isspace(*cmd_copy)){
            cmd_copy++;
        }

        if(*cmd_copy == ','){
            cmd_copy++;
            while(*cmd_copy && isspace(*cmd_copy)){
                cmd_copy++;
            }

            if(strcmp(func, "print_set") == 0 && *cmd_copy){
                printf(bad_c);
                free(sets);
                return NULL;
            }

        } else if (set_count < *size && *cmd_copy != '\0'){
            printf(miss_param);
            free(sets);
            return NULL;
        }
    }

    return sets;
}

char* read_command(char* command, char** func) {
    int buffer = 1, txt_len = 0, found_comma = 0;
    int i;
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

        if(command[txt_len] == ','){
            found_comma = 1;
        }

        txt_len++;

    } while (command[txt_len - 1] != '\n' && command[txt_len - 1] != EOF);
    command[txt_len - 1] = '\0';


    printf("[USER INPUT] %s\n", command);

    for(i = 0; command[i] != '\0' ; i++){
        if(!(isalnum(command[i]) || isspace(command[i]) || command[i] == ',' || command[i] == '_' || command[i] == '-')) {
            printf("%c\n", command[i]);
            printf(invalid_input);
            free(command);
            return NULL;
        }
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

    for(i = 0; i < txt_len; i++){
        if(i > 0){
            if(command[i] == ',' && command[i - 1] == ','){
                printf(cons_c);
                free(command);
                return NULL;
            }
        }
    }

    if (!(isupper(command[txt_len - 2]) || isdigit(command[txt_len - 2]) || isspace(command[txt_len -2]))) {
        printf(ext_txt);
        free(command);
        return NULL;
    }


    return command;
}

void analyze_command(char* command, const char* func, const int* set_pointers, Set sets[]) {
    char* array_as_string = NULL;
    int* nums = NULL;
    int arr_size;
    if (strcmp(func, "print_set") == 0) {
        print_set(&sets[set_pointers[0]]);
    } else if (strcmp(func, "read_set") == 0) {
        array_as_string = get_arr_as_string(command);
        nums = get_arr_as_int(array_as_string, &arr_size);
        read_set(&sets[set_pointers[0]], nums, arr_size);
        free(nums);
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


int main() {
    Set set_arr[6];
    char* func = NULL;
    char* command = NULL;
    int *sets;
    int size;
    int i = 1;
    command = (char*)malloc(1 * sizeof(char));
    if (command == NULL) {
        printf("Failed to allocate memory");
        return 1;
    }
    memset(set_arr, 0, 6 * sizeof(Set));

    do {
        printf("Enter the desired command:\n");
        command = read_command(command, &func);
        sets = get_sets(command, func, &size);
        if(command == NULL || sets == NULL){
            free(sets);
            free(command);
            return 1;
        }

        analyze_command(command, func, sets, set_arr);
        printf("Finished command no. %d\n", i++);
        free(sets);
    } while (strcmp(command, "stop") != 0);
    free(command);


    return 0;
}
