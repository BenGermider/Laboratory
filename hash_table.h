#include <stdlib.h>
#include "linked_list.h"

#ifndef LABORATORY_HASH_TABLE_H
#define LABORATORY_HASH_TABLE_H

typedef struct {
    Node* lists[29];
} HashTable;

/**
 * Initializes the hash table to empty linked lists.
 * @return - pointer to an empty hash-table.
 */
HashTable* init_hash();

/**
 * Adds an integer to the hash table and/or counts it's occurrences.
 * @param hash_table - hash table the integers are stored.
 * @param file_name - file name currently reading.
 * @param nums - array of integers being added to the hash-table.
 * @param size - amount of integers in nums.
 */
void add_to_hash(HashTable* hash_table, char* file_name, int* nums, size_t size);

/**
 * Prints the hash-table in the requested format.
 * @param hash_table - hash-table to print.
 */
void print_hash_table(HashTable* hash_table);

/**
 * Runs the program that receives files, stores in a hash-table the integers in the files and prints it.
 * @param argc - number of arguments received.
 * @param argv - name of arguments received.
 * @param hash_table - pointer to the initialized hash-table.
 */
void run(int argc, char* argv[], HashTable* hash_table);

#endif /* LABORATORY_HASH_TABLE_H */
