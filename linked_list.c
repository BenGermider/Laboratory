#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "linked_list.h"

Node* create_node(char* file_name){
    Node* new_node = (Node*)malloc(sizeof(Node));
    if(new_node == NULL){
        fprintf(stderr, "[ERROR] Failed to allocate memory, terminating...\n");
        exit(1);
    }
    new_node->file_name = file_name;
    new_node->data = 1;
    new_node->next = NULL;
    return new_node;
}

void add_to_tail(Node** head, char* file_name){
    Node *copy, *new_node;
    if(*head == NULL){
        *head = create_node(file_name);
        return;
    }
    copy = *head;
    while(copy->next && copy->file_name != file_name){
        copy = (Node*)copy->next;
    }
    if(strcmp(copy->file_name, file_name) == 0){
        copy->data++;
    } else {
        new_node = create_node(file_name);
        copy->next = (struct Node*)new_node;
    }
}
