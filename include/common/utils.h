#ifndef LABORATORY_UTILS_H
#define LABORATORY_UTILS_H

void clear_side_blanks_remove_newline(char** line);

void clear_side_blanks(char**);

char* get_word(char*);

void free_space(int amount, ...);

int is_ignorable(char*);

void get_file(const char* file_name, char** input_file, const char* suffix);

char* get_line_copy(const char* origin_line);

#endif
