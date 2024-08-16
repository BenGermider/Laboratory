#include <stdio.h>
#include "../../include/common/errors.h"


/**
 * Prints error in code
 * @param error
 */
void exception(Node* error){
    printf("[EXCEPTION] Encountered exception in line %d: %s.\n", error->data->line, error->data->text);
}

/**
 * Prints fatal errors leading termination of the project.
 * @param error
 */
void critical_error(Node* error){
    printf("[ERROR] %s\n", error->data->text);
}