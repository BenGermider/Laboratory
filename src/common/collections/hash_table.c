#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/common/collections/hash_table.h"


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
    size_t i;
    unsigned int new_index;
    if (new_table == NULL) {
        return;
    }

    for (i = 0; i < ht->size; i++) {
        KeyVal* current = ht->table[i];
        while (current != NULL) {
            KeyVal* next = current->next;
            new_index = hash(current->key, new_size);
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
    unsigned int index;
    KeyVal* newPair;
    KeyVal* current;

    /* TODO: CHECK IF NECESSARY */
    if (ht->count >= ht->size * 0.75) {
        resizeHashTable(ht, ht->size * 2);
    }

    index = hash(key, ht->size);
    newPair = (KeyVal*)malloc(sizeof(KeyVal));
    if (newPair == NULL) {
        return;
    }

    newPair->key = (char*)malloc(strlen(key) + 1);
    if (newPair->key == NULL) {
        free(newPair);
        return;
    }
    strcpy(newPair->key, key);

    newPair->value = (char*)malloc(strlen(value) + 1);
    if (newPair->value == NULL) {
        free(newPair->key);
        free(newPair);
        return;
    }
    strcpy(newPair->value, value);

    newPair->next = NULL;

    if (ht->table[index] == NULL) {
        ht->table[index] = newPair;
        ht->count++;
    } else {
        current = ht->table[index];
        while (current != NULL) {
            if (strcmp(current->key, key) == 0) {
                free(current->value);
                current->value = (char*)malloc(strlen(value) + 1);
                if (current->value == NULL) {
                    /* Handle allocation failure */
                    free(newPair->key);
                    free(newPair->value);
                    free(newPair);
                    return;
                }
                strcpy(current->value, value);
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
        current->next =  newPair;
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
    size_t i;
    for (i = 0; i < ht->size; i++) {
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

void printHashTable(HashTable* ht) {
    size_t i;
    if (ht == NULL) {
        printf("Hash table is NULL\n");
        return;
    }

    printf("Hash Table Contents:\n");
    printf("--------------------\n");
    for (i = 0; i < ht->size; i++) {
        KeyVal* current = ht->table[i];
        if (current != NULL) {
            printf("Bucket %d:\n", (int)i);
            while (current != NULL) {
                printf("  Key: %s, Value: %s\n", current->key, current->value);
                current = current->next;
            }
        }
    }
    printf("--------------------\n");
    printf("Total items: %d\n", (int) ht->count);
    printf("Table size: %d\n", (int) ht->size);
    printf("Load factor: %.2f\n", (float)ht->count / ht->size);
}