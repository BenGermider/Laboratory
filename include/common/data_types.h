#ifndef LABORATORY_OPERATIONS_H
#define LABORATORY_OPERATIONS_H

#include <stdlib.h>

typedef struct {
    char* name;
    int opcode;
    int arg_count;
} Operation;

typedef struct {
    char* name;
} Instruction;

typedef struct {
    char* name;
} Reg;

typedef struct label_data {
    int number;
    char *text;
} LabelData;

typedef struct {
    char *label;
    int *data;
    size_t size;
    int src;
} InstructionSentence;

typedef struct {
    char *label;
    int operation;
    char *src;
    char *dest;
    int ARE;
} CommandSentence;

#endif
