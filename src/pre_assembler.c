#include <stdio.h>
#include "../include/pre_assembler.h"


int run(char* name_of_file){
    FILE *file_to_scan = fopen(name_of_file, "r+");
    if(!file_to_scan){
        printf("[ERROR] Failed to open file %s\n", name_of_file);
        return 1;
    }

    return 0;
}