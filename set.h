typedef struct {
    unsigned char bits[16];
} Set;

int read_set(Set* set, int nums[], int arr_size);

int print_set(Set* set);

int is_bit_on(Set* set, int byte, int bit);

int union_set(Set* set_a, Set* set_b, Set* set_c);

int intersect_set(Set* set_a, Set* set_b, Set* set_c);

int sub_set(Set* set_a, Set* set_b, Set* set_c);

int symdiff_set(Set* set_a, Set* set_b, Set* set_c);

int stop();
