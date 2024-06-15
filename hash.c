#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

int* read_integers_from_file(char* filename, size_t* count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("FILE SIZE: %ld \n", file_size);
    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Unable to allocate buffer");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);
    printf("BUFFER: %s\n", buffer);
    *count = 0;
    char* ptr = buffer;
    while (*ptr) {
        if (*ptr == ' ' || *ptr == '\n') {
            ptr++;
            continue;
        }
        (*count)++;
        while (*ptr && *ptr != ' ' && *ptr != '\n') {
            ptr++;
        }
    }
    printf("COUNT: %zu\n", *count);
    int* integers = (int*)malloc(*count * sizeof(int));
    if (integers == NULL) {
        perror("Unable to allocate memory for integers");
        free(buffer);
        return NULL;
    }

    size_t index = 0;
    ptr = strtok(buffer, " \n");
    while (ptr != NULL) {
        integers[index++] = (int)strtoul(ptr, NULL, 10);
        ptr = strtok(NULL, " \n");
    }

    free(buffer);
    return integers;
}



int main(int argc, char* argv[]) {
    int i, j;
    int *integers;
    size_t count;
    if (argc <= 1) {
        printf("NOT ENOUGH ARGUMENTS\n");
        return 1;
    }
    read_files(argc, argv);
}