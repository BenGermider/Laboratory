#include "../../include/common/init.h"


/**
 * Initializes an instruction
 * @param sen instruction to initialize
 */
void declare_sentence(InstructionSentence *sen){
    sen->label = NULL;
    sen->data = NULL;
    sen->src = 0;
    sen->size = 0;
    sen->pos = 0;
}

/**
 * Initializes a command object
 * @param c_s command to initialize.
 */
void generate_command(CommandSentence *c_s){
    c_s->label = NULL;
    c_s->pos = 0;
    c_s->src = NULL;
    c_s->dest = NULL;
    c_s->operation = -1;
    c_s->word_count = 0;
}

/**
 * Initializes lists
 * @param code list holds commands
 * @param data list holds instructions
 */
void declare_lists(SentenceList* code, SentenceList* data){
    code->head = NULL;
    code->size = 0;
    data->head = NULL;
    data->size = 0;
}
