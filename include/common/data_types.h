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

typedef struct {
    char *label;
    int *data;
    size_t size;
    int src;
} instruction_sentence;

typedef struct {
    char *label;
    int operation;
    char *src;
    char *dest;
    int ARE;
} command_sentence;

#endif
