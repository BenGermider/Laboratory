#include <stdlib.h>
#include "../../include/assembler/pre_assembler.h"
#include "../../include/assembler/phase1_asm.h"
#include "../../include/assembler/phase2_asm.h"

int assembler(const char* file_name){
    char* src_file_name;
    FILE *assembly;
    Node* externals = NULL;
    Node* ext_file = NULL;
    Node* entries = NULL;
    Node* labels = NULL;
    SentenceList* code = NULL;
    SentenceList* data = NULL;

    get_file(file_name, &src_file_name, ".am");

    assembly = fopen(src_file_name, "r");
    if (assembly == NULL) {
        printf("[ERROR] Failed to open file %s\n", src_file_name);
        free(src_file_name);
        return 1;
    }

    code = (SentenceList*)malloc(sizeof(SentenceList));
    if (code == NULL) {
        printf("[ERROR] Failed to allocate memory for SentenceList\n");
        return 1;
    }
    data = (SentenceList*)malloc(sizeof(SentenceList));
    if (data == NULL) {
        free(code);
        printf("[ERROR] Failed to allocate memory for SentenceList\n");
        return 1;
    }

    declare_lists(code, data);
    if(first_pass(assembly, &labels, &externals, &entries, code, data)){
        return 1;
    }
    if(second_pass(file_name, &labels, &entries, &externals, &ext_file, code, data)){
        return 1;
    }
    freeSentenceList(code);
    free(code);

    return 0;
}


int main(int argc, char* argv[]) {

    while (--argc > 0) {
        printf("Start pre-proc\n");
        if (!pre_assembler(argv[argc])) {
            continue;
        }
        printf("Start first pass\n");
        /*Execute the first pass, and then the second on the ".am" file.*/
        if (assembler(argv[argc])) {
            continue;
        }
    }
    return 0;
}