#include <stdio.h>
#include "file_handler.h"
#include "linked_list.h"

int main(int argc, char* argv[]){
    Node *node = create_node("HELLO");
    add_to_tail(&node, "HELLO");
    printf("%s\n", node->file_name);
    return 0;
}