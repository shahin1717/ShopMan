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


// writes Book structure in the end of the inventory file
void writeBookIntoFile(FILE *file, Book book,int index){

    fputs("#\n", file); // separator between books
    fprintf(file,"%d: %s\n",index,book.title);
    fprintf(file,"%d.1: %s\n",index,book.author);
    fprintf(file,"%d.2: %s\n",index,book.genre);
    fprintf(file,"%d.3: %0.2f\n",index,book.price);
    fprintf(file,"%d.4: %d\n",index,book.quantity_sale);
    fprintf(file,"%d.5: %d\n",index,book.quantity_rent);

}


// Function to add new books to the inventory
void addBook(char *fileName,int *numBooks) {
    int index = *numBooks + 1;
    // temporary book to store user input
    Book new;
    // Prompt the user to enter book details
    printf("Enter the title: ");
    fgets(new.title, sizeof(new.title), stdin);
    new.title[strcspn(new.title, "\n")] = '\0'; // Remove trailing newline character

    printf("Enter the author: ");
    fgets(new.author, sizeof(new.author), stdin);
    new.author[strcspn(new.author, "\n")] = '\0'; // Remove trailing newline character

    printf("Enter the genre: ");
    fgets(new.genre, sizeof(new.genre), stdin);
    new.genre[strcspn(new.genre, "\n")] = '\0'; // Remove trailing newline character

    
    float price = -1;
    while (price = -1){
        printf("Enter the price: ");
        scanf(" %f", &new.price);
    }


    // TODO: catch error here
    printf("Enter the quantity for sale: ");
    scanf("%d", &new.quantity_sale);

    // TODO: catch error here
    printf("Enter the quantity for rent: ");
    scanf("%d", &new.quantity_rent);

    

    FILE * file = fopen("inventory.txt","a");
    writeBookIntoFile(file, new, index);
    fclose(file);

    (*numBooks)++; // Increment the number of books in the inventory
    printf("Book added successfully.\n");
}


// returns TITLE from 1: TITLE
//         AUTHOR from 1.1: AUTHOR
char* extractStringFromLine(char *line){
    
    char *sepPosition = strchr(line, ':');
    if (sepPosition != NULL) {
        // Move the pointer after the colon to get the title
        char *titleStart = sepPosition + 1;

        // Remove leading and trailing spaces
        while (*titleStart == ' ' || *titleStart == '\t') {
            titleStart++;
        }

        // Find the end of the title
        char *titleEnd = strchr(titleStart, '\n');
        if (titleEnd != NULL) {
            *titleEnd = '\0'; // Null-terminate the title
        }
        return titleStart;
        printf("Clean title: %s\n", titleStart);
    }

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
            char *foundtitle = extractStringFromLine(buffer);
            foundtitle[strcspn(foundtitle, "\n")] = '\0';

            if (foundtitle != NULL && strcmp(foundtitle, searchTitle) == 0) {

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



// Function to retrieve book information
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

// Function to process book sales transactions
void processSale(Book *inventory, int numBooks) {
    char searchTitle[100];
    int found = 0;

    printf("Enter the title of the book to process sale: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    // Search for the book in the inventory
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(inventory[i].title, searchTitle) == 0) {
            found = 1;

            int quantity;
            printf("Enter the quantity to sell: ");
            scanf("%d", &quantity);

            // Check if there are enough books in stock
            if (inventory[i].quantity_sale >= quantity) {
                inventory[i].quantity_sale -= quantity;
                printf("Sale processed successfully.\n");
            } else {
                printf("Insufficient quantity in stock.\n");
            }

            break;
        }
    }

    if (!found) {
        printf("Book not found in the inventory.\n");
    }
}

float calculateTotalAmount(Book *inventory, int numBooks) {
    char searchTitle[100];
    int found = 0;
    float totalAmount = 0.0;

    printf("Enter the title of the book to calculate total amount: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    // Search for the book in the inventory
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(inventory[i].title, searchTitle) == 0) {
            found = 1;

            int quantity;
            printf("Enter the quantity to calculate total amount: ");
            scanf("%d", &quantity);

            // Calculate total amount payable
            totalAmount = inventory[i].price * quantity;
            break;
        }
    }

    if (!found) {
        printf("Book not found in the inventory.\n");
    }

    return totalAmount;
}

