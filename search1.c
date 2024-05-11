#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

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
// Define data structure for sold books
typedef struct {
    char title[255];
    int total_sold;
} SoldBook;

int numBooks;
// Function declarations
void writeBookToFile(FILE *file, Book book,int index);
void appendBookToFile(char *fileName,int *numBooks);
char* extractStringFromLine(char *line);
void makeChanges(char *file, int index, Book book);
int searchBookInFile(FILE* file, char *searchTitle);
void updateBookInfo(char *filename);
void getBookInfo(char *filename, int numBooks);
void generateSalesReport(char* filename, Book book, int quantity);
void processSale(char *filename, char *sellFileName);
void findBestSellingBook(char *filename, char rent);
void displaySaleReport(char *filename);
void generateRentalReport(char* filename, Book book, int quantity, char *date, int days); 
void processRent(char *filename, char *rentFileName);
void displayRentalReport(char *filename);  
void fillTemporaryBookDetails(Book *book, char *title, char *author, char *genre, float price, int sell, int rent);
void sortRecords(char *filename, char *tofind, char ag );
void browseBooks(char *filename);
void displayMenu();

// Writes Book structure in the end of the inventory file
void writeBookToFile(FILE *file, Book book,int index){

    fputs("#\n", file); // separator between books
    fprintf(file,"%d: %s\n",index,book.title);
    fprintf(file,"Author: %s\n",book.author);
    fprintf(file,"Genre: %s\n",book.genre);
    fprintf(file,"Price: %0.2f\n",book.price);
    fprintf(file,"Quantity for sale: %d\n",book.quantity_sale);
    fprintf(file,"Quantity for rent: %d\n",book.quantity_rent);
}

// Function to add new books to the inventory
void appendBookToFile(char *fileName,int *numBooks) {
    
    // temporary book to store user input
    Book new;
    // Prompt the user to enter book details
    printf("\n\t\033[35m Add book information:\033[0m\n");
    printf("Enter the title: ");
    fgets(new.title, sizeof(new.title), stdin);
    new.title[strcspn(new.title, "\n")] = '\0'; // Remove trailing newline character

    printf("Enter the author: ");
    fgets(new.author, sizeof(new.author), stdin);
    new.author[strcspn(new.author, "\n")] = '\0'; // Remove trailing newline character

    printf("Enter the genre: ");
    fgets(new.genre, sizeof(new.genre), stdin);
    new.genre[strcspn(new.genre, "\n")] = '\0'; // Remove trailing newline character

    printf("Enter the price: ");
    while(1){
            if(scanf("%f", &new.price)!=1){
                printf("\033[31;1mInvalid input.\033[0m Please enter a number!\nYour price: ");
                scanf("%*[^\n]"); // Clear input buffer
                scanf("%*c"); 
            }
            else{
                break;
            }
        }

    printf("Enter the quantity for sale: ");
    while(1){
            if(scanf("%d", &new.quantity_sale)!=1){
                printf("\033[31;1mInvalid input.\033[0m Please enter a number!\nYour quantity for sale: ");
                scanf("%*[^\n]"); 
                scanf("%*c"); 
            }
            else{
                break;
            }
        }

    printf("Enter the quantity for rent: ");
    while(1){
            if(scanf("%d", &new.quantity_rent)!=1){
                printf("\033[31;1mInvalid input.\033[0m Please enter a number!\nYour quantity for rent: ");
                scanf("%*[^\n]"); 
                scanf("%*c"); 
            }
            else{
                break;
            }
        }

    // Increment the number of books in the inventory
    (*numBooks)++;    

    FILE * file = fopen("inventory.txt","a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    writeBookToFile(file, new, *numBooks);
    fclose(file);

    printf("Book added successfully.\n");
}

// Function returns TITLE from 1: TITLE
//                      AUTHOR from 1.1: AUTHOR
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

// Function to make changes to the inventory file
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

    writeBookToFile(temp, book, index);

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

// Function to search for a book in the inventory file
int searchBookInFile(FILE* file, char *searchTitle){
    
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
void updateBookInfo(char *filename) {

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

    int index = searchBookInFile(file, searchTitle );

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
    while(1){
            if(scanf("%f", &temp.price)!=1){
                printf("\033[31;1mInvalid input.\033[0m Please enter a number!\nYour price: ");
                scanf("%*[^\n]"); // Clear input buffer
                scanf("%*c"); 
            }
            else{
                break;
            }
        }

    printf("Enter the new quantity for sale: ");
    while(1){
            if(scanf("%d", &temp.quantity_sale)!=1){
                printf("\033[31;1mInvalid input.\033[0m Please enter a number!\nYour quantity for sale: ");
                scanf("%*[^\n]"); 
                scanf("%*c"); 
            }
            else{
                break;
            }
        }

    printf("Enter the new quantity for rent: ");
    while(1){
            if(scanf("%d", &temp.quantity_rent)!=1){
                printf("\033[31;1mInvalid input.\033[0m Please enter a number!\nYour quantity for rent: ");
                scanf("%*[^\n]"); 
                scanf("%*c"); 
            }
            else{
                break;
            }
        }

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

    int index = searchBookInFile(file, searchTitle);

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
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file,"%s\n",book.title);
    fprintf(file,"%s\n",book.author);
    fprintf(file,"%s\n",book.genre);
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
    if (file == NULL) {
        perror("Error opening file");
        return ;
    }
    int index = searchBookInFile(file, searchTitle);


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

        printf("\t\t\033[1;32;4mSALE RECEIPT\033[0m\n");
        printf("Title: %s\n", searchTitle);
        printf("Author: %s\n", author);
        printf("Price: %.2f\n", price); 
        printf("Quanity: %d\n", quantity);
        printf("Total: %.2f\n", price*quantity); // total

    } 
    else {
        printf("Insufficient quantity in stock.\n");
        printf("There are only %d in the stock\n", sell);
        return;
    }
}

