#ifndef LABORATORY_HASH_TABLE_H
#define LABORATORY_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct key_val {
    char* key;
    char* value;
    struct key_val* next;
} key_val;

typedef struct hash_table {
    key_val** table;
    size_t size;
    size_t count;
} hash_table;

/* Calculates the hash value for every new key */
unsigned int hash(char* key, size_t table_size);

/* Initializes a hash table */
hash_table* init_hash_table();

/* Resizes hash table to be able to store more data and make each key a unique hash */
void resize_hash_table(hash_table* h_table, size_t new_size);

/* Insert new key-value into the hash table */
void insert(hash_table* h_table, char* key, char* value);

/* Get a value by a key from the hash table */
char* get(hash_table* h_table, char* key);

/* Free each pair of the hash table up to clearing the entire table */
void free_hash_table(hash_table* h_table);

void flush_hash_table(hash_table* h_table);

#endif
