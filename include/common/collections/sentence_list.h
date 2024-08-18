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
SenNode* create_sentence(void* data, SentenceType type);

/* Add a new node into the list */
int add_code(SentenceList* list, void* data, SentenceType type);

/* Merge two lists into one */
void merge_lists(SentenceList* list1, SentenceList* list2);

/* Free the list */
void free_sentence_list(SentenceList*);

void printSentenceList(SenNode*);

#endif /* LABORATORY_SENTENCE_LIST_H */
