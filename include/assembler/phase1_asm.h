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
#include "../common/collections/hash_table.h"

int is_valid_label(char **label, int is_decl);

int store_or_src(char *instruction);

void declare_sentence(InstructionSentence *sen);

int insert_label_table(Node **database, char *label, int lines);

InstructionSentence* store_data(char* line);

InstructionSentence* src_handling(char* line, int src_index);

void insert_source_label(Node** list, char* label, int line);

void generate_command(CommandSentence *c_s);

void get_command(CommandSentence* c_s, char* command);

void args(CommandSentence* c_s, char* command);

void word_count(CommandSentence* c_s);

void analyze_command(CommandSentence* c_s, char* command);

CommandSentence *pull_command(char *command, int line);

int first_pass(FILE*, Node**, Node**, Node**, SentenceList*, SentenceList*, HashTable*);

void declare_lists(SentenceList* code, SentenceList* data);

char* get_line_copy(const char* origin_line);

void get_file(const char* file_name, char** input_file, const char* suffix);

#endif /* LABORATORY_PHASE1_ASM_H */
