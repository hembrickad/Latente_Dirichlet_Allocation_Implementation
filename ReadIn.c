#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#define MAXCHAR 10000


int main(){

    FILE *fp;
    char row[MAXCHAR];

    fp = fopen("/Users/Adhsketch/Desktop/repos/2022-603-Project-Hembrickad/data/practice.csv","r");

    if(fp == 0) {
    perror("fopen");
    exit(1);
}

    while (feof(fp) != true)
    {
        fgets(row, MAXCHAR, fp);
        printf("Row: %s", row);
    }
    
    return 0;
}

