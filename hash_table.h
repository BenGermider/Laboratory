#include <stdlib.h>
#include "linked_list.h"

#ifndef LABORATY_HASH_TABLE_H
#define LABORATY_HASH_TABLE_H

typedef struct {
    Node* lists[29];
} HashTable;

HashTable* init_hash();

void add_to_hash(HashTable* hash_table, char* file_name, int* nums, size_t size);

void print_hash_table(HashTable* hash_table);

void run(int argc, char* argv[], HashTable* hash_table);

#endif /* LABORATY_HASH_TABLE_H */
