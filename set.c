#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"


void read_set(Set* set, int* arr, int arr_size){
    int i;
    int byte_index, bit_index;

    for (i = 0; i < arr_size; i++) {
        if(arr[i] == -1 && i == arr_size - 1){
            break;
        }
        else if(arr[i] >= 0 && arr[i] <= 127){
            byte_index = arr[i] / 8;
            bit_index = arr[i] % 8;
            set->bits[byte_index] |= (1 << (7 - bit_index));
        } else {
            exit(1);
        }
    }
}

void print_set(Set* set){
    int byte, bit;
    int empty_flag = 0, beginning = 1;
    for (byte = 0; byte < 16; byte++) {
        for (bit = 0; bit < 8; bit++) {
            if (set->bits[byte] & (1 << (7 - bit))) {
                // Bit is set, so print its index
                empty_flag = 1;
                if(beginning){
                    printf("SET: {");
                    beginning = 0;
                }
                int index = byte * 8 + bit;
                printf("%d ", index);
            }
        }
    }
    if(!beginning){
        printf("}");
    }
    if(!empty_flag){
        printf("The set is empty.");
    }
    printf("\n");
}

int is_bit_on(Set* set, int byte, int bit){
    if (set->bits[byte] & (1 << (7 - bit))) {
        return 1;
    }
    return 0;
}

void union_set(Set* set_a, Set* set_b, Set* set_c){
    int byte, bit;
    for (byte = 0; byte < 16; byte++) {
        for (bit = 0; bit < 8; bit++) {
            if (is_bit_on(set_a, byte, bit) || is_bit_on(set_b, byte, bit)){
                set_c->bits[byte] |= (1 << (7 - bit));
            } else {
                set_c->bits[byte] &= ~(1 << (7 - bit));
            }
        }
    }
}

void intersect_set(Set* set_a, Set* set_b, Set* set_c){
    int byte, bit;
    for (byte = 0; byte < 16; byte++) {
        for (bit = 0; bit < 8; bit++) {
            if (is_bit_on(set_a, byte, bit) && is_bit_on(set_b, byte, bit)){
                set_c->bits[byte] |= (1 << (7 - bit));
            } else {
                set_c->bits[byte] &= ~(1 << (7 - bit));
            }
        }
    }
}

void sub_set(Set* set_a, Set* set_b, Set* set_c){
    int byte, bit;
    for (byte = 0; byte < 16; byte++) {
        for (bit = 0; bit < 8; bit++) {
            if (is_bit_on(set_a, byte, bit) && !(is_bit_on(set_b, byte, bit))){
                set_c->bits[byte] |= (1 << (7 - bit));
            } else {
                set_c->bits[byte] &= ~(1 << (7 - bit));
            }
        }
    }
}

void symdiff_set(Set* set_a, Set* set_b, Set* set_c){
    int byte, bit;
    for (byte = 0; byte < 16; byte++) {
        for (bit = 0; bit < 8; bit++) {
            if (is_bit_on(set_a, byte, bit) ^ is_bit_on(set_b, byte, bit)){
                set_c->bits[byte] |= (1 << (7 - bit));
            } else {
                set_c->bits[byte] &= ~(1 << (7 - bit));
            }
        }
    }
}

void stop(){
    exit(0);
}



//char* miss_param = "Missing parameter.\n";
//char* illegal_c = "Illegal comma.\n";
