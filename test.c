#include<stdio.h>

int main(){
    FILE *fptr;
    fptr = fopen("words.txt","r+");

    char chunk[128];
 
     while(fgets(chunk, sizeof(chunk), fptr) != NULL) {
         fputs(chunk, stdout);
         fputs("|*\n", stdout);  // marker string used to show where the content of the chunk array has ended
     }
}