
#ifndef LABORATORY_PHASE1_ASM_H
#define LABORATORY_PHASE1_ASM_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../../include/common/consts.h"
#include "../../include/common/utils.h"
#include "../../include/common/data_types.h"
#include "../../include/common/library.h"
#include "../common/collections/linked_list.h"
#include "../common/collections/sentence_list.h"

int is_reg(char *str);

int is_operation(char *str);

int is_valid_instruction(char* str);

int is_valid_label(char **label, int);

int store_or_src(char *instruction);

void instruction(char* line, char* line_copy);

void declare_sentence(InstructionSentence *sen);

int* pull_numbers(char* data, size_t* size);

InstructionSentence* store_data(char* line);

int first_pass(FILE* src_file, Node** labels, Node** externals, Node** entries, SentenceList* code);

char* get_line_copy(const char* origin_line);

void get_file(const char* file_name, char** input_file, const char*);

int assembler(const char* file_name);

#endif //LABORATORY_PHASE1_ASM_H