// Function to find the best selling book
void findBestSellingBook(char *filename, char rent){
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
        fgets(buffer, sizeof(buffer), file); // genre
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
    switch (rent)
    {
    case 's':
        printf("The best selling book is: %s\n\n", best_selling_title);
        break;
    
    case 'r':
        printf("The best renting book is: %s\n\n", best_selling_title);
        break;
    }

}

// Function to display sales report
void displaySaleReport(char *filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    // Display sales report header
    printf("\n\t\t\033[1;36;4mSALES REPORT\033[0m\n\n"); 
    float total_rev = 0;

    char buffer[255];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("Title: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Author: %s", buffer);
        fgets(buffer, sizeof(buffer), file);
        printf("Genre: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Quantity sold: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Total sales amount: %s", buffer);
        total_rev += atof(buffer);
        printf("------------\n");
    }

    fclose(file);

    printf("\033[1mTotal rev: %.2f\033[0m\n", total_rev);

    // printf bestselling books
    findBestSellingBook(filename, 's');    
}

// writes into rent file, where all transactions are stored
void generateRentalReport(char* filename, Book book, int quantity, char *date, int days) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error opening file");
        return ;
    }

    fprintf(file,"%s\n",book.title);
    fprintf(file,"%s\n",book.author);
    fprintf(file,"%s\n",book.genre);
    fprintf(file,"%d\n",quantity); // Quantity sold
    fprintf(file,"%.2f\n",book.price * (float)quantity);  // total price
    fprintf(file,"%s", date);
    fprintf(file, "%d\n", days);

    // in case if in future price of the book will change
    // these wont be any problem in the code, as report was generated right after selling

    fclose (file);   
}

// Function to process book rent
void processRent(char *filename, char *rentFileName) {

    // title
    // author
    // genre
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
    if (file == NULL) {
        perror("Error opening file");
        return ;
    }
    int index = searchBookInFile(file, searchTitle);


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
    float price = (atof(cprice))/2; // 50% off

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
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strcpy(buf, asctime(tm));
    


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

    printf("\t\t\033[1;32;4mRENT RECEIPT\033[0m\n");
    printf("Title: %s\n", searchTitle);
    printf("Author: %s\n", author);
    printf("Price: %.2f\n", price); 
    printf("Quantity: %d\n", quantity);
    printf("Total: %.2f\n", price*quantity); // total
    printf("Days for rent: %d\n", day);

}

// Function to display rental report
void displayRentalReport(char *filename){

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    // Display sales report header
    printf("\n\t\t\033[1;36;4mRENT REPORT\033[0m\n");
    float total_rev = 0;

    char buffer[255];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("Title: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Author: %s", buffer);

        fgets(buffer, sizeof(buffer), file);
        printf("Genre: %s", buffer);

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

    printf("\033[1mTotal rev: %.2f\033[0m\n", total_rev);

    // printf bestselling books
    findBestSellingBook(filename, 'r');   

    
}

// Function to fill temporary book details
void fillTemporaryBookDetails(Book *book, char *title, char *author, char *genre, float price, int sell, int rent){
    strcpy(book->title, title);
    strcpy(book->author, author);
    strcpy(book->genre, genre);
    book->price = price;
    book->quantity_sale = sell;
    book->quantity_rent = rent;
}

