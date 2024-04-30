char* extractStringFromLine(char *line){
    
    const char delimiter[] = ": ";

    char *token = strtok(line, delimiter);
    if (token != NULL) {
        return strtok(NULL, delimiter);

    }
    return NULL;
}