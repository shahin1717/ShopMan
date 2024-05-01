char* extractStringFromLine(char *line){
    
    const char delimiter[] = ": ";

    char *token = strtok(line, delimiter);
    if (token != NULL) {
        return strtok(NULL, delimiter);

    }
    return NULL;
}


void getBookInfo(Book *inventory, int numBooks) {
    char searchTitle[100];
    int found = 0;

    printf("Enter the title of the book to retrieve information: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    // Search for the book in the inventory
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(inventory[i].title, searchTitle) == 0) {
            found = 1;

            // Display book information
            printf("Title: %s\n", inventory[i].title);
            printf("Author: %s\n", inventory[i].author);
            printf("Genre: %s\n", inventory[i].genre);
            printf("Price: %.2f\n", inventory[i].price);
            printf("Quantity for sale: %d\n", inventory[i].quantity_sale);
            printf("Quantity for rent: %d\n", inventory[i].quantity_rent);
            break;
        }
    }

    if (!found) {
        printf("Book not found in the inventory.\n");
    }
}