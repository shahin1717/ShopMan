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
    
    fprintf(file,"%d: %s\n",index,book.title);
    fprintf(file,"%d.1: %s\n",index,book.author);
    fprintf(file,"%d.2: %s\n",index,book.genre);
    fprintf(file,"%d.3: %0.2f\n",index,book.price);
    fprintf(file,"%d.4: %d\n",index,book.quantity_sale);
    fprintf(file,"%d.5: %d\n",index,book.quantity_rent);
    fputs("#\n", file); // separator between books

}


// Function to add new books to the inventory
void addBook(char *fileName,int *numBooks) {
    int index = *numBooks + 1;
    // temporary
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

    // TODO: catch error here
    printf("Enter the price: ");
    scanf("%f", &new.price);

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


char *extractStringfromLine(char *line)[
    char *token;

    // Get the first token
    token = strtok(str, ": ");

    // Traverse through other tokens
    while (token != NULL) {
        token = strtok(NULL, ": "); // Passing NULL tells strtok to continue from the previous string
        if (token != NULL) {
            printf("String after ': ': %s\n", token);
        }
    }
}


// Function to update(change) book details
void updateBook(char *filename, int numbooks) {

    char searchTitle[100];
    int found = 0;
    printf("Enter the title of the book to update: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    FILE *file = ("inventory.txt","r");
    // Search for the book in the inventory
    while (int i = 0; i < numbooks; i++) {
        if (strcmp(inventory[i].title, searchTitle) == 0) {
            found = 1;

            // Prompt the user to update book details
            printf("Enter the new author: ");
            fgets(inventory[i].author, sizeof(inventory[i].author), stdin);
            inventory[i].author[strcspn(inventory[i].author, "\n")] = '\0'; // Remove trailing newline character

            printf("Enter the new genre: ");
            fgets(inventory[i].genre, sizeof(inventory[i].genre), stdin);
            inventory[i].genre[strcspn(inventory[i].genre, "\n")] = '\0'; // Remove trailing newline character

            // TODO catch error when user inputs string value
            printf("Enter the new price: ");
            
            scanf("%f", &inventory[i].price);
            

            // TODO catch error when user inputs string value
            printf("Enter the new quantity for sale: ");
            scanf("%d", &inventory[i].quantity_sale);

            // TODO catch error when user inputs string value
            printf("Enter the new quantity for rent: ");
            scanf("%d", &inventory[i].quantity_rent);

            printf("Book details updated successfully.\n");
            break;
        }
    }

    if (!found) {
        printf("Book not found in the inventory.\n");
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
    char *fileName = "inventory.txt";
    Book inventory[100]; // Assuming a maximum of 100 books in the inventory
    FILE *inventoryFile = fopen("inventory.txt","r+"); // r+ for both reading and writing

    if (inventoryFile == NULL) {
        perror("Error opening file");
        return 1;
    }
   
    // Counts the initial number of books(line in a file) in inventory before starting the operations
    int numBooks = 0;
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
            // case 2:
            //     updateBook(inventoryFile, numBooks);
            //     break;
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

    char buf [255];
    while(fgets(buf, 255, inventoryFile) != NULL){
        printf("%s", buf);
    }
    

    
    return 0;
}