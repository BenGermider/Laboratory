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

/* Checks if data instructions actually hold data */
char* data_to_check(char*, int, int, Node**);

/* Handle an instruction of data storing */
InstructionSentence* store_data(char*, int, Node**);


/*Check whether argument received is a legal argument.*/
int is_valid_arg(char*, Node**, int);

/* Handle an instruction of source declaration of a label */
InstructionSentence* src_handling(char*, Node**, int);

/* Gets the number of operation given */
void get_command(CommandSentence*, char*);

/* Reads and analyzes args of given operation */
void args(CommandSentence*, char*, int, Node**);

/* Calculates how many words the line will store in the RAM */
void calc_word_count(CommandSentence*);

/* Searches if macro and label share names */
int is_label_macro(hash_table*, char*, int, Node**);

/* Reads and analyzes the operation and arguments received in a line */
void analyze_command(CommandSentence*, char*, int, Node**);

/* Reads all information from a .am file */
CommandSentence *pull_command(char*, int, Node**, hash_table*);

/* Reads the file and saves crucial data towards conversion to machine code */
int first_pass(FILE*, Node**, Node**, Node**, Node**, SentenceList*, SentenceList*, hash_table*);

#endif /* LABORATORY_PHASE1_ASM_H */
