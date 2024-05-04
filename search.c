#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

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

typedef struct {
    char title[255];
    int total_sold;
} SoldBook;

int numBooks;

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

    // TODO: catch error here
    printf("Enter the price: ");
    scanf(" %f", &new.price);
    
    // TODO: catch error here
    printf("Enter the quantity for sale: ");
    scanf("%d", &new.quantity_sale);

    // TODO: catch error here
    printf("Enter the quantity for rent: ");
    scanf("%d", &new.quantity_rent);

    // Increment the number of books in the inventory
    (*numBooks)++;    

    FILE * file = fopen("inventory.txt","a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    writeBookIntoFile(file, new, *numBooks);
    fclose(file);

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

int searchBook(FILE* file, char *searchTitle){
    
    int found = 0;
    int index = 0; // which book is it
    char *foundtitle;

    char buffer[255];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strcmp(buffer, "#\n") == 0) {

            index++;

            fgets(buffer, sizeof(buffer), file); // Read the title line
            searchTitle[strcspn(searchTitle, "\n")] = '\0';
            char *foundtitle = extractStringFromLine(buffer);
            foundtitle[strcspn(foundtitle, "\n")] = '\0';
            
            if (foundtitle != NULL && strcmp(foundtitle, searchTitle) == 0) {
                return index;
                found = 1;
            }
        }
    }

    return -1;
}

