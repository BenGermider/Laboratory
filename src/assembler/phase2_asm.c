#include "../../include/assembler/phase2_asm.h"
#include "../../include/assembler/output_manager.h"
#include "../../include/common/data_types.h"
#include "../../include/common/operands/registers.h"
#include "../../include/common/operands/integers.h"
#include "../../include/common/collections/linked_list.h"
#include "../../include/common/collections/sentence_list.h"
#include "../../include/common/consts.h"

/**
 * Get the type of the operand according to the protocol
 * @param operand string of the operand
 * @param bound of integer possible to store
 * @return the type of the operand as a number.
 */
int get_operand_type(char* operand, INT_BOUND bound){
    if(operand == NULL) {
        return -1;
    } else if (*operand == '#' && is_num_legal(operand + 1, bound)){
        return 0;
    } else if (is_reg(operand)) {
        return 3;
    } else if (*operand == '*' && is_reg(operand + 1)){
        return 2;
    } else {
        return 1;
    }
}

/**
 * Converts a command with operation to an integer
 * @param c_s command holding opcode and arguments
 * @return the command as short uint
 */
unsigned short int operation_as_num(CommandSentence* c_s){
    int op = 0;
    int padding;
    /* store the opcode */
    op |= c_s->operation << OPCODE;
    /* store source operand if there is one */
    padding = get_operand_type(c_s->src, COM_BOUND);
    if(padding >= 0){
        op |= (1 << (SOURCE + padding));
    }
    /* store destination operand if there is one */
    padding = get_operand_type(c_s->dest, COM_BOUND);
    if(padding >= 0){
        op |= (1 << (DESTINATION + padding));
    }
    /* all commands declarations are absolute, so add it to the code */
    op |= ABSOLUTE;
    return op;
}

/**
 * Returns a code representing two registers.
 * @param reg1 first register as a string to store
 * @param reg2 second register as a string to store
 * @return code representing both registers
 */
unsigned short int two_regs(char* reg1, char* reg2){
    int src, dest;
    src = get_reg(reg1);
    dest = get_reg(reg2);
    return ((src << SRC_OP) | (dest << DESTINATION) | ABSOLUTE);
}

/**
 * Converts operands given to a code
 * @param operand operand as a string
 * @param labels database of labels
 * @param externals database of external labels
 * @param path source operand or destination operand
 * @return code of the operand
 */
unsigned short int operand_as_code(char* operand, Node** labels, Node** externals, OPERAND path){
    Node* operand_node;
    if(*operand == '#' && is_num_legal(operand + 1, COM_BOUND)){ /* Integer */
        return integer_word(operand);
    } else if(exists(*externals, operand, 0)){ /* External label */
        return 1;
    } else if (exists(*labels, operand, 0)){ /* Not external label */
        operand_node = get_node(*labels,operand);
        return ((operand_node->data->line) << DESTINATION) | RELOCATABLE;
    } else if(reg_arg(operand)){  /* Registers */
        if(path){
            return (get_reg(operand) << SRC_OP) | ABSOLUTE;
        } else {
            return (get_reg(operand) << DESTINATION) | ABSOLUTE;
        }
    }
    return 0; /* No operands */
}

/**
 * Converts the operands of the command into words to be stored in the memory.
 * @param machine_code pointer to array holding the words to be stored.
 * @param c_s pointer to a command storing the operands
 * @param labels database of labels of the code
 * @param externals database of extern labels of the code
 */
void handle_operands(unsigned short int** machine_code, CommandSentence* c_s, Node** labels, Node** externals){
    int last_index;
    if(c_s->word_count == 1) {
        /* If no operands, then no need to handle */
        return;
    }
    last_index = c_s->word_count - 1;
    if(reg_arg(c_s->src) && reg_arg(c_s->dest)){
        /* Handle two registers as args */
        (*machine_code)[last_index] = two_regs(c_s->src, c_s->dest);
        return;
    } else {
        /* Save the destination operand in the machine code array */
        (*machine_code)[last_index] = operand_as_code(c_s->dest, labels, externals, DESTINATION);
    }
    if(c_s->src){
        /* If there is source operand, save it as well */
        (*machine_code)[1] = operand_as_code(c_s->src, labels, externals, SOURCE);
    }
}

/**
 * Converts an entire command and converts it to machine code, words to be stored in memory
 * @param c_s command to store in memory
 * @param labels database of labels of the code
 * @param entries database of entry labels of the code
 * @param externals database of extern labels of the code
 * @return words as machine code to be stored in the memory.
 */
