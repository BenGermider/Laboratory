#include <stdlib.h>
#include <stdio.h>
#include "set.h"

int main(){
    Set SETA = {0};
    Set SETB = {0};
    Set SETC = {0};
    Set SETD = {0};
    Set SETE = {0};
    Set SETF = {0};
    int arr[] = {1, 2, 3};
    int arr2[] = {6, 5, 4};

//    int arr[] = {1,2,3,4,5, 6, 1, 1, 1, -1};
    read_set(&SETA, arr, sizeof(arr) / sizeof(int));
    read_set(&SETB, arr2, sizeof(arr2) / sizeof(int));
    union_set(&SETA, &SETB, &SETC);
    print_set(&SETC);
//    read_set()
//    print_set(&SETA);
    return 0;
}
