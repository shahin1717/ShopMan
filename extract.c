#include <stdio.h> 
#include <string.h> 

char* extract(char *line);
 
int main() { 
    char line[] = "12.4: TITLE";
    printf("%s", extract(line));
    return 0; 
} 

char* extract(char *line){
    
    const char delimiter[] = ": "; 
 
    char *token = strtok(line, delimiter); 
 
    if (token != NULL) { 
        // printf("%s\n", token); 
        return (strtok(NULL, delimiter)); 

    } 

    return NULL;
}