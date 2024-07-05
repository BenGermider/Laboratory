#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/common/utils.h"


void clear_side_blanks(char** line){
    char *end_of_line, *line_copy;
    line_copy = (char*)malloc(strlen(*line) + 1);
    strcpy(line_copy, *line);
    while(isspace(*line_copy)) line_copy++;
    end_of_line = line_copy + strlen(line_copy) - 1;
    while(isspace(*end_of_line)) end_of_line--;
    *(end_of_line + 1) = '\0';
    *line = line_copy;
}