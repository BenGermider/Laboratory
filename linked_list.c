#include <stdlib.h>
#include "linked_list.h"


Node* create_node(int data, char* file_name) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->data = data;
    node->file_name = file_name;
    node->next = NULL;
    return node;
}

void add_to_tail(Node **head, int data, char* file_name){
    Node* copy;
    Node* new_tail = create_node(data, file_name);
    if(*head == NULL){
        *head = new_tail;
    }
    copy = *head;
    while(copy->next != NULL){
        copy = copy->next;
    }
    copy->next = new_tail;
}
