#ifndef LABORATORY_OPERATIONS_H
#define LABORATORY_OPERATIONS_H

#include <stdlib.h>

typedef struct {
    char* name;
    int opcode;
    int arg_count;
    int src_arg[4];
    int dest_arg[4];
} Operation;

typedef struct {
    char* name;
} Instruction;

typedef struct {
    char* name;
} Reg;

typedef struct {
    int line;
    char *text;
} SignData;

typedef struct {
    char *label;
    int *data;
    size_t size;
    int src;
    int pos;
} InstructionSentence;

typedef struct {
    char *label;
    int pos;
    int operation;
    char *src;
    char *dest;
    int word_count;
} CommandSentence;

typedef union {
    InstructionSentence* instruction;
    CommandSentence* command;
} Sentence;

typedef enum {
    INST_BOUND = 15,
    COM_BOUND = 12
} INT_BOUND;

typedef enum {
    INSTRUCTION,
    COMMAND
} SentenceType;

typedef enum {
    DESTINATION = 3,
    SRC_OP = 6,
    SOURCE = 7,
    OPCODE = 11
} OPERAND;

typedef enum {
    EXTERNAL = 1,
    RELOCATABLE = 2,
    ABSOLUTE = 4
} ARE;


#endif
