#include <stdlib.h>
#include "validation.h"
#include <stdio.h>
//
//void memory_allocation(const char* text){
//    if (text == NULL) {
//        printf("Memory allocation failed\n");
//        exit(1);
//    }
//}
//
//char* func_name(char* func){
//
//}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define bad_set_name "Undefined set name.\n"

void print_error(const char *message) {
    printf("%s\n", message);
}

#define MAX_SETS 6



//
//
//int main() {
//    char command[] = "sub_set SETA, SETB, SETC";
//    char func[] = "sub_set";
//    int set_count = 0;
//
////    char** sets = get_sets(command, func, &set_count);
////    if (sets) {
////        printf("Found sets:\n");
////        for (int i = 0; i < set_count; i++) {
////            printf("%s\n", sets[i]);
////            free(sets[i]);  // Free each duplicated string
////        }
////        free(sets);  // Free the array itself
////    }
//
//    return 0;

