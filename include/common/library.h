#include "operations.h"

#ifndef LABORATORY_LIBRARY_H
#define LABORATORY_LIBRARY_H

Operation OPERATIONS[] = {
        {"mov",0, 2},
        {"cmp",1, 2},
        {"add",2,2},
        {"sub",3,2},
        {"lea",4,2},
        {"clr",5,1},
        {"not",6,1},
        {"inc",7,1},
        {"dec",8,1},
        {"jmp",9,1},
        {"bne",10,1},
        {"red",11,1},
        {"prn",12,1},
        {"jsr",13,1},
        {"rts",14,0},
        {"stop",15,0},
};

Instruction INSTRUCTIONS[] = {
        {".data"},
        {".string"},
        {".entry"},
        {".extern"}
};


#endif
