#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

void clear_side_blanks(char** line){
    char *line_copy;
    char* start;
    char *end;
    size_t new_length;
    if (*line == NULL) {
        return;
    }

    start = *line;
    while (isspace((unsigned char)*start) && *start != '\n') {
        start++;
    }

    end = *line + strlen(*line) - 1;
    while (end > start && isspace((unsigned char)*end) && *end != '\n') {
        end--;
    }

    if (*end == '\n') {
        end++;
    }

    new_length = end - start + 1;

    line_copy = (char*)malloc(new_length + 1);
    if (line_copy == NULL) {
        printf("[ERROR] Memory allocation failed.\n");
        return;
    }

    strncpy(line_copy, start, new_length);
    line_copy[new_length] = '\0';

    *line = line_copy;
}

int* get_ascii(char* line, size_t *size){
    int curr_size = 1, *arr, i, *temp;
    clear_side_blanks(&line);
    if(*line != '\"' || *(line + strlen(line) - 1) != '\"'){
        printf("AJAJABAD");
        return NULL;
    }

    arr = (int*)malloc(curr_size * sizeof(int));
    if(arr == NULL){
        printf("BAD");
        return NULL;
    }

    for (i = 1; i < strlen(line) - 1; i++) {
        if (line[i] == '\"') {
            printf("BADASD\n");
            free(arr);
            return NULL;
        }

        arr[curr_size - 1] = (int)line[i];
        curr_size++;

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

int main() {
    size_t size, i;
    char x[] = "\"zif zif zif zif zif\"";
    int* arr = get_ascii(x, &size);
    if (arr == NULL) {
        printf("DEAD\n");
        return 1;
    }
    for (i = 0; i < size; i++) {
        printf("%c\n", (char)arr[i]);
        printf("%d\n", arr[i]);
    }
    free(arr);
    return 0;
}