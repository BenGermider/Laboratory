#include <string.h>
#include "../../../include/common/library.h"
#include "../../../include/common/operands/registers.h"

/**
 * Returns if argument is valid register or not
 * @param str suspected as register
 * @return 1 if valid 0 otherwise
 */
int is_reg(char *str){
    int i;
    if(str == NULL){
        return 0;
    }
    for(i = 0; i < 8; i++){
        if(strcmp(str, REGISTERS[i].name) == 0){
            return 1;
        }
    }
    return 0;
}

/**
 * Returns if valid register operand
 * @param operand suspected as register operand
 * @return 0 if not valid, 1 if valid.
 */
int reg_arg(char* operand){
    char *copy;

    if(!operand){
        return 0;
    }

    if(is_reg(operand)){
        return 1;
    } else {
        if(*operand == '*'){
            copy = operand + 1;
            if(is_reg(copy)){
                return 1;
            }
        } else {
            return 0;
        }
    }
    return 0;
}

/**
 * Returns the number of register
 * @param reg register
 * @return it's number
 */
int get_reg(char* reg){
    int i;
    char* reg_cpy;
    if(*reg == '*'){
        reg_cpy = reg + 1;
    } else {
        reg_cpy = reg;
    }
    for(i = 0; i < 8; i++){
        if(strcmp(reg_cpy, REGISTERS[i].name) == 0){
            return i;
        }
    }
    return -1;
}
