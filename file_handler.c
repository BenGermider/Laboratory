#include <stdio.h>
#include <stdlib.h>

char* read_file(char* file_name){
    FILE *file;
    char *buffer;
    long file_size;
    size_t read_size;

    file = fopen(file_name, "r");
    if(file == NULL){
        printf("[ERROR] Failed to open file, terminating...\n");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    rewind(file);

    buffer = (char*)malloc(sizeof(char) * (file_size + 1));
    if(buffer == NULL){
        printf("[ERROR] Failed to allocate memory, terminating...\n");
        fclose(file);
        exit(1);
    }

    read_size = fread(buffer, sizeof(char), file_size, file);
    if (read_size != file_size) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        exit(1);
    }

    buffer[file_size] = '\0';

    return buffer;
}
