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

/* Differentiate between the two types of instructions */
int store_or_src(char *instruction);

/* Initializes an instruction */
void declare_sentence(InstructionSentence *sen);

/* Insert a label received into the label collection */
int insert_label_table(Node **database, char *label, int lines);

/* Handle an instruction of data storing */
InstructionSentence* store_data(char*, int, Node**);

/* Handle an instruction of source declaration of a label */
InstructionSentence* src_handling(char* line);

/* Initializes a command */
void generate_command(CommandSentence *c_s);

/* Gets the number of operation given */
void get_command(CommandSentence* c_s, char* command);

/* Reads and analyzes args of given operation */
void args(CommandSentence*, char*, int, Node**);

/* Calculates how many words the line will store in the RAM */
void word_count(CommandSentence* c_s);

/* Searches if macro and label share names */
int is_label_macro(HashTable* macros, char* label, int line, Node** errors);

/* Reads and analyzes the operation and arguments received in a line */
void analyze_command(CommandSentence*, char*, int, Node**);

/* Reads all information from a .am file */
CommandSentence *pull_command(char*, int, Node**, HashTable*);

/* Reads the file and saves crucial data towards conversion to machine code */
int first_pass(FILE*, Node**, Node**, Node**, Node**, SentenceList*, SentenceList*, HashTable*);

/* Initiates lists of code and data */
void declare_lists(SentenceList*, SentenceList*);


#endif /* LABORATORY_PHASE1_ASM_H */