unsigned short int* get_command_code(CommandSentence* c_s, Node** labels,  Node** entries, Node** externals) {
    unsigned short int* machine_code = (unsigned short int*)calloc(c_s->word_count, sizeof(unsigned short int));
    if (!machine_code) {
        printf("FAILED TO ALLOCATE MEMORY.\n");
        return NULL;
    }

    /* store command as the first word */
    machine_code[0] = operation_as_num(c_s);

    /* handle next words (operands) */
    handle_operands(&machine_code, c_s, labels, externals);
    return machine_code;
}

/**
 * Converts an instruction into a machine code, according to the amount of data.
 * @param i_s instruction to be converted into machine code.
 * @return array of words to be stored in the memory.
 */
unsigned short int* get_instruction_code(InstructionSentence* i_s){
    int i;
    unsigned short int* machine_code = (unsigned short int*)calloc(i_s->size, sizeof(unsigned short int));
    if (!machine_code) {
        printf("FAILED TO ALLOCATE MEMORY.\n");
        return NULL;
    }
    /* Add each data to the code */
    for(i = 0; i < i_s->size; i++){
        machine_code[i] = i_s->data[i];
    }
    return machine_code;
}

/**
 * Main conversion code. Runs over the commands and instructions and converts them into integers representing
 * the code according to the given protocol.
 * @param code all operations and their information.
 * @param data all data stored in it's information.
 * @param IC number of line total in code.
 * @param labels database of labels of the code
 * @param entries database of entry labels of the code
 * @param externals database of extern labels of the code
 * @param ext_file database of extern labels to store in a file
 * @return machine code for the assembly file.
 */
unsigned short int* calc_code(SentenceList* code, SentenceList* data,  int* IC, Node** labels, Node** entries, Node** externals, Node** ext_file){
    int i, L = 0;
    SenNode* current_line;
    unsigned short int* translation = NULL;
    unsigned short int* machine_code;
    /* Merge lists, first code then data, according to the protocol */

    merge_lists(code, data);
    machine_code = (unsigned short int*)calloc(code->size, sizeof(unsigned short int));

    /* TODO: FREE DATA INSTEAD OF JUST EXITING */
    if(!machine_code){ printf("FAILED TO ALLOCATE MEMORY.\n"); exit(1); }
    current_line = code->head;

    while (current_line != NULL && *IC + data->size <= LAST_ADDRESS - FIRST_ADDRESS) {
        /* Iterate over the lines */
        if (current_line->type == COMMAND) {
            /* Convert the command into words and store them in the machine code array. */
            translation = get_command_code(current_line->sentence.command, labels, entries, externals);

            if (translation != NULL) {
                for (i = 0; i < current_line->sentence.command->word_count; i++) {
                    if(translation[i] == 1){
                        /* Store external labels if there are in the database for the extern file TODO: FIND IF POSSIBLE TO USE EXTERNALS*/
                        if(exists(*externals, current_line->sentence.command->src, 0)){
                            append(ext_file, L + FIRST_ADDRESS, current_line->sentence.command->src);
                        } else if (exists(*externals, current_line->sentence.command->dest, 0)){
                            append(ext_file, L + FIRST_ADDRESS, current_line->sentence.command->dest);
                        }
                    }
                    machine_code[L] = translation[i];
                    (*IC)++;
                    L++;
                }
                free(translation);
            }
        } else if (current_line->type == INSTRUCTION) {
            /* Get the code for the instruction */
            translation = get_instruction_code(current_line->sentence.instruction);
            if (translation != NULL) {
                for (i = 0; i < current_line->sentence.instruction->size; i++) {
                    machine_code[L] = translation[i];
                    L++;

                }
                free(translation);
            }
        } else {
            /* ERROR */
            free(machine_code);
            return NULL;
        }
        current_line = (SenNode *) current_line->next;
    }
    return machine_code;
}

/**
 * Main second pass function. Converts the assembly code into a machine code and saves the desired data in the
 * files, according to the protocol.
 * @param file_name file to read and store in desired files (.ob, .ext and .ent)
 * @param labels database of labels of the code
 * @param entries database of entry labels of the code
 * @param externals database of extern labels of the code
 * @param ext_file database of extern labels to store in the file
 * @param code commands of the assembly file
 * @param data data to store of the assembly file
 * @return code of success or failure
 */
int second_pass(
        const char* file_name,
        Node** labels,
        Node** entries,
        Node** externals,
        Node** ext_file,
        SentenceList* code,
        SentenceList* data
        ){
    int IC = 0;
    /* Get the machine code */
    printf("I GOT HERE");
    unsigned short int* machine_code = calc_code(code, data, &IC, labels, entries, externals, ext_file);
    printf("I OUT OF HERE");

    if(machine_code == NULL){
        printf("GAY");
        return 1;
    }

    /* Write the desired files*/

    write_obj_file(file_name, machine_code, IC, data->size);
    /*write_entry_file(file_name, labels, entries);
    write_extern_file(file_name, ext_file);*/

    free(machine_code);
    return 0;
}