#include <stdio.h>
#include "../../include/common/errors.h"



void exception(Node* error){
    printf("[EXCEPTION] Encountered exception in line %d: %s.\n", error->data->line, error->data->text);
}

void critical_error(Node* error){
    printf("[ERROR] %s\n", error->data->text);
}