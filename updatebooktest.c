#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define data structure for books
typedef struct {
    int id;
    char title[100];
    char author[100];
    char genre[50];
    float price;
    int quantity_sale;
    int quantity_rent;
} Book;

void writeBookIntoFile(FILE *file, Book book, int index) {
    fputs("#\n", file); // separator between books
    fprintf(file, "%d: %s\n", index, book.title);
    fprintf(file, "%d.1: %s\n", index, book.author);
    fprintf(file, "%d.2: %s\n", index, book.genre);
    fprintf(file, "%d.3: %.2f\n", index, book.price);
    fprintf(file, "%d.4: %d\n", index, book.quantity_sale);
    fprintf(file, "%d.5: %d\n", index, book.quantity_rent);
}

char* extractStringfromLine(char *line) {
    const char delimiter[] = ": ";
    char *token = strtok(line, delimiter);
    if (token != NULL) {
        return strtok(NULL, delimiter);

    }
    return NULL;
}

void makeChanges(char *file, int index, Book book) {
    FILE *temp = fopen("temp.txt", "w");
    FILE *initial = fopen(file, "r");
    if (temp == NULL || initial == NULL) {
        perror("Error opening file");
        return;
    }

    char buffer[255];
    int i = 0;
    while (fgets(buffer, sizeof(buffer), initial) != NULL) {
        if (strcmp(buffer, "#\n") == 0) {
            i++;
            if (i >= index) {
                break;
            }
        }
        fprintf(temp, "%s", buffer);
    }

    writeBookIntoFile(temp, book, index);

    for (int k = 0; k < 6; k++) {
        if (fgets(buffer, sizeof(buffer), initial) == NULL) {
            break; // Exit loop if end of file is reached prematurely
        }
    }

    while (fgets(buffer, sizeof(buffer), initial) != NULL) {
        fprintf(temp, "%s", buffer);
    }

    fclose(temp);
    fclose(initial);

    FILE *updated = fopen(file, "w");
    FILE *tempp = fopen("temp.txt", "r");
    if (updated == NULL || tempp == NULL) {
        perror("Error opening file");
        return;
    }

    while (fgets(buffer, sizeof(buffer), tempp) != NULL) {
        fprintf(updated, "%s", buffer);
    }

    fclose(updated);
    fclose(tempp);

    remove("temp.txt");
}

// Function to update(change) book details
void updateBook(char *filename) {
    char searchTitle[100];
    printf("Enter the title of the book to update: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int found = 0;
    int index = 0; // which book is it
    char *foundtitle;
    Book temp;

    char buffer[255];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strcmp(buffer, "#\n") == 0) {
            index++;

            fgets(buffer, sizeof(buffer), file); // Read the title line
            searchTitle[strcspn(searchTitle, "\n")] = '\0';
            char *foundtitle = extractStringfromLine(buffer);
            foundtitle[strcspn(foundtitle, "\n")] = '\0';


            printf("found:%s, search: %s", foundtitle,searchTitle);
            if (foundtitle != NULL && strcmp(foundtitle, searchTitle) == 0) {
                printf("found:%s, search: %s", foundtitle,searchTitle);
                found = 1;
                fflush(stdin);
                strcpy(temp.title, searchTitle);
                // Prompt the user to update book details
                printf("Enter the new author: ");
                fgets(temp.author, sizeof(temp.author), stdin);
                temp.author[strcspn(temp.author, "\n")] = '\0'; // Remove trailing newline character

                printf("Enter the new genre: ");
                fgets(temp.genre, sizeof(temp.genre), stdin);
                temp.genre[strcspn(temp.genre, "\n")] = '\0'; // Remove trailing newline character

                printf("Enter the new price: ");
                scanf("%f", &temp.price);

                printf("Enter the new quantity for sale: ");
                scanf("%d", &temp.quantity_sale);

                printf("Enter the new quantity for rent: ");
                scanf("%d", &temp.quantity_rent);
                fclose(file);

                makeChanges(filename, index, temp);

                printf("Book details updated successfully.\n");
                return; // Exit the function after updating the book
            }
        }
    }

    fclose(file);
    if (!found) {
        printf("Book not found.\n");
    }
}


int main() {
    updateBook("inventory.txt");
    return 0;
}