void rentBook(Book *inventory, int numBooks) {
    char searchTitle[100];
    int found = 0;

    printf("Enter the title of the book to rent: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    // Search for the book in the inventory
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(inventory[i].title, searchTitle) == 0) {
            found = 1;

            int quantity;
            printf("Enter the quantity to rent: ");
            scanf("%d", &quantity);

            // Check if there are enough books in stock
            if (inventory[i].quantity_rent >= quantity) {
                inventory[i].quantity_rent -= quantity;
                printf("Book rented successfully.\n");
            } else {
                printf("Insufficient quantity in stock.\n");
            }

            break;
        }
    }

    if (!found) {
        printf("Book not found in the inventory.\n");
    }
}

// bring back the book (opposite of renting)

void generateSalesReport(Book *inventory, int numBooks) {
    // Display sales report header
    printf("Sales Report\n");
    printf("------------\n");

    // Iterate over the inventory and display sales information for each book
    for (int i = 0; i < numBooks; i++) {
        printf("Title: %s\n", inventory[i].title);
        printf("Author: %s\n", inventory[i].author);
        printf("Quantity sold: %d\n", inventory[i].quantity_sale);
        printf("Total sales amount: %.2f\n", inventory[i].price * inventory[i].quantity_sale);
        printf("------------\n");
    }
}

void generateRentalReport(Book *inventory, int numBooks) {
    // Display rental report header
    printf("Rental Report\n");
    printf("-------------\n");

    // Iterate over the inventory and display rental information for each book
    for (int i = 0; i < numBooks; i++) {
        printf("Title: %s\n", inventory[i].title);
        printf("Author: %s\n", inventory[i].author);
        printf("Quantity rented: %d\n", inventory[i].quantity_rent);
        printf("-------------\n");
    }
}

void displayMenu() {
    printf("Main Menu\n");
    printf("---------\n");
    printf("1. Add Book\n");
    printf("2. Update Book\n");
    printf("3. Get Book Information\n");
    printf("4. Process Sale\n");
    printf("5. Calculate Total Amount\n");
    printf("6. Rent Book\n");
    printf("7. Generate Sales Report\n");
    printf("8. Generate Rental Report\n");
    // browse
    printf("9. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    // not absolute address
    char *fileName = "inventory.txt";

    // Book inventory[100]; // Assuming a maximum of 100 books in the inventory

    // Counts the initial number of books(line in a file) in inventory before starting the operations
    int numBooks = 0;

    FILE *inventoryFile = fopen("inventory.txt","r+"); // r+ for both reading and writing
    if (inventoryFile == NULL) {
        perror("Error opening file");
        return 1;
    }
    char buffer[255];
    while(fgets(buffer, 255, inventoryFile) != NULL){
        if (strcmp(buffer, "#\n") == 0){
            numBooks ++;
        } 
    }
    fclose(inventoryFile);

    int choice;
    do {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Consume the newline character

        switch (choice) {
            case 1:
                addBook(fileName, &numBooks);
                break;
            case 2:
                updateBook("inventory.txt");
                break;
            // case 3:
            //     getBookInfo(inventory, numBooks);
            //     break;
            // case 4:
            //     processSale(inventory, numBooks);
            //     break;
            // case 5:
            //     printf("Total amount: %.2f\n", calculateTotalAmount(inventory, numBooks));
            //     break;
            // case 6:
            //     rentBook(inventory, numBooks);
            //     break;
            // case 7:
            //     generateSalesReport(inventory, numBooks);
            //     break;
            // case 8:
            //     generateRentalReport(inventory, numBooks);
            //     break;
            // browse
            case 9:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

        fflush(stdin); // after user inut a number they also input \n, get rid of it

    } while (choice != 9);
  
    return 0;
}