
#ifndef LABORATORY_LINKED_LIST_H
#define LABORATORY_LINKED_LIST_H

typedef struct {
    int data;
    char* file_name;
    struct Node* next;
} Node;

/**
 * Create a new node to the linked-list.
 * @param file_name - file name to save in the node.
 * @return - pointer to the node.
 */
Node* create_node(char* file_name);

/**
 * Adds or updates the list according to the integer received. If file exists in the linked list, means the integer
 * exists already, then just update the node.
 * @param head - the head of the given linked list.
 * @param file_name - name of the file the integer read from.
 */
void add_or_update_list(Node **head, char* file_name);

#endif /* LABORATORY_LINKED_LIST_H */
