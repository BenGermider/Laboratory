#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "linked_list.h"
#include "hash_table.h"

void initiate_hash_table(HashTable *hash){
    int i;
    for(i = 0; i < 29; i++){
        hash->lists[i] = NULL;
    }
}

void insert_to_hash_table(int num, char* file_name, HashTable *hash){
    add_to_tail(&hash->lists[num], num, file_name);
}

void read_integers_from_file(char* filename, size_t* count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        perror("Unable to allocate buffer");
        fclose(file);
        exit(1);
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';
    fclose(file);

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

    int* integers = (int*)malloc(*count * sizeof(int));
    if (integers == NULL) {
        perror("Unable to allocate memory for integers");
        free(buffer);
        exit(1);
    }

    size_t index = 0;
    ptr = strtok(buffer, " \n");
    while (ptr != NULL) {
        insert_to_hash_table((int)strtoul(ptr, NULL, 10), filename, hash_table);
        ptr = strtok(NULL, " \n");
    }

    free(buffer);
}

void read_files(int argc, char* argv[]){
    int i;
    size_t size;
    for(i = 1; i < argc ; i++){
        read_integers_from_file(argv[i], &size);
    }
}

