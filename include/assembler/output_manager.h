#ifndef LABORATORY_OUTPUT_MANAGER_H
#define LABORATORY_OUTPUT_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "../../include/common/data_types.h"
#include "../../include/common/collections/linked_list.h"
#include "../../include/common/consts.h"
#include "../../include/common/utils.h"

/* Converts a number into a 5-digit padded octal number */
char* short_to_5_digit_octal(unsigned short int num);

/* Writes a .ob file out of the assembly code received */
void write_obj_file(const char* file_name, unsigned short int* code, int IC, int DC);

/* Writes a .ent file that holds the entry labels and where they are called in the code */
void write_entry_file(const char* file_name, Node** labels, Node** entries);

/* Writes a .ext file that holds the extern labels and where they are called in the code */
void write_extern_file(const char* file_name, Node** ext_file);

#endif /* LABORATORY_OUTPUT_MANAGER_H */
