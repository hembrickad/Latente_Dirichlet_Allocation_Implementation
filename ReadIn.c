#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#define MAXCHAR 10000


int main(){

    char row[MAXCHAR];

    FILE* fp = fopen("/Users/Adhsketch/Desktop/repos/2022-603-Project-Hembrickad/data/practice.csv","r");

    while (fgets(row, MAXCHAR, fp))
    {
        int column = 0;

        char* r = strtok(row, ",");
        while(r){

            if(column == 0)
                printf("Title: ");
            else
                printf("Abstract: ");
            
            printf("%s\n", r);
            r = strtok(NULL, ",");
            column++;
        }
    }
    
    fclose(fp);
    return 0;
}

