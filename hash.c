#include "hash_table.h"

int main(int argc, char* argv[]){
    HashTable *hash = init_hash();
    run(argc, argv, hash);
    free(hash);
    return 0;
}