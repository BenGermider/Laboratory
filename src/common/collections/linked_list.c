#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/common/collections/linked_list.h"

/* Create a new node with given data */
Node* create_node(int number, const char *text) {
    Node *new_node;

    new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        perror("Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = (LabelData*)malloc(sizeof(LabelData));
    if(new_node->data == NULL){
        printf("[ERROR] Failed to allocate memory\n");
        free(new_node);
        exit(EXIT_FAILURE);
    }
    new_node->data->line = number;
    new_node->data->text = (char *)malloc(strlen(text) + 1);
    if (new_node->data->text == NULL) {
        perror("Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    }
    strcpy(new_node->data->text, text);

    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}

/* Append a new node to the end of the list */
void append(Node **head, int number, const char *text) {
    Node *new_node;
    Node *temp;
    new_node = create_node(number, text);
    if (*head == NULL) {
        *head = new_node;
        return;
    }

    temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
    new_node->prev = temp;
}

/* Check if a node with a given number exists in the list */
int exists(Node *head, char* label, int invalid) {
    Node *temp;

    temp = head;
    while (temp != NULL) {
        if (strcmp(temp->data->text, label) == 0) {
            if(invalid) {
                printf("[ERROR] Invalid usage of source declaration on label.\n");
            }
            return 1;  /* Found */
        }
        temp = temp->next;
    }
    return 0;  /* Not found */
}

void print_list(Node *head) {
    Node *temp;

    temp = head;
    while (temp != NULL) {
        printf("Number: %d, Text: %s\n", temp->data->line, temp->data->text);
        temp = temp->next;
    }
}

/* Free all nodes in the list */
void free_list(Node *head) {
    Node *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->data->text);  /* Free the duplicated string */
        free(temp);
    }
}