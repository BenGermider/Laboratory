#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../include/common/collections/linked_list.h"

/**
 * Creates a node that holds a data which is an int and a string
 * @param number int to hold in the node
 * @param text string to hold in the node.
 * @return a pointer to the node.
 */
Node* create_node(int number, const char *text) {
    Node *new_node;

    new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = (SignData *)malloc(sizeof(SignData));
    if(new_node->data == NULL){
        free(new_node);
        return NULL;
    }
    new_node->data->line = number;
    new_node->data->text = (char *)malloc(strlen(text) + 1);
    if (new_node->data->text == NULL) {
        free(new_node->data);
        return NULL;
    }
    strcpy(new_node->data->text, text);

    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}

/**
 * Add a new node into the list
 * @param head of the list
 * @param number int stored in the new node
 * @param text string stored in the new node
 * @return code of success or failure
 */
int append(Node **head, int number, const char *text) {
    Node *new_node;
    Node *temp;
    new_node = create_node(number, text);
    if(new_node == NULL){
        return 1;
    }
    if (*head == NULL) {
        *head = new_node;
        return 0;
    }

    temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
    new_node->prev = temp;
    return 0;
}

/**
 * Finds if there's a node holding that string
 * @param head of the list
 * @param label string to check if exists in the list
 * @param invalid flag for an error
 * @return 1 if exists, 0 otherwise
 */
int exists(Node *head, char* label, int invalid) {
    Node *temp;

    temp = head;
    if(label == NULL){
        return 0;
    }
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

/**
 * Get a node from the list by the string it holds
 * @param head of the list
 * @param label string to look for
 * @return pointer to the desired node, NULL if there's no such node.
 */
Node* get_node(Node* head, char* label){
    Node *temp;
    temp = head;
    while (temp != NULL) {
        if (strcmp(temp->data->text, label) == 0) {
            return temp;  /* Found */
        }
        temp = temp->next;
    }
    return NULL;  /* Not found */
}

/**
 * Prints the list
 * @param head of the list
 */
void print_list(Node *head) {
    Node *temp;

    temp = head;

    while (temp != NULL) {
        printf("In line %d, encountered: %s\n", temp->data->line, temp->data->text);
        temp = temp->next;
    }
}

/**
 * Frees all nodes of the list
 * @param head of the list
 */
void free_list(Node *head) {
    Node *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp->data->text);  /* Free the duplicated string */
        free(temp);
    }
}