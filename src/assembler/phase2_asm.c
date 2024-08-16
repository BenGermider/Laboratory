#include "../../include/assembler/phase2_asm.h"
#include "../../include/assembler/output_manager.h"
#include "../../include/common/data_types.h"
#include "../../include/common/operands/registers.h"
#include "../../include/common/operands/integers.h"
#include "../../include/common/collections/linked_list.h"
#include "../../include/common/collections/sentence_list.h"
#include "../../include/common/consts.h"

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

unsigned short int operation_as_num(CommandSentence* c_s){
    int op = 0;
    int padding;
    op |= c_s->operation << OPCODE;
    padding = get_operand_type(c_s->src, COM_BOUND);
    if(padding >= 0){
        op |= (1 << (SOURCE + padding));
    }
    padding = get_operand_type(c_s->dest, COM_BOUND);
    if(padding >= 0){
        op |= (1 << (DESTINATION + padding));
    }
    op |= ABSOLUTE;
    return op;
}

unsigned short int two_regs(char* reg1, char* reg2){
    int src, dest;
    src = get_reg(reg1);
    dest = get_reg(reg2);
    return ((src << SRC_OP) | (dest << DESTINATION) | ABSOLUTE);
}

unsigned short int operand_as_code(char* operand, Node** labels, Node** externals, OPERAND path){
    Node* operand_node;
    if(*operand == '#' && is_num_legal(operand + 1, COM_BOUND)){
        return integer_word(operand);
    } else if(exists(*externals, operand, 0)){
        return 1;
    } else if (exists(*labels, operand, 0)){
        operand_node = get_node(*labels,operand);
        return ((operand_node->data->line) << DESTINATION) | RELOCATABLE;
    } else if(reg_arg(operand)){
        if(path){
            return (get_reg(operand) << SRC_OP) | ABSOLUTE;
        } else {
            return (get_reg(operand) << DESTINATION) | ABSOLUTE;
        }
    }
    return 0;
}

void handle_operands(unsigned short int** machine_code, CommandSentence* c_s, Node** labels, Node** externals){
    int last_index;
    if(c_s->word_count == 1) {
        return;
    }
    last_index = c_s->word_count - 1;
    if(reg_arg(c_s->src) && reg_arg(c_s->dest)){
        (*machine_code)[last_index] = two_regs(c_s->src, c_s->dest);
        return;
    } else {
        (*machine_code)[last_index] = operand_as_code(c_s->dest, labels, externals, DESTINATION);
    }
    if(c_s->src){
        (*machine_code)[1] = operand_as_code(c_s->src, labels, externals, SOURCE);
    }
}

unsigned short int* get_command_code(CommandSentence* c_s, Node** labels,  Node** entries, Node** externals) {
    unsigned short int* machine_code = (unsigned short int*)calloc(c_s->word_count, sizeof(unsigned short int));
    if (!machine_code) {
        printf("FAILED TO ALLOCATE MEMORY.\n");
        return NULL;
    }
    machine_code[0] = operation_as_num(c_s);
    handle_operands(&machine_code, c_s, labels, externals);
    return machine_code;
}

unsigned short int* get_instruction_code(InstructionSentence* i_s){
    int i;
    unsigned short int* machine_code = (unsigned short int*)calloc(i_s->size, sizeof(unsigned short int));
    if (!machine_code) {
        printf("FAILED TO ALLOCATE MEMORY.\n");
        return NULL;
    }
    for(i = 0; i < i_s->size; i++){
        machine_code[i] = i_s->data[i];
    }
    return machine_code;
}

unsigned short int* calc_code(SentenceList* code, SentenceList* data,  int* IC, Node** labels, Node** entries, Node** externals, Node** ext_file){
    int i, L = 0;
    SenNode* current_line;
    unsigned short int* translation = NULL;
    unsigned short int* machine_code;
    merge_lists(code, data);
    machine_code = (unsigned short int*)calloc(code->size, sizeof(unsigned short int));
    if(!machine_code){ printf("FAILED TO ALLOCATE MEMORY.\n"); exit(1); }
    current_line = code->head;
    while (current_line != NULL && *IC + data->size <= LAST_ADDRESS - FIRST_ADDRESS) {
        if (current_line->type == COMMAND) {
            translation = get_command_code(current_line->sentence.command, labels, entries, externals);
            if (translation != NULL) {
                for (i = 0; i < current_line->sentence.command->word_count; i++) {
                    if(translation[i] == 1){
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
            translation = get_instruction_code(current_line->sentence.instruction);
            if (translation != NULL) {
                for (i = 0; i < current_line->sentence.instruction->size; i++) {
                    machine_code[L] = translation[i];
                    L++;

                }
                free(translation);
            }
        } else {
            free(machine_code);
            return NULL;
        }
        current_line = (SenNode *) current_line->next;
    }
    return machine_code;
}

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
    unsigned short int* machine_code = calc_code(code, data, &IC, labels, entries, externals, ext_file);

    if(machine_code == NULL){
        printf("GAY");
        return 1;
    }
    write_obj_file(file_name, machine_code, IC, data->size);
    write_entry_file(file_name, labels, entries);
    write_extern_file(file_name, ext_file);
    free(machine_code);
    return 0;
}