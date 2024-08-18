#include <stdio.h>
#include <stdlib.h>
#include "../../../include/common/collections/sentence_list.h"

/**
 * Creates a node holding a sentence
 * @param data of the sentence
 * @param type of the sentence
 * @return a pointer to the node
 */
SenNode* create_sentence(void* data, SentenceType type) {
    SenNode *newNode = (SenNode*)malloc(sizeof(SenNode));

    if (newNode == NULL) {
        return NULL;
    }
    if(type == INSTRUCTION){
        newNode->sentence.instruction = (InstructionSentence*)data;
    } else if (type == COMMAND){
        newNode->sentence.command = (CommandSentence*)data;
    }
    newNode->type = type;
    newNode->next = NULL;
    return newNode;
}

/**
 * Add data to the desired list, according to it's type.
 * @param list to add the node to
 * @param data of the node
 * @param type of the node
 */
int add_code(SentenceList* list, void* data, SentenceType type) {
    SenNode* newNode = create_sentence(data, type);
    if (!newNode) return 1; /* Memory allocation failure */
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        SenNode* current = list->head;
        while (current->next != NULL) {
            current = (SenNode*) current->next;
        }
        current->next = (struct SenNode*) newNode;
    }
    /* save the size of the code */
    if(type == INSTRUCTION){
        list->size += newNode->sentence.instruction->size;
    } else if (type == COMMAND) {
        list->size += newNode->sentence.command->word_count;
    }
    return 0;
}

/**
 * Merge two lists
 * @param list1 head of the merged list
 * @param list2 to be merged
 */
void merge_lists(SentenceList* list1, SentenceList* list2) {
    SenNode* current;
    if (list1 == NULL) {
        /* If list1 is empty, it's head is list2 */
        list1 = list2;
        return;
    }

    current = (SenNode*) list1->head;
    while (current->next != NULL) {
        current =(SenNode*) current->next;
    }
    current->next = (struct SenNode*) list2->head;
    list1->size += list2->size;
}

/**
 * Free the list
 * @param list to be freed.
 */
void free_sentence_list(SentenceList* list) {
    SenNode* current = list->head;
    SenNode* nextNode;

    while (current != NULL) {
        nextNode = (SenNode*) current->next;
        free(current);
        current = nextNode;
    }

    list->head = NULL;
    list->size = 0;
    free(list);
}

void printSentenceList(SenNode* head) {
    SenNode* current = head;
    int i = 0;
    size_t j;

    while (current != NULL) {
        printf("Node %d:\n", i);

        if (current->type == INSTRUCTION) {
            InstructionSentence* is = current->sentence.instruction;
            printf("  Type: INSTRUCTION\n");
            printf("  Label: %s\n", is->label);
            printf("  Data: ");
            for (j = 0; j < is->size; j++) {
                printf("%d ", is->data[j]);
            }
            printf("\n  Source: %d\n", is->src);
        } else if (current->type == COMMAND) {
            CommandSentence* cs = current->sentence.command;
            printf("  Type: COMMAND\n");
            printf("  Label: %s\n", cs->label);
            printf("  Operation: %d\n", cs->operation);
            printf("  Source: %s\n", cs->src);
            printf("  Destination: %s\n", cs->dest);
            printf("  Word Count: %d\n", cs->word_count);
        }

        printf("\n");
        current = (SenNode*) current->next;
        i++;
    }
}