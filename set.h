//
// Created by Ben on 6/4/2024.
//

#ifndef LABORATY_SET_H
#define LABORATY_SET_H

typedef struct {
    unsigned char bits[16];
} Set;

void initialize_set(Set* set);

void read_set(Set* set, int nums[], int arr_size);

void print_set(Set* set);

int is_bit_on(Set* set, int byte, int bit);

void union_set(Set* set_a, Set* set_b, Set* set_c);

void intersect_set(Set* set_a, Set* set_b, Set* set_c);

void sub_set(Set* set_a, Set* set_b, Set* set_c);

void symdiff_set(Set* set_a, Set* set_b, Set* set_c);

void stop();

#endif //LABORATY_SET_H
