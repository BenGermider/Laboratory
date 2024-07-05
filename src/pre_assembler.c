#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/pre_assembler.h"
#include "../include/common/hash_table.h"
#include "../include/common/utils.h"
#include "../include/common/consts.h"

char* is_valid(char* macro_name){
    char* copy = macro_name;
    char* end;
    while(isspace(*copy)) copy++;
    

}

char* handle_line(char* line, int* in_macro, HashTable* ht){
    char *macro_name, *macro_content;
    clear_side_blanks(&line);
    if((macro_content = get(ht, line)) != NULL){
        return macro_content;
    }
    else if(strncmp(line, MACRO_START, 4) == 0 && !(*in_macro)){
        *(in_macro) = 1;
        macro_name = is_valid(line + 4);
        if(!macro_name){
            return NULL;
        }
    }
}

FILE* analyze(FILE *input_file, FILE *output_file){
    HashTable *ht = createHashTable();
    int in_macro = 0;
    char* c;
    char* line = (char*)malloc(MAX_LINE_LEN);
    while(!feof(input_file)){
        fgets(line, MAX_LINE_LEN, input_file);
        printf("%s\n", line);
        c = handle_line(line, &in_macro, ht);

    }
    return output_file;
}

int pre_assembler(char* name_of_file){


    FILE *file_to_scan, *post_analysis;
    file_to_scan = fopen(name_of_file, "r+");
    if(!file_to_scan){
        printf("[ERROR] Failed to open file %s\n", name_of_file);
        return 1;
    }
    post_analysis = analyze(file_to_scan, post_analysis);
    fclose(file_to_scan);
    return 0;
}