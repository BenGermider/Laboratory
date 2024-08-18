#ifndef LABORATORY_INIT_H
#define LABORATORY_INIT_H

#include "collections/sentence_list.h"

/**
 * Initializes an instruction
 * @param sen instruction to initialize
 */
void declare_sentence(InstructionSentence*);
/**
 * Initializes a command object
 * @param c_s command to initialize.
 */
void generate_command(CommandSentence*);

/**
 * Initializes lists
 * @param code list holds commands
 * @param data list holds instructions
 */
void declare_lists(SentenceList*, SentenceList*);

#endif /* LABORATORY_INIT_H */
