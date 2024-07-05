#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/common/hash_table.h"


unsigned int hash(char* key, size_t table_size) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key;
        key++;
    }
    return hash % table_size;
}

HashTable* createHashTable() {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (ht == NULL) {
        return NULL;
    }
    ht->table = (KeyVal**)calloc(1, sizeof(KeyVal*));
    if (ht->table == NULL) {
        free(ht);
        return NULL;
    }
    ht->size = 1;
    ht->count = 0;
    return ht;
}

void resizeHashTable(HashTable* ht, size_t new_size) {
    KeyVal** new_table = (KeyVal**)calloc(new_size, sizeof(KeyVal*));
    if (new_table == NULL) {
        return;
    }

    for (size_t i = 0; i < ht->size; i++) {
        KeyVal* current = ht->table[i];
        while (current != NULL) {
            KeyVal* next = current->next;
            unsigned int new_index = hash(current->key, new_size);
            current->next = new_table[new_index];
            new_table[new_index] = current;
            current = next;
        }
    }

    free(ht->table);
    ht->table = new_table;
    ht->size = new_size;
}

void insert(HashTable* ht, char* key, char* value) {
    /* TODO: CHECK IF NECESSARY */
    if (ht->count >= ht->size * 0.75) {
        resizeHashTable(ht, ht->size * 2);
    }

    unsigned int index = hash(key, ht->size);
    KeyVal* newPair = (KeyVal*)malloc(sizeof(KeyVal));
    if (newPair == NULL) {
        return;
    }
    newPair->key = strdup(key);
    newPair->value = strdup(value);
    newPair->next = NULL;

    if (ht->table[index] == NULL) {
        ht->table[index] = newPair;
        ht->count++;
    } else {
        KeyVal* current = ht->table[index];
        while (current != NULL) {
            if (strcmp(current->key, key) == 0) {
                free(current->value);
                current->value = strdup(value);
                free(newPair->key);
                free(newPair->value);
                free(newPair);
                return;
            }
            if (current->next == NULL) {
                break;
            }
            current = current->next;
        }
        current->next = newPair;
        ht->count++;
    }
}

char* get(HashTable* ht,  char* key) {
    unsigned int index = hash(key, ht->size);
    KeyVal* current = ht->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void freeHashTable(HashTable* ht) {
    for (size_t i = 0; i < ht->size; i++) {
        KeyVal* current = ht->table[i];
        while (current != NULL) {
            KeyVal* temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}
