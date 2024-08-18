#ifndef LABORATORY_INIT_H
#define LABORATORY_INIT_H

#include "collections/sentence_list.h"

 /*Initializes an instruction*/
void declare_sentence(InstructionSentence*);

/*Initializes a command object*/
void generate_command(CommandSentence*);

/*Initializes lists*/
void declare_lists(SentenceList*, SentenceList*);

#endif /* LABORATORY_INIT_H */
