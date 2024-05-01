#include <stdio.h>

int main(){
    FILE *file = fopen("inventory.txt", "r+");
    fprintf(file, 8);
}