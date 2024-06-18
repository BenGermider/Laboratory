#include <stdlib.h>
#include <stdio.h>
#include "file_handler.h"
#include "utils.h"
#include "hash_table.h"
#include "linked_list.h"

#define MAX_VALUE 28

/**
 * Initializes the hash table to empty linked lists.
 * @return - pointer to an empty hash-table.
 */
HashTable* init_hash(){
    int i;
    /* Creates a hash-table and handles error */
    HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable));
    if(!hash_table){
        fprintf(stderr, "[ERROR] Failed to allocate memory, terminating...\n");
        exit(1);
    }
    /* Initializes every list to an empty list */
    for(i = 0; i <= MAX_VALUE; i++){
        hash_table->lists[i] = NULL;
    }
    return hash_table;
}

/**
 * Adds an integer to the hash table and/or counts it's occurrences.
 * @param hash_table - hash table the integers are stored.
 * @param file_name - file name currently reading.
 * @param nums - array of integers being added to the hash-table.
 * @param size - amount of integers in nums.
 */
void add_to_hash(HashTable* hash_table, char* file_name, int* nums, size_t size){
    int i;
    for(i = 0; i < size; i++){
        add_or_update_list(&hash_table->lists[nums[i]], file_name);
    }
}

/**
 * Prints the hash-table in the requested format.
 * @param hash_table - hash-table to print.
 */
void print_hash_table(HashTable* hash_table){
    int i;
    Node* head, *current;
    for(i = 0; i <= MAX_VALUE; i++){
        /* Run over the hash-table */
        head = hash_table->lists[i];
        if(!head){
            /* If the number doesn't exist in any of the given files, skip to the next number. */
            continue;
        }
        current = head;
        printf("%d appears in file ", i);
        while (current != NULL) {
            /* Print the occurrences in the required format (1 line for each number). */
            if(current->next){
                print(current->data, 1, current->file_name);
            } else {
                print(current->data, 0, current->file_name);
            }
            current = (Node*)current->next;
        }
    }
}

/**
 * Runs the program that receives files, stores in a hash-table the integers in the files and prints it.
 * @param argc - number of arguments received.
 * @param argv - name of arguments received.
 * @param hash_table - pointer to the initialized hash-table.
 */
void run(int argc, char* argv[], HashTable* hash_table){
    int i;
    size_t size;
    int* nums;
    /* Handles if not enough arguments given by the user */
    if(argc == 1){
        fprintf(stderr, "[ERROR] Not enough arguments given, terminating...\n");
        exit(1);
    }
    for(i = 1; i < argc; i++){
        /* Iterates over the given files, saves the in the hash-table. */
        nums = get_nums(argv[i], &size);
        add_to_hash(hash_table, argv[i], nums, size);
    }
    /* Prints the hash-table */
    print_hash_table(hash_table);
    free(nums);
}