// Function to sort records
void sortRecords(char *filename, char *tofind, char ag ){
    FILE *inventory = fopen(filename, "r");


    if (inventory == NULL) {
        printf("Error opening file.\n");
        return;
    }

    FILE *fake = fopen("fake.txt", "a");
    if (fake == NULL) {
        printf("Error opening fake file.\n");
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

    while (fgets(buffer, sizeof(buffer), inventory) != NULL) {
        if (strcmp(buffer, "#\n") == 0) {
            fgets(title, sizeof(title), inventory); // Read title
            title[strcspn(title, "\n")] = '\0';
            strcpy(title,extractStringFromLine(title));

            fprintf(fake, "%s\n", title);

            fgets(author, sizeof(author), inventory); // Read author
            author[strcspn(author, "\n")] = '\0';
            strcpy(author,extractStringFromLine(author));

            fprintf(fake, "%s\n", author);

            fgets(genre, sizeof(genre), inventory); // Read genre
            genre[strcspn(genre, "\n")] = '\0';
            strcpy(genre,extractStringFromLine(genre));

            fprintf(fake, "%s\n", genre);

            fgets(price, sizeof(price), inventory); // Read price
            price[strcspn(price, "\n")] = '\0';
            strcpy(price,extractStringFromLine(price));

            fprintf(fake, "%s\n", price);

            fgets(quantity_sale, sizeof(quantity_sale), inventory); // Read quant sale
            quantity_sale[strcspn(quantity_sale, "\n")] = '\0';
            strcpy(quantity_sale,extractStringFromLine(quantity_sale));

            fprintf(fake, "%s\n", quantity_sale);
            
            fgets(quantity_rent, sizeof(quantity_rent), inventory); // Read sell quantity
            quantity_rent[strcspn(quantity_rent, "\n")] = '\0';
            strcpy(quantity_rent,extractStringFromLine(quantity_rent));

            fprintf(fake, "%s\n", quantity_rent);

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

    // if this function was call for author or genre and it failed to find any book
    if(ag == 'a' || ag == 'g'){
        if (!found) {
            printf("No books found for the given paramether.\n");
        }

        fclose(inventory); // we wont touch inventory any more
        fclose(fake);
        remove("fake.txt");
        return;
    }

    // if fucntion was called for price sorting
    else{
        float p; //price
        int s; // sell
        int r; // rent

        fclose(inventory); // we wont touch inventory any more
        fclose(fake); // fake version of inventory which we will delete books from

        int smallest;
        int smallest_index;
        Book temp_book;

        for (int i = 0; i < numBooks; i++){
            // for each book
            FILE *fake = fopen("fake.txt", "r");
                if (fake == NULL) {
                    perror("Error opening file");
                    return;
                }

            for (int j = 0; j < numBooks-i; j++){
                fgets(title, sizeof(title), fake);
                title[strcspn(title, "\n")] = '\0';

                fgets(author, sizeof(author), fake); // Read author
                author[strcspn(author, "\n")] = '\0';

                fgets(genre, sizeof(genre), fake); // Read genre
                genre[strcspn(genre, "\n")] = '\0';

                fgets(price, sizeof(price), fake); // Read price
                price[strcspn(price, "\n")] = '\0';
                p = atof(price);

                // first element is orientier for finding smallest
                if(j == 0){
                    smallest = p; // smallest price
                    smallest_index = j;

                    fgets(quantity_sale, sizeof(quantity_sale), fake); // Read quant sale
                    quantity_sale[strcspn(quantity_sale, "\n")] = '\0';
                    s = atoi(quantity_sale);

                    fgets(quantity_rent, sizeof(quantity_rent), fake); // Read rent quantity
                    quantity_rent[strcspn(quantity_rent, "\n")] = '\0';
                    r = atoi(quantity_rent);

                    fillTemporaryBookDetails(&temp_book, title, author, genre, p, s, r);
                    continue;
                }
                else if(p<smallest){
                    smallest = p;

                    fgets(quantity_sale, sizeof(quantity_sale), fake); // Read quant sale
                    quantity_sale[strcspn(quantity_sale, "\n")] = '\0';
                    s = atoi(quantity_sale);

                    fgets(quantity_rent, sizeof(quantity_rent), fake); // Read sell quantity
                    quantity_rent[strcspn(quantity_rent, "\n")] = '\0';
                    r = atoi(quantity_rent);

                    fillTemporaryBookDetails(&temp_book, title, author, genre, p, s, r);

                    smallest_index = j;
                    continue;
                }
                fgets(quantity_sale, sizeof(quantity_sale), fake); // Read quant sale
                fgets(quantity_rent, sizeof(quantity_rent), fake); // Read sell quantity
            }

            // print smallest
            printf("___________________\n");
            printf("Title: %s\n", temp_book.title);
            printf("Author: %s\n", temp_book.author);
            printf("Genre: %s\n", temp_book.genre);
            printf("Price: %.2f\n", temp_book.price);
            printf("Quantity for sale: %d\n", temp_book.quantity_sale);
            printf("Quantity for rent: %d\n", temp_book.quantity_rent);
            printf("___________________\n");

            fclose(fake);

            // delete from fake
            FILE *helper = fopen("helper.txt", "a"); //append
            FILE *fakee = fopen("fake.txt", "r"); //read
            if (helper == NULL || fakee== NULL) {
                perror("Error opening file");
                return;
            }

            for (int k = 0; k < smallest_index*6; k++){
                fgets(buffer, sizeof(buffer), fakee);
                fprintf(helper, "%s", buffer);
            }

            // dont copy book at smallest index, go throu it without copying into helper
            for (int s = 0; s < 6; s++){
                fgets(buffer, sizeof(buffer), fakee);
            }

            while(fgets(buffer, sizeof(buffer), fakee)!= NULL){
                fprintf(helper, "%s", buffer);
            }

            fclose(helper);
            fclose(fakee);
            remove("fake.txt");
            rename ("helper.txt", "fake.txt");

        }
    }
    remove("fake.txt");
}

// Function to browse books
void browseBooks(char *filename){
    int choice;
    char author[255];
    char genre [255];

    printf("\n\t\t\033[1;36;4mSort by:\033[0m\n");
    printf("1. Author\n");
    printf("2. Price (increasing)\n");
    printf("3. Genre\n");

    printf("Enter your choice: ");

    while(1){
            if(scanf("%d", &choice)!=1){
                printf("\033[31;1mInvalid input.\033[0m Please enter a number!\nYour choice: ");
                scanf("%*[^\n]"); // Clear input buffer
                scanf("%*c"); 
            }
            else{
                break;
            }
        }
    getchar(); // Consume the newline character

    switch (choice) {
        case 1:
            printf("Which author's book you want?: ");
            fgets(author, sizeof(author), stdin);
            author[strcspn(author, "\n")] = '\0';
            printf("\n");
            sortRecords(filename, author,'a');
            printf("\n\t\033[3;32m     Sotring completed!\033[0m\n");
            break;   
        case 2:
            printf("\n");
            sortRecords(filename, " ", 'p');
            printf("\n\t\033[3;32m     Sotring completed!\033[0m\n");

            break;
        case 3:
            printf("Which genre book you want?: ");
            fgets(genre, sizeof(genre), stdin);
            genre[strcspn(genre, "\n")] = '\0';
            printf("\n");
            sortRecords(filename, genre, 'g');
            printf("\n\t\033[3;32m     Sotring completed!\033[0m\n");
            break;
        default:
            printf("\033[31;1mInvalid choice!\033[0m Please try again\n");
            browseBooks(filename);
            return;
    }
}

// Function to display the main menu
void displayMenu() {
    printf("\n\t\t\033[1;36;4mMain Menu\033[0m\n");
    printf("1. Add Book\n");
    printf("2. Update Book\n");
    printf("3. Get Book Information\n");
    printf("4. Process Sale\n");
    printf("5. Rent Book\n");
    printf("6. Sort Books\n");
    printf("7. Display Sales Report\n");
    printf("8. Display Rental Report\n");
    printf("\033[32m9. Exit\033[0m\n");
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
    displayMenu();
    do {
        while(1){
            if(scanf("%d", &choice)!=1){
                printf("\033[31;1mInvalid input.\033[0m Please enter a number!\nYour choice: ");
                fflush(stdin);
            }
            else{
                break;
            }
        }
        getchar(); // Consume the newline character

        switch (choice) {
            case 1:
                appendBookToFile(fileName, &numBooks);
                break;
            case 2:
                updateBookInfo(fileName);
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
            case 6:
                browseBooks(fileName);
                break;
            case 7:
                displaySaleReport(saleFileName);
                break;
                
            case 8:
                displayRentalReport(rentFileName);
                break;
            case 9:
                printf("\n\t\t\033[31mExiting...\033[0m\n\n");
                exit(0);
            default:
                printf("\033[31;1mInvalid choice.\033[0m Please try again.\nYour choice: ");
                break;
        }

        fflush(stdin); // after user inut a number they also input \n, get rid of it

    } while (1);

    return 0;
}