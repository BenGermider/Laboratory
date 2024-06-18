#include <stdlib.h>
#include <stdio.h>
#include "file_handler.h"
#include "utils.h"
#include "hash_table.h"
#include "linked_list.h"

HashTable* init_hash(){
    int i;
    HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable));
    if(!hash_table){
        fprintf(stderr, "[ERROR] Failed to allocate memory, terminating...\n");
        exit(1);
    }
    for(i = 0; i < 29; i++){
        hash_table->lists[i] = NULL;
    }
    return hash_table;
}

void add_to_hash(HashTable* hash_table, char* file_name, int* nums, size_t size){
    int i;
    for(i = 0; i < size; i++){
        add_to_tail(&hash_table->lists[nums[i]], file_name);
    }
}

void print_hash_table(HashTable* hash_table){
    int i;
    Node* head, *current;
    for(i = 0; i < 29; i++){
        head = hash_table->lists[i];
        if(!head){
            continue;
        }
        current = head;
        printf("%d appears in file ", i);
        while (current != NULL) {
            if(current->next){
                print(current->data, 1, current->file_name);
            } else {
                print(current->data, 0, current->file_name);
            }
            current = (Node*)current->next;
        }
    }
}

void run(int argc, char* argv[], HashTable* hash_table){
    int i;
    size_t size;
    int* nums;
    for(i = 1; i < argc; i++){
        nums = get_nums(argv[i], &size);
        add_to_hash(hash_table, argv[i], nums, size);
    }
    print_hash_table(hash_table);
    free(nums);
}