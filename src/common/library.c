#include "../../include/common/library.h"
#include "../../include/common/data_types.h"

const Operation OPERATIONS[] = {
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

const Instruction INSTRUCTIONS[] = {
        {".data"},
        {".string"},
        {".entry"},
        {".extern"}
};

const Reg REGISTERS[] = {
        {"r0"},
        {"r1"},
        {"r2"},
        {"r3"},
        {"r4"},
        {"r5"},
        {"r6"},
        {"r7"},
};