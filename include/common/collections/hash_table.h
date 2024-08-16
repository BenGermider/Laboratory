#ifndef LABORATORY_HASH_TABLE_H
#define LABORATORY_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KeyVal {
    char* key;
    char* value;
    struct KeyVal* next;
} KeyVal;

typedef struct HashTable {
    KeyVal** table;
    size_t size;
    size_t count;
} HashTable;

/* Calculates the hash value for every new key */
unsigned int hash(char* key, size_t table_size);

/* Initializes a hash table */
HashTable* createHashTable();

/* Resizes hash table to be able to store more data and make each key a unique hash */
void resizeHashTable(HashTable* ht, size_t new_size);

/* Insert new key-value into the hash table */
void insert(HashTable* ht, char* key, char* value);

/* Get a value by a key from the hash table */
char* get(HashTable* ht, char* key);

/* Free each pair of the hash table up to clearing the entire table */
void freeHashTable(HashTable* ht);

/* Prints the hash table TODO: REMOVE IT'S FOR DEBUGGING */
void printHashTable(HashTable* ht);
#endif
