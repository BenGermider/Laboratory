#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "../data_types.h"

typedef struct Node {
    SignData *data;
    struct Node *prev;
    struct Node *next;
} Node;

/* Creates a node storing a number and a string */
Node* create_node(int number, const char *text);

/* Adds a new node into the list */
int append(Node **head, int number, const char *text);

/* Checks whether a node holding a specific string exists */
int exists(Node*, char*, int);

/* Get a node from the list */
Node* get_node(Node* head, char* label);

/* Prints a list TODO: REMOVE IT'S FOR DEBUGGING */
void print_list(Node *head);

/* Free a list at the end of the use */
void free_list(Node *head);

#endif /* LINKED_LIST_H */