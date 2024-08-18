#ifndef LABORATORY_PHASE2_ASM_H
#define LABORATORY_PHASE2_ASM_H

#include "../../include/common/data_types.h"
#include "../../include/common/operands/registers.h"
#include "../../include/common/operands/integers.h"
#include "../../include/common/collections/linked_list.h"
#include "../../include/common/collections/sentence_list.h"
#include "../../include/common/consts.h"

/* Differentiate between types of operands received in the code */
int get_operand_type(char*, INT_BOUND);

/* Converts a command into a number */
unsigned short int operation_as_num(CommandSentence*, Node**);

/* Gets code for storing two registers in one word. */
unsigned short int two_regs(char*, char*);

/* Converts a line of command into a number */
unsigned short int operand_as_code(char*, Node**, Node**, OPERAND, Node**, int);

/* Sets the value of the operands in the command */
void handle_operands(unsigned short int**, CommandSentence*, Node**, Node**, Node**);

/* Convert all commands into a code */
unsigned short int* get_command_code(CommandSentence*, Node**,  Node**, Node**, Node**);

/* Convert all instructions into a code */
unsigned short int* get_instruction_code(InstructionSentence*);

/* Gets machine code for given commands and instructions */
unsigned short int* calc_code(
        SentenceList*,
        SentenceList*,
        int*,
        Node**,
        Node**,
        Node**,
        Node**
);

/* Converts data received from first pass into desired output files */
int second_pass(
        const char*,
        Node** labels,
        Node** entries,
        Node** externals,
        Node** errors,
        SentenceList* code,
        SentenceList* data
);

#endif /* LABORATORY_PHASE2_ASM_H */
