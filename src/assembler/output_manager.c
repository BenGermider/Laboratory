#include <stdio.h>
#include <stdlib.h>
#include "../../include/common/data_types.h"
#include "../../include/common/collections/linked_list.h"
#include "../../include/common/consts.h"
#include "../../include/common/utils.h"
#include "../../include/assembler/output_manager.h"

/**
 * Converts a machine code into 5-digits octal string according to the given protocol.
 * @param num code represented as an int to be converted.
 * @return code converted into a string.
 */
char* short_to_5_digit_octal(unsigned short int num) {
    char* result;
    int i;
    unsigned short int abs_num;

    /* Get memory for the code */
    result = (char*)malloc(6 * sizeof(char));
    if (result == NULL) {
        return NULL;
    }

    if (num < 0) {
        /* Handle negative numbers received TODO: CHECK IF RELEVANT*/
        abs_num = (unsigned short int)(num + 65536);
    } else {
        abs_num = (unsigned short int)num;
    }

    /* Adds for the string each digit as octal into the string */
    for (i = 4; i >= 0; i--) {
        result[i] = (abs_num % 8) + '0';
        abs_num = abs_num / 8;
    }

    result[5] = '\0';

    return result;
}

/**
 * Receives machine code for each line and saves it in a .ob file
 * @param file_name name of output file, without suffix.
 * @param code machine code of the assembly file.
 * @param IC number of words in RAM the code stored.
 * @param DC amount of data stored in RAM.
 */
void write_obj_file(const char* file_name, unsigned short int* code, int IC, int DC){
    char* obj_file, *octal;
    int i;
    FILE* f;

    /* Get the name of the file with the suffix and open it if possible */
    get_file(file_name, &obj_file, ".ob");
    f = fopen(obj_file, "w");
    if(f == NULL){
        printf("FAILED TO OPEN FILE.\n");
        return;
    }

    /* Prints the title into the file according to the protocol */
    fprintf(f, "  %d %d  \n", IC, DC);
    for(i = 0; i < DC + IC; i++){
        /* Adds each word as a code into the .ob file */
        octal = short_to_5_digit_octal(code[i]);
        fprintf(f, "%04d\t%s\n", FIRST_ADDRESS + i, octal);
        free(octal);
    }

    free(obj_file);
    fclose(f);
}

/**
 * Writes the .ent file, including name of labels declared as .entry and where they appear in the code
 * @param file_name of .ent file
 * @param labels all labels of the code
 * @param entries labels declared as .entry
 */
void write_entry_file(const char* file_name, Node** labels, Node** entries){
    char* entry_file;
    Node* label_node, *entry;
    FILE* f;

    /* Get the name of the file with the suffix and open it */
    get_file(file_name, &entry_file, ".ent");
    entry = *entries;
    if(entry == NULL){
        /* stop if no entry labels */
        return;
    }
    f = fopen(entry_file, "w");
    if(!f){
        printf("FAILED TO OPEN FILE\n");
        return;
    }

    /* Put in the .entry file every label declared as .entry and where it appears in code */
    while(entry != NULL){
        label_node = get_node(*labels, entry->data->text);
        if(label_node != NULL){
            entry->data->line = label_node->data->line;
        }
        fprintf(f, "%s %d\n", entry->data->text, entry->data->line);
        entry = entry->next;
    }

    free(entry_file);
    free_list(*entries);
    fclose(f);
}

/**
 * Writes a .ext file holding the extern labels and where they appear in the code
 * @param file_name file without the suffix of .ext
 * @param ext_file database holding the extern labels
 */
void write_extern_file(const char* file_name, Node** ext_file){
    char* ext_name;
    Node* ext_label;
    FILE* f;

    get_file(file_name, &ext_name, ".ext");
    ext_label = *ext_file;
    if(ext_label == NULL){
        /* stop if no extern labels */
        return;
    }

    /* Get the name of the file with the suffix and open it */
    f = fopen(ext_name, "w");
    if(!f){
        printf("FAILED TO OPEN FILE\n");
        return;
    }

    while(ext_label != NULL){
        /* Write the ext labels in the file */
        fprintf(f, "%s %04d\n", ext_label->data->text, ext_label->data->line);
        ext_label = ext_label->next;
    }

    free(ext_name);
    fclose(f);
}
