#include <stdlib.h>
#include "../../include/assembler/pre_assembler.h"
#include "../../include/assembler/phase1_asm.h"
#include "../../include/assembler/phase2_asm.h"

/**
 * Assembler main function, runs second pass if first pass runs successfully.
 * @param file_name file to convert to machine code
 * @param macros macros found and saved in .as file
 * @return code of success or fail.
 */
int assembler(const char* file_name, HashTable* macros){
    char* src_file_name;
    FILE *assembly;
    Node* externals = NULL;
    Node* ext_file = NULL;
    Node* entries = NULL;
    Node* labels = NULL;
    SentenceList* code = NULL;
    SentenceList* data = NULL;

    /* Get the name of the file with the suffix */
    get_file(file_name, &src_file_name, ".am");

    assembly = fopen(src_file_name, "r");
    if (assembly == NULL) {
        printf("[ERROR] Failed to open file %s\n", src_file_name);
        free(src_file_name);
        return 1;
    }

    /* Declare data structure that stores all types rows */
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

    /* Executed first pass and stops if exceptions occurred */
    if(first_pass(assembly, &labels, &externals, &entries, code, data, macros)){
        return 1;
    }

    /* Executed second pass and stops if exceptions occurred */
    if(second_pass(file_name, &labels, &entries, &externals, &ext_file, code, data)){
        return 1;
    }

    /* Free dynamically allocated data */
    freeSentenceList(code);
    free(code);

    return 0;
}


int main(int argc, char* argv[]) {
    /* Database of macros declaration */
    HashTable* macros = createHashTable();

    while (--argc > 0) {
        printf("START PART 1");
        /* Converts an assembly file into machine code file but stops if errors occurred,
         * for every file given as an argument */
        if (pre_assembler(argv[argc], macros)) {
            continue;
        }
        printf("FINISHED PART 1");
        if (assembler(argv[argc], macros)) {
            continue;
        }
        printf("FINISHED PART 2");
    }

    /* Free database of macros */
    freeHashTable(macros);

    return 0;
}