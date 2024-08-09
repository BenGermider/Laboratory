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
    int line;
    char *text;
} LabelData;

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
    INSTRUCTION,
    COMMAND
} SentenceType;

typedef enum {
    DESTINATION = 1,
    SOURCE = 2
} OPERAND;

#endif
