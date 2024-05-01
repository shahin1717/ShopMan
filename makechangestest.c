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

void writeBookIntoFile(FILE *file, Book book,int index);
void makeChanges (char *file, int index, Book book);

int main(){
    Book kniqa;
    strcpy(kniqa.author, "NEW");
    strcpy(kniqa.title, "TOTALY NEW");
    strcpy(kniqa.genre, "aha");
    kniqa.id = 2;
    kniqa.price = 1000.005;
    kniqa.quantity_rent = 5;
    kniqa.quantity_sale = 6;
    char start [] = "inventory.txt";
    makeChanges(start,3,kniqa);
}

void writeBookIntoFile(FILE *file, Book book,int index){

    fputs("#\n", file); // separator between books
    fprintf(file,"%d: %s\n",index,book.title);
    fprintf(file,"%d.1: %s\n",index,book.author);
    fprintf(file,"%d.2: %s\n",index,book.genre);
    fprintf(file,"%d.3: %0.2f\n",index,book.price);
    fprintf(file,"%d.4: %d\n",index,book.quantity_sale);
    fprintf(file,"%d.5: %d\n",index,book.quantity_rent);

}


void makeChanges (char *file, int index, Book book){
    FILE *temp = fopen("temp.txt", "w");
    FILE *initial = fopen(file, "r");

    char buffer[255];

    // copy everything until changing part
    int i = 0;
    while (1){
        fgets(buffer, 255, initial);
        if (strcmp(buffer, "#\n") == 0){
            i++;
            if (i>= index){
                break;
            }
        }
        
        fprintf(temp, "%s", buffer);
    }

    writeBookIntoFile(temp, book, index);

    // skip old record
    for (int k = 0; k<6; k++){
        fgets(buffer, 255, initial);
    }

    while(fgets(buffer, 255, initial) != NULL){
        fprintf(temp, "%s", buffer);
    }
     fclose(temp);
     fclose(initial);

     FILE *updated = fopen(file, "w");
     FILE *tempp = fopen("temp.txt", "r");

     while(fgets(buffer, 255, tempp) != NULL){
        fprintf(updated, "%s", buffer);
    }
    
    fclose(updated);
    fclose (tempp);

    remove("temp.txt");
}
