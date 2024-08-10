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

SenNode* createNode(void* data, SentenceType type);

void add_code(SentenceList* list, void* data, SentenceType type);

void merge_lists(SentenceList* list1, SentenceList* list2);

void printSentenceList(SenNode* head);

void freeSentenceList(SentenceList*);

#endif /* LABORATORY_SENTENCE_LIST_H */
