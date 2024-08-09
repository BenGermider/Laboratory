#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "../data_types.h"

typedef struct Node {
    LabelData *data;
    struct Node *prev;
    struct Node *next;
} Node;

Node* create_node(int number, const char *text);

void append(Node **head, int number, const char *text);

int exists(Node*, char*, int);

Node* get_node(Node* head, char* label);

void print_list(Node *head);

void free_list(Node *head);

#endif /* LINKED_LIST_H */