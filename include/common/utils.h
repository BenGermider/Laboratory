#ifndef LABORATORY_UTILS_H
#define LABORATORY_UTILS_H

/* Clears the blanks from each side of the string including the new-line */
void clear_side_blanks_remove_newline(char** line);

/* Clears the blanks from each side of the string excluding the new-line */
void clear_side_blanks(char**);

/* Get a substring from a string which doesn't include a blank */
char* get_word(char*);

/* Free multiple dynamically allocated data */
void free_space(int amount, ...);

/* Checks whether the line is ignorable (comment or blanks) */
int is_ignorable(char*);

/* Get a file name by name and suffix */
void get_file(const char* file_name, char** input_file, const char* suffix);

/* Get a line copy */
char* get_line_copy(const char* origin_line);

#endif
