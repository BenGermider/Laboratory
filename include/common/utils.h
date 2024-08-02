#ifndef LABORATORY_UTILS_H
#define LABORATORY_UTILS_H

void clear_side_blanks(char**);

char* get_word(char*);

void free_space(int amount, ...);

int is_num_legal(char*);

int is_legal_string(char*);

int is_ignorable(char*);

#endif
