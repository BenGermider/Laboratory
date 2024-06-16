
#ifndef LABORATY_LINKED_LIST_H
#define LABORATY_LINKED_LIST_H

typedef struct {
    int data;
    char* file_name;
    struct Node* next;
} Node;

Node* create_node(char* file_name);

void add_to_tail(Node **head, char* file_name);

void count(Node** head, char* file_name);

#endif /* LABORATY_LINKED_LIST_H */