// Function to update(change) book details
void updateBook(char *filename) {

    Book temp;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char searchTitle[100];
    printf("Enter the title of the book: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    int index = searchBook(file, searchTitle );

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

// Function to retrieve book information
void getBookInfo(char *filename, int numBooks) {
    
    char searchTitle[100];
    printf("Enter the title of the book to get information about: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    int index = searchBook(file, searchTitle);

     char buffer[255];

    printf("_________________________\n");


    printf("Title: %s\n", searchTitle);

    fgets(buffer, sizeof(buffer), file); // Read the next line (author line)
    
    char *author = extractStringFromLine(buffer);
    printf("Author: %s\n", author);

    fgets(buffer, sizeof(buffer), file); // Read the next line (genre line)
    char *genre = extractStringFromLine(buffer);
    printf("Genre: %s\n", genre);

    fgets(buffer, sizeof(buffer), file); // Read the next line (genre line)
    char *price = extractStringFromLine(buffer);
    printf("Price: %s\n", price);

    fgets(buffer, sizeof(buffer), file); // Read the next line (genre line)
    char *sale = extractStringFromLine(buffer);
    printf("Quantity for sale: %s\n", sale);

    fgets(buffer, sizeof(buffer), file); // Read the next line (genre line)
    char *rent = extractStringFromLine(buffer);
    printf("Quantity for rent: %s\n", rent);

    printf("_________________________\n");

    fclose(file);
    
}

// writes into sale file, where all transactions are stored
void generateSalesReport(char* filename, Book book, int quantity) {
    FILE *file = fopen(filename, "a");

    fprintf(file,"%s\n",book.title);
    fprintf(file,"%s\n",book.author);
    // fprintf(file,"%s\n",book.genre);
    fprintf(file,"%d\n",quantity); // Quantity sold
    fprintf(file,"%f\n",book.price * quantity);  // total price

    // in case if in future price of the book will change
    // these wont be any problem in the code, as report was generated right after selling

    fclose (file);
    
}

// Function to process book sales
void processSale(char *filename, char *sellFileName) {
    char searchTitle[100];
    char buffer[255];

    printf("Enter the title of the book to process sale: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    FILE *file = fopen(filename, "r");
    int index = searchBook(file, searchTitle);


    if (index == -1){
        printf("Book not found in the inventory.\n");
        return;
    }
    

    fgets(buffer, sizeof(buffer), file);  //author
    buffer[strcspn(buffer, "\n")] = '\0';
    char author[255];
    strcpy(author,extractStringFromLine(buffer));
    fgets(buffer, sizeof(buffer), file); //genre
    buffer[strcspn(buffer, "\n")] = '\0';
    char genre[255];
    strcpy(genre,extractStringFromLine(buffer));

    fgets(buffer, sizeof(buffer), file); // price
    buffer[strcspn(buffer, "\n")] = '\0';
    char *cprice = extractStringFromLine(buffer);
    float price = atof(cprice);

    fgets(buffer, sizeof(buffer), file); // quntity for sale
    buffer[strcspn(buffer, "\n")] = '\0';
    char *csell = extractStringFromLine(buffer);
    int sell = atof(csell) ;

    fgets(buffer, sizeof(buffer), file);
    strcpy(buffer, extractStringFromLine(buffer));
    buffer[strcspn(buffer, "\n")] = '\0';

    fclose(file);


    if (sell == 0){
        printf("Not in stock.\n");
        return;
    }

    int quantity;
    printf("Enter the quantity to sell: ");
    scanf("%d", &quantity);

    if (sell >= quantity) {
        Book temp;

        strcpy(temp.title, searchTitle);
        strcpy(temp.author, author);
        strcpy(temp.genre, genre);

        temp.price = price;
        temp.quantity_sale = sell - quantity;
        temp.quantity_rent = atof(buffer);

        makeChanges(filename,index, temp);

        generateSalesReport(sellFileName, temp, quantity);

        printf("Sale processed successfully.\n");

        // Как чек чтобы вывело плз
        printf("______чек(?)_______");
        printf("%s", searchTitle);
        printf("%s", author);
        printf("%f", price); 
        printf("%d", quantity);
        printf("%f", price*quantity); // total

    } 
    else {
        printf("Insufficient quantity in stock.\n");
        printf("There are only %d in the stock\n", sell);
        return;
    }
}

void bestSeller(char *filename, char rent){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // totally numBook in inventory
    SoldBook books[numBooks];

    int numSoldBooks = 0;

    char buffer[255];
    char title[255];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {

        // Remove trailing newline from title
        buffer[strcspn(buffer, "\n")] = '\0';
        strcpy(title, buffer);
        
        fgets(buffer, sizeof(buffer), file); // author
        fgets(buffer, sizeof(buffer), file); // quantity

        int quantity = atoi(buffer);

        fgets(buffer, sizeof(buffer), file); // revenue
        
        if(rent == 'r'){
            fgets(buffer, sizeof(buffer), file); // date
            fgets(buffer, sizeof(buffer), file); // days
        }

        // Check if the book is already in the array
        int found = 0;
        for (int i = 0; i < numSoldBooks; i++) {
            if (strcmp(books[i].title, title) == 0) {
                books[i].total_sold += quantity;
                found = 1;
                break;
            }
        }

        // If the book is not in the array, add it
        if (!found) {
            strcpy(books[numSoldBooks].title, title);
            books[numSoldBooks].total_sold = quantity;
            numSoldBooks++;
        }
    }

    fclose(file);

    int max_sold = 0;
    char best_selling_title[255];
    for (int i = 0; i < numSoldBooks; i++) {
        if (books[i].total_sold > max_sold) {
            max_sold = books[i].total_sold;
            strcpy(best_selling_title, books[i].title);
        }
    }

    printf("The best selling book is: %s\n\n", best_selling_title);
}

void displaySaleReport(char *filename){
    FILE *file = fopen(filename, "r");
    // Display sales report header
    printf("Sales Report\n");
    printf("------------\n");
    float total_rev = 0;

    char buffer[255];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("Title: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Author: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Quantity sold: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Total sales amount: %s", buffer);
        total_rev += atof(buffer);
        printf("------------\n");
    }

    fclose(file);

    printf("Total rev: %f\n", total_rev);

    // printf bestselling books
    bestSeller(filename, 's');    
}


// writes into rent file, where all transactions are stored
void generateRentalReport(char* filename, Book book, int quantity, char *date, int days) {
    FILE *file = fopen(filename, "a");

    fprintf(file,"%s\n",book.title);
    fprintf(file,"%s\n",book.author);
    // fprintf(file,"%s\n",book.genre);
    fprintf(file,"%d\n",quantity); // Quantity sold
    fprintf(file,"%f\n",book.price * quantity);  // total price
    fprintf(file,"%s\n", date);
    fprintf(file, "%d\n", days);

    // in case if in future price of the book will change
    // these wont be any problem in the code, as report was generated right after selling

    fclose (file);   
}

void processRent(char *filename, char *rentFileName) {

    // title
    // author
    // number of book that was rent
    // price
    // date that is was given
    // number of days 

    // 50% off when renting

    char searchTitle[100];
    char buffer[255];
    int BUF_LEN = 255;

    float off = 0.5; // how much cheaper it is rent that buy
    

    printf("Enter the title of the book to process rent: ");
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Remove trailing newline character

    FILE *file = fopen(filename, "r");
    int index = searchBook(file, searchTitle);


    if (index == -1){
        printf("Book not found in the inventory.\n");
        return;
    }

    fgets(buffer, sizeof(buffer), file);  //author
    buffer[strcspn(buffer, "\n")] = '\0';
    char author[255];
    strcpy(author, buffer);
    strcpy(author,extractStringFromLine(buffer));


    fgets(buffer, sizeof(buffer), file); //genre
    buffer[strcspn(buffer, "\n")] = '\0';
    char genre[255];
    strcpy(genre, buffer);
    strcpy(genre,extractStringFromLine(buffer));

    fgets(buffer, sizeof(buffer), file); // price
    buffer[strcspn(buffer, "\n")] = '\0';
    char *cprice = extractStringFromLine(buffer);
    float price = atof(cprice)*off; // 50% off

    fgets(buffer, sizeof(buffer), file); // quntity for sale
    buffer[strcspn(buffer, "\n")] = '\0';
    char *csell = extractStringFromLine(buffer);
    int sell = atof(csell) ;

    fgets(buffer, sizeof(buffer), file); // quntity for rent
    buffer[strcspn(buffer, "\n")] = '\0';
    char *crent = extractStringFromLine(buffer);
    int rent = atof(crent) ;
    // printf("%d", rent);

    fclose(file);
       

    int quantity;
    printf("Enter the quantity to rent: ");
    scanf("%d", &quantity);

    if(quantity>rent){
        printf("Insufficient quantity in stock.\n");
        return;
    }

    int day;
    printf("Enter number of days to rent: ");
    scanf("%d", &day);

    // POSSIBLE DEVELOPMENT:
    // ask buyer's contacts to remind them to return the book in time

    char buf[256] = {0};
    time_t rawtime = time(NULL);
    // printf("%ld\n",rawtime);
    // rawtime /=365*60*24*60; // years since eroch(?)
    struct tm *ptm = localtime(&rawtime);
    strftime(buf, BUF_LEN, "%D", ptm); // %D for day/month/year format 

    // puts(buf);


    Book temp;

    strcpy(temp.title, searchTitle);
    strcpy(temp.author, author);
    strcpy(temp.genre, genre);

    temp.price = price; // return to original price
    temp.quantity_sale = sell;
    temp.quantity_rent = rent-quantity;

    // write into rent.txt
    generateRentalReport(rentFileName,temp,quantity,buf,day);

    temp.price = price*(pow(off, -1));
    

    // make changes to the inventory
    // decrease number of books to rent in stock(inventory)
    makeChanges(filename,index, temp);

    printf("Rent processed successfully.\n");

    // Как чек чтобы вывело плз
    printf("______чек(?)_______");
    printf("%s\n", searchTitle);
    printf("%s\n", author);
    printf("%f\n", price); 
    printf("%d\n", quantity);
    printf("%f\n", price*quantity); // total
    printf("days %d\n", day);

}

void displayRentalReport(char *filename){

    FILE *file = fopen(filename, "r");
    // Display sales report header
    printf("Rent Report\n");
    printf("------------\n");
    float total_rev = 0;

    char buffer[255];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("Title: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Author: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Quantity sold: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Total rent price: %s", buffer);
        total_rev += atof(buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Date when it was rented: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Days it was rented for: %s", buffer);


        printf("------------\n");
    }

    fclose(file);

    printf("Total rev: %f\n", total_rev);

    // printf bestselling books
    bestSeller(filename, 'r');   

    
}


//TODO // bring back the book (opposite of renting)
void returnRented(){

}


void sort13(char *filename, char *tofind, char ag ){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char buffer[255];
    char title[255];
    char author[255];
    char genre[255];
    char price[255];
    char quantity_sale[255];
    char quantity_rent[255];

    int found = 0; // Flag to indicate if any books are found for the given author

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strcmp(buffer, "#\n") == 0) {
            fgets(title, sizeof(title), file); // Read title
            title[strcspn(title, "\n")] = '\0';
            strcpy(title,extractStringFromLine(title));

            fgets(author, sizeof(author), file); // Read author
            author[strcspn(author, "\n")] = '\0';
            strcpy(author,extractStringFromLine(author));

            fgets(genre, sizeof(genre), file); // Read genre
            genre[strcspn(genre, "\n")] = '\0';
            strcpy(genre,extractStringFromLine(genre));

            fgets(price, sizeof(price), file); // Read price
            price[strcspn(price, "\n")] = '\0';
            strcpy(price,extractStringFromLine(price));

            fgets(quantity_sale, sizeof(quantity_sale), file); // Read quant sale
            quantity_sale[strcspn(quantity_sale, "\n")] = '\0';
            strcpy(quantity_sale,extractStringFromLine(quantity_sale));
            
            fgets(quantity_rent, sizeof(quantity_rent), file); // Read price
            quantity_rent[strcspn(quantity_rent, "\n")] = '\0';
            strcpy(quantity_rent,extractStringFromLine(quantity_rent));

            if ((ag == 'a' && strcmp(author, tofind) == 0) || (ag == 'g' && strcmp(genre, tofind) == 0)){

                printf("Title: %s\n", title);
                printf("Author: %s\n", author);
                printf("Genre: %s\n", genre);
                printf("Price: %s\n", price);
                printf("Quantity for sale: %s\n", quantity_sale);
                printf("Quantity for rent: %s\n", quantity_rent);
                printf("_________________________\n");
                found = 1;
            }

        }
    }

    fclose(file);

    if (!found) {
        printf("No books found for the given author.\n");
    }
}

void browse(char *filename){
    int choice;
    char author[255];
    char genre [255];

    printf("Sort by:\n");
    printf("1. Author\n");
    printf("2. Price (increasing)\n");
    printf("3. Genre\n");

    printf("Enter your choice: ");

    scanf("%d", &choice);
    getchar(); // Consume the newline character

    switch (choice) {
        case 1:
            
            printf("Which author's book you want?: ");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = '\0';
            sort13(filename, author,'a');
            break;   

        // case 2:
        //     sortPrice(filename, &numBooks);
        //     break;
        
        case 3:
            printf("Which genre book you want?: ");
            fgets(genre, sizeof(genre), stdin);
            genre[strcspn(genre, "\n")] = '\0';
            sort13(filename, genre, 'g');
            break;
        default:
            printf("Invalid input!");
            browse(filename);
            return;
    }
}

void displayMenu() {
    printf("Main Menu\n");
    printf("---------\n");
    printf("1. Add Book\n");
    printf("2. Update Book\n");
    printf("3. Get Book Information\n");
    printf("4. Process Sale\n");
    printf("5. Rent Book\n");
    printf("6: Return Rented Book\n");
    printf("7. Browse Books\n");
    printf("8. Display Sales Report\n");
    printf("9. Display Rental Report\n");
    printf("10. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    // not absolute address
    char *fileName = "inventory.txt";
    char *saleFileName = "sale.txt";
    char *rentFileName = "rent.txt";

    // Book inventory[100]; // Assuming a maximum of 100 books in the inventory

    // Counts the initial number of books(line in a file) in inventory before starting the operations
    numBooks = 0;

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
                updateBook(fileName);
                break;
            case 3:
                getBookInfo(fileName, numBooks);
                break;
            case 4:
                processSale(fileName,saleFileName);
                break;
            case 5:
                processRent(fileName, rentFileName);
                break;
            //case 6:
            //    returnRent(fileName);
            //    break;
            case 7:
                browse(fileName);
                break;
            case 8:
                displaySaleReport(saleFileName);
                break;
                
            case 9:
                displayRentalReport(rentFileName);
                break;
            case 10:
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