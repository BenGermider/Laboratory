#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"

/**
 * Create a new node to the linked-list.
 * @param file_name - file name to save in the node.
 * @return - pointer to the node.
 */
Node* create_node(char* file_name){
    /* Creates a node and handle error */
    Node* new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL){
        fprintf(stderr, "[ERROR] Failed to allocate memory, terminating...\n");
        exit(1);
    }
    /* Set node's data */
    new_node->file_name = file_name;
    new_node->data = 1;
    new_node->next = NULL;
    return new_node;
}

/**
 * Adds or updates the list according to the integer received. If file exists in the linked list, means the integer
 * exists already, then just update the node.
 * @param head - the head of the given linked list.
 * @param file_name - name of the file the integer read from.
 */
void add_or_update_list(Node** head, char* file_name){
    Node *copy, *new_node;
    /* If list is empty add to head */
    if(*head == NULL){
        *head = create_node(file_name);
        return;
    }
    copy = *head;
    while(copy->next && copy->file_name != file_name){
        /* Iterate until reaches end of list */
        copy = (Node*)copy->next;
    }
    if(strcmp(copy->file_name, file_name) == 0){
        /* If number already exists in file, increase the amount of occurrences */
        copy->data++;
    } else {
        /* Else add it into the list */
        new_node = create_node(file_name);
        copy->next = (struct Node*)new_node;
    }
}
