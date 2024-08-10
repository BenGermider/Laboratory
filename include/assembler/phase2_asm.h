#ifndef LABORATORY_PHASE2_ASM_H
#define LABORATORY_PHASE2_ASM_H

#include "../../include/common/data_types.h"
#include "../../include/common/operands/registers.h"
#include "../../include/common/operands/integers.h"
#include "../../include/common/collections/linked_list.h"
#include "../../include/common/collections/sentence_list.h"
#include "../../include/common/consts.h"

int get_operand_type(char* operand);

short int operation_as_num(CommandSentence*);

short int two_regs(char*, char*);

short int operand_as_code(char*, Node**, Node**, OPERAND);

void handle_operands(short int**, CommandSentence*, Node**, Node**);

short int* get_command_code(CommandSentence*, Node**,  Node**, Node**);

short int* get_instruction_code(InstructionSentence*);

short int* calc_code(
        SentenceList*,
        SentenceList*,
        int*,
        Node**,
        Node**,
        Node**,
        Node**
);

int second_pass(
        const char*,
        Node** labels,
        Node** entries,
        Node** externals,
        Node** ext_file,
        SentenceList* code,
        SentenceList* data
);

#endif /* LABORATORY_PHASE2_ASM_H */
