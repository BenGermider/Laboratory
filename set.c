#include <stdio.h>
#include <stdlib.h>
#include "set.h"


int read_set(Set* set, int* arr, int arr_size){
    int i;
    int byte_index, bit_index;

    for(i = 0; i < 16; i++){
        set->bits[i] = 0;
    }

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
    return 0;

}

int print_set(Set* set){
    int byte, bit, index;
    int empty_flag = 0, beginning = 1;
    for (byte = 0; byte < 16; byte++) {
        for (bit = 0; bit < 8; bit++) {
            if (set->bits[byte] & (1 << (7 - bit))) {
                empty_flag = 1;
                if(beginning){
                    printf("SET: { ");
                    beginning = 0;
                }
                index = byte * 8 + bit;
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
    return 0;
}

int is_bit_on(Set* set, int byte, int bit){
    if (set->bits[byte] & (1 << (7 - bit))) {
        return 1;
    }
    return 0;
}

int union_set(Set* set_a, Set* set_b, Set* set_c){
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
    return 0;

}

int intersect_set(Set* set_a, Set* set_b, Set* set_c){
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
    return 0;

}

int sub_set(Set* set_a, Set* set_b, Set* set_c){
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
    return 0;
}

int symdiff_set(Set* set_a, Set* set_b, Set* set_c){
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
    return 0;
}

int stop(){
    printf("[TERMINATION] Turning off, have a nice day.\n");
    return 1;
}



