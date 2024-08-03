#include "../../../include/common/collections/linked_list.h"
#include <stdlib.h>
#include <stdio.h>



Node* create_node(int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

// Function to append a node to the end of the list
void append(Node** head_ref, int new_data) {
    Node* new_node = create_node(new_data);
    Node* last = *head_ref;

    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    while (last->next != NULL) {
        last = last->next;
    }

    last->next = new_node;
}

// Function to print the linked list
void print_list(Node* node) {
    while (node != NULL) {
        printf("%d -> ", node->data);
        node = node->next;
    }
    printf("NULL\n");
}

// Function to delete the entire linked list
void delete_list(Node** head_ref) {
    Node* current = *head_ref;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head_ref = NULL;
}