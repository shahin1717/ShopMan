#include<stdio.h>

int main(){
    FILE *fptr;
    fptr = fopen("inventory.txt","ab");
    fprintf(fptr,"...\n%d,",5050);
    fputs("hello..", fptr);
    fclose(fptr);
}