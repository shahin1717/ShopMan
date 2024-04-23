#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define data structure for books
typedef struct {
    char title[100];
    char author[100];
    char genre[50];
    float price;
    int quantity_sale;
    int quantity_rent;
} Book;


// Function to add new books to the inventory
void addBook(Book *inventory, int *numBooks) {
    // Prompt the user to enter book details
    printf("Enter the title: ");
    fgets(inventory[*numBooks].title, sizeof(inventory[*numBooks].title), stdin);
    inventory[*numBooks].title[strcspn(inventory[*numBooks].title, "\n")] = '\0'; // Remove trailing newline character

    printf("Enter the author: ");
    fgets(inventory[*numBooks].author, sizeof(inventory[*numBooks].author), stdin);
    inventory[*numBooks].author[strcspn(inventory[*numBooks].author, "\n")] = '\0'; // Remove trailing newline character

    printf("Enter the genre: ");
    fgets(inventory[*numBooks].genre, sizeof(inventory[*numBooks].genre), stdin);
    inventory[*numBooks].genre[strcspn(inventory[*numBooks].genre, "\n")] = '\0'; // Remove trailing newline character

    printf("Enter the price: ");
    scanf("%f", &inventory[*numBooks].price);

    printf("Enter the quantity for sale: ");
    scanf("%d", &inventory[*numBooks].quantity_sale);

    printf("Enter the quantity for rent: ");
    scanf("%d", &inventory[*numBooks].quantity_rent);

    (*numBooks)++; // Increment the number of books in the inventory

    printf("Book added successfully.\n");
}
    // TODO: Implement the logic to add a new book to the inventory

// Function to update book details
void updateBook(Book *inventory, int numBooks) {
    char searchTitle[100];
    int found = 0;

    printf("Enter the title of the book to update: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    // Search for the book in the inventory
    for (int i = 0; i < numBooks; i++) {
        if (strcmp(inventory[i].title, searchTitle) == 0) {
            found = 1;

            // Prompt the user to update book details
            printf("Enter the new author: ");
            fgets(inventory[i].author, sizeof(inventory[i].author), stdin);
            inventory[i].author[strcspn(inventory[i].author, "\n")] = '\0'; // Remove trailing newline character

            printf("Enter the new genre: ");
            fgets(inventory[i].genre, sizeof(inventory[i].genre), stdin);
            inventory[i].genre[strcspn(inventory[i].genre, "\n")] = '\0'; // Remove trailing newline character

            printf("Enter the new price: ");
            scanf("%f", &inventory[i].price);

            printf("Enter the new quantity for sale: ");
            scanf("%d", &inventory[i].quantity_sale);

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
    // TODO: Implement the logic to update book details


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
    printf("9. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    Book inventory[100]; // Assuming a maximum of 100 books in the inventory
    int numBooks = 0;
    int choice;

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Consume the newline character

        switch (choice) {
            case 1:
                addBook(inventory, &numBooks);
                break;
            case 2:
                updateBook(inventory, numBooks);
                break;
            case 3:
                getBookInfo(inventory, numBooks);
                break;
            case 4:
                processSale(inventory, numBooks);
                break;
            case 5:
                printf("Total amount: %.2f\n", calculateTotalAmount(inventory, numBooks));
                break;
            case 6:
                rentBook(inventory, numBooks);
                break;
            case 7:
                generateSalesReport(inventory, numBooks);
                break;
            case 8:
                generateRentalReport(inventory, numBooks);
                break;
            case 9:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 9);

    return 0;
}