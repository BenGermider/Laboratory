#include "set.h"

char* clear_edges(char* text);

int is_valid_set_name(char* set_name);

int get_set_index(char* set_name);

char* get_prefix_plus_substring(char* input, char** func);

int is_integer(const char* num);

char* get_arr_as_string(char* command);

int calc_size(char *str);

int* get_arr_as_int(char* arr, int* size);

int* get_sets(char* command, const char* func, int* size);

char* read_command(char* command, char** func);

int analyze_command(char* command, const char* func, const int* set_pointers, Set sets[]);

int run(char* command, Set set_arr[], char* func, int size, int* sets);
