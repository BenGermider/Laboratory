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

/* Checks whether the label received is legal */
int is_valid_label(char **label, int is_decl);

/* Differentiate between the two types of instructions */
int store_or_src(char *instruction);

/* Initializes an instruction */
void declare_sentence(InstructionSentence *sen);

/* Insert a label received into the label collection */
int insert_label_table(Node **database, char *label, int lines);

/* Handle an instruction of data storing */
InstructionSentence* store_data(char* line);

/* Handle an instruction of source declaration of a label */
InstructionSentence* src_handling(char* line, int src_index);

/* Insert label into the desired source label database */
void insert_source_label(Node** list, char* label, int line);

/* Initializes a command */
void generate_command(CommandSentence *c_s);

/* Gets the number of operation given */
void get_command(CommandSentence* c_s, char* command);

/* Reads and analyzes args of given operation */
void args(CommandSentence* c_s, char* command);

/* Calculates how many words the line will store in the RAM */
void word_count(CommandSentence* c_s);

/* Reads and analyzes the operation and arguments received in a line */
void analyze_command(CommandSentence* c_s, char* command);

/* Reads all information from a .am file */
CommandSentence *pull_command(char *command, int line);

/* Reads the file and saves crucial data towards conversion to machine code */
int first_pass(FILE*, Node**, Node**, Node**, SentenceList*, SentenceList*, HashTable*);

/* Initiates lists of code and data */
void declare_lists(SentenceList*, SentenceList*);

/* Returns a copy of a line */
char* get_line_copy(const char* origin_line);

/* Sets a new string which is a concatenation of a file name and a suffix. */
void get_file(const char* file_name, char** input_file, const char* suffix);

#endif /* LABORATORY_PHASE1_ASM_H */
