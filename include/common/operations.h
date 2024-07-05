#ifndef LABORATORY_OPERATIONS_H
#define LABORATORY_OPERATIONS_H

typedef struct {
    char* name;
    int opcode;
    int arg_count;
} Operation;

typedef struct {
    char* name;
} Instruction;

#endif
