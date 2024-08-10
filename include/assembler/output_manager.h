#ifndef LABORATORY_OUTPUT_MANAGER_H
#define LABORATORY_OUTPUT_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "../../include/common/data_types.h"
#include "../../include/common/collections/linked_list.h"
#include "../../include/common/consts.h"
#include "../../include/common/utils.h"

char* short_to_5_digit_octal(short int num);

void write_obj_file(const char* file_name, short int* code, int IC, int DC);

void write_entry_file(const char* file_name, Node** labels, Node** entries);

void write_extern_file(const char* file_name, Node** ext_file);

#endif /* LABORATORY_OUTPUT_MANAGER_H */
