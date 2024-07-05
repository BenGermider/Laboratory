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

unsigned int hash(char* key, size_t table_size);

HashTable* createHashTable();

void resizeHashTable(HashTable* ht, size_t new_size);

void insert(HashTable* ht, char* key, char* value);

char* get(HashTable* ht, char* key);

void freeHashTable(HashTable* ht);

#endif
