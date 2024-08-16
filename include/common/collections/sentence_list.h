#ifndef LABORATORY_SENTENCE_LIST_H
#define LABORATORY_SENTENCE_LIST_H

#include "../data_types.h"

typedef struct {
    Sentence sentence;
    SentenceType type;
    struct SenNode* next;
} SenNode;

typedef struct {
    SenNode* head;
    size_t size;
} SentenceList;

/* Creates a node by the type of data it stores */
SenNode* createNode(void* data, SentenceType type);

/* Add a new node into the list */
int add_code(SentenceList* list, void* data, SentenceType type);

/* Merge two lists into one */
void merge_lists(SentenceList* list1, SentenceList* list2);

/* Prints a list of code TODO: Remove it is for debugging */
void printSentenceList(SenNode* head);

/* Free the list */
void freeSentenceList(SentenceList*);

#endif /* LABORATORY_SENTENCE_LIST_H */
