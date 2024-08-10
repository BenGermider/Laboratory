#ifndef LABORATORY_REGISTERS_H
#define LABORATORY_REGISTERS_H

#include <string.h>
#include <stdlib.h>
#include "../../../include/common/library.h"

int is_reg(char *str);

int reg_arg(char* operand);

int get_reg(char* reg);


#endif /* LABORATORY_REGISTERS_H */
