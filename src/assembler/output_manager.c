#include <stdio.h>
#include <stdlib.h>
#include "../../include/common/data_types.h"
#include "../../include/common/collections/linked_list.h"
#include "../../include/common/consts.h"
#include "../../include/common/utils.h"
#include "../../include/assembler/output_manager.h"

char* short_to_5_digit_octal(unsigned short int num) {
    char* result;
    int i;
    unsigned short int abs_num;

    result = (char*)malloc(6 * sizeof(char));
    if (result == NULL) {
        return NULL;
    }

    if (num < 0) {
        abs_num = (unsigned short int)(num + 65536);
    } else {
        abs_num = (unsigned short int)num;
    }

    for (i = 4; i >= 0; i--) {
        result[i] = (abs_num % 8) + '0';
        abs_num = abs_num / 8;
    }

    result[5] = '\0';

    return result;
}
void write_obj_file(const char* file_name, unsigned short int* code, int IC, int DC){
    char* obj_file, *octal;
    int i;
    FILE* f;
    get_file(file_name, &obj_file, ".ob");
    f = fopen(obj_file, "w");
    if(f == NULL){
        printf("FAILED TO OPEN FILE.\n");
        return;
    }
    fprintf(f, "  %d %d  \n", IC, DC);
    for(i = 0; i < DC + IC; i++){
        octal = short_to_5_digit_octal(code[i]);
        fprintf(f, "%d\t%s\n", FIRST_ADDRESS + i, octal);
        free(octal);
    }
    free(obj_file);
    fclose(f);
}

void write_entry_file(const char* file_name, Node** labels, Node** entries){
    char* entry_file;
    Node* label_node, *entry;
    FILE* f;
    get_file(file_name, &entry_file, ".ent");
    entry = *entries;
    if(entry == NULL){
        return;
    }
    f = fopen(entry_file, "w");
    if(!f){
        printf("FAILED TO OPEN FILE\n");
        return;
    }
    while(entry != NULL){
        label_node = get_node(*labels, entry->data->text);
        entry->data->line = label_node->data->line;
        fprintf(f, "%s %d\n", entry->data->text, entry->data->line);
        entry = entry->next;
    }
    free(entry_file);
    fclose(f);
}

void write_extern_file(const char* file_name, Node** ext_file){
    char* ext_name;
    Node* ext_label;
    FILE* f;
    get_file(file_name, &ext_name, ".ext");
    ext_label = *ext_file;
    if(ext_label == NULL){
        return;
    }
    f = fopen(ext_name, "w");
    if(!f){
        printf("FAILED TO OPEN FILE\n");
        return;
    }
    while(ext_label != NULL){
        fprintf(f, "%s %d\n", ext_label->data->text, ext_label->data->line + FIRST_ADDRESS);
        ext_label = ext_label->next;
    }
    free(ext_name);
    fclose(f);
}
