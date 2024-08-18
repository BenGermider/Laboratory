#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/common/collections/hash_table.h"
#include "../../../include/common/utils.h"

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
    key_val* new_key_val;
    key_val* current;

    if (h_table->count >= h_table->size / 2) {
        /* Resize the hash table to avoid possible dump */
        resize_hash_table(h_table, h_table->size * 2);
    }

    index = hash(key, h_table->size); /* Calc hash for key */
    new_key_val = (key_val*)malloc(sizeof(key_val));
    if (new_key_val == NULL) {
        return;
    }
    /* Save key val pair */
    new_key_val->key = (char*)malloc(strlen(key) + 1);
    if (new_key_val->key == NULL) {
        free(new_key_val);
        return;
    }
    strcpy(new_key_val->key, key);

    new_key_val->value = (char*)malloc(strlen(value) + 1);
    if (new_key_val->value == NULL) {
        free_space(2, new_key_val->key, new_key_val);
        return;
    }
    strcpy(new_key_val->value, value);

    new_key_val->next = NULL;

    /* place new key-val in the hash-table */
    if (h_table->table[index] == NULL) {
        h_table->table[index] = new_key_val;
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
                    free_space(3, new_key_val->key, new_key_val->value, new_key_val);
                    return;
                }
                strcpy(current->value, value);
                free_space(3, new_key_val->key, new_key_val->value, new_key_val);
                return;
            }
            if (current->next == NULL) {
                break;
            }
            current = current->next;
        }
        current->next =  new_key_val;
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
    key_val* current, *temp;
    for (i = 0; i < h_table->size; i++) {
        current = h_table->table[i];
        while (current != NULL) {
            temp = current;
            current = current->next;
            free_space(3, temp->key, temp->value, temp);
        }
    }
    free_space(2,h_table->table, h_table);
}

void flush_hash_table(hash_table* h_table) {
    size_t i;
    for (i = 0; i < h_table->size; i++) {
        key_val *current = h_table->table[i];
        while (current != NULL) {
            key_val *temp = current;
            current = current->next;
            free_space(3, temp->key, temp->value, temp);
        }
        h_table->table[i] = NULL;
    }
    h_table->count = 0;
}
