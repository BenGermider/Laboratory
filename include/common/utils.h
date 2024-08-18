#ifndef LABORATORY_UTILS_H
#define LABORATORY_UTILS_H

/* Clears blanks from both edges of the string, including/excluding newline, depending on the flag */
void clear_side_blanks(char**, int);

/* Get a substring from a string which doesn't include a blank */
char* get_word(char*);

/* Free multiple dynamically allocated data */
void free_space(int amount, ...);

/* Checks whether the line is ignorable (comment or blanks) */
int is_ignorable(char*);

/* Get a file name by name and suffix */
int get_file(const char* file_name, char** input_file, const char* suffix);

/* Get a line copy */
char* get_line_copy(const char* origin_line);

#endif
