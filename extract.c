#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* extractStringfromLine(const char* line);

int main(){
    printf("%s", extractStringfromLine("12.45: hello loxes"));
    return 0;
}

char* extractStringfromLine(const char *line){
    char *token;

    // Get the first token
    token = strtok((char*)line, ": ");

    // Allocate memory for the token
    char *result = NULL;
    if (token != NULL) {
        result = malloc(strlen(token) + 1); // +1 for null terminator
        if (result != NULL) {
            strcpy(result, token);
        }
    }

    // Traverse through other tokens
    while (token != NULL) {
        token = strtok(NULL, ": "); // Passing NULL tells strtok to continue from the previous string
        if (token != NULL) {
            // Reallocate memory to include the new token
            result = realloc(result, strlen(result) + strlen(token) + 2); // +2 for colon and null terminator
            if (result != NULL) {
                strcat(result, ":"); // Add colon
                strcat(result, token); // Add token
            } else {
                // Memory allocation failed, free the previously allocated memory
                free(result);
                return NULL;
            }
        }
    }

    return result;
}
