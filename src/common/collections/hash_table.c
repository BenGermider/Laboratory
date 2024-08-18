#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/common/collections/hash_table.h"

/**
 * Returns the hash code for the key according to hash formula.
 * @param key to generate hash code for.
 * @param table_size size of the current table
 * @return hash code for the key.
 */
unsigned int hash(char* key, size_t table_size) {
    unsigned int hash = 0;
    while (*key) {
        hash = (hash * 31) + *key;
        key++;
    }
    return hash % table_size;
}

/**
 * Initialize a hash table.
 * @return pointer to the hash table.
 */
hash_table* init_hash_table() {
    hash_table* h_table = (hash_table*)malloc(sizeof(hash_table));
    if (h_table == NULL) {
        return NULL;
    }
    h_table->table = (key_val**)calloc(1, sizeof(key_val*));
    if (h_table->table == NULL) {
        free(h_table);
        return NULL;
    }
    h_table->size = 1;
    h_table->count = 0;
    return h_table;
}

/**
 * Resizes the hash table
 * @param h_table hash table to resize
 * @param new_size desired new size for the hash table.
 */
void resize_hash_table(hash_table* h_table, size_t new_size) {
    key_val** new_table = (key_val**)calloc(new_size, sizeof(key_val*));
    size_t i;
    unsigned int new_index;
    if (new_table == NULL) {
        return;
    }

    for (i = 0; i < h_table->size; i++) {
        /* move the old hash table to the new hash table */
        key_val* current = h_table->table[i];
        while (current != NULL) {
            key_val* next = current->next;
            new_index = hash(current->key, new_size);
            current->next = new_table[new_index];
            new_table[new_index] = current;
            current = next;
        }
    }

    free(h_table->table);
    h_table->table = new_table;
    h_table->size = new_size;
}

/**
 * Insert a new key-val pair into the hash table.
 * @param h_table hash table to insert to.
 * @param key of the pair
 * @param value of the pair
 */
void insert(hash_table* h_table, char* key, char* value) {
    unsigned int index;
    key_val* newPair;
    key_val* current;

    /* TODO: CHECK IF NECESSARY */
    if (h_table->count >= h_table->size * 0.75) {
        /* Resize the hash table */
        resize_hash_table(h_table, h_table->size * 2);
    }

    index = hash(key, h_table->size); /* Calc hash for key */
    newPair = (key_val*)malloc(sizeof(key_val));
    if (newPair == NULL) {
        return;
    }
    /* Save key val pair */
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

    /* place new key-val in the hash-table */
    if (h_table->table[index] == NULL) {
        h_table->table[index] = newPair;
        h_table->count++;
    } else {
        current = h_table->table[index];
        while (current != NULL) {
            if (strcmp(current->key, key) == 0) {
                /* Override the old value */
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
        h_table->count++;
    }
}

/**
 * Get the value of the key from the hash-table.
 * @param h_table hash table that stores key-val
 * @param key to get the value
 * @return the value
 */
char* get(hash_table* h_table,  char* key) {
    unsigned int index = hash(key, h_table->size);
    key_val* current = h_table->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

/**
 * Free the hash-table
 * @param h_table hash table to free allocated memory.
 */
void free_hash_table(hash_table* h_table) {
    size_t i;
    for (i = 0; i < h_table->size; i++) {
        key_val* current = h_table->table[i];
        while (current != NULL) {
            key_val* temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(h_table->table);
    free(h_table);
}

void flush_hash_table(hash_table* h_table) {
    size_t i;
    for (i = 0; i < h_table->size; i++) {
        key_val *current = h_table->table[i];
        while (current != NULL) {
            key_val *temp = current;
            current = current->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
        h_table->table[i] = NULL;
    }
    h_table->count = 0;
}

/* TODO: FOR DEBUGGING, REMOVE */
void print_hash_table(hash_table* h_table) {
    size_t i;
    if (h_table == NULL) {
        printf("Hash table is NULL\n");
        return;
    }

    printf("Hash Table Contents:\n");
    printf("--------------------\n");
    for (i = 0; i < h_table->size; i++) {
        key_val* current = h_table->table[i];
        if (current != NULL) {
            printf("Bucket %d:\n", (int)i);
            while (current != NULL) {
                printf("  Key: %s, Value: %s\n", current->key, current->value);
                current = current->next;
            }
        }
    }
    printf("--------------------\n");
    printf("Total items: %d\n", (int) h_table->count);
    printf("Table size: %d\n", (int) h_table->size);
    printf("Load factor: %.2f\n", (float)h_table->count / h_table->size);
}