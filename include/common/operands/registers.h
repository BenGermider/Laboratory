#ifndef LABORATORY_REGISTERS_H
#define LABORATORY_REGISTERS_H

#include <string.h>
#include <stdlib.h>
#include "../../../include/common/library.h"

/* Checks whether the argument received represents a register */
int is_reg(char *str);

/* Checks if the register declaration is valid */
int reg_arg(char* operand);

/* Receives the number of register */
int get_reg(char* reg);

#endif /* LABORATORY_REGISTERS_H */
