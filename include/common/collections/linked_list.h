#ifndef LABORATORY_LINKED_LIST_H
#define LABORATORY_LINKED_LIST_H

typedef struct Node {
    int data;
    struct Node* next;
} Node;

Node* create_node(int data);

void append(Node** head_ref, int new_data);

void print_list(Node* node);

void delete_list(Node** head_ref);

#endif
