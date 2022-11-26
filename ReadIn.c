#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#define MAXCHAR 10000

void readIn(){
    char row[MAXCHAR];
    int numRows = 0;
    char* titles = NULL;
    char* abstracts = NULL;

    titles = malloc(sizeof(char));
    abstracts = malloc(sizeof(char));


    FILE* fp = fopen("/Users/Adhsketch/Desktop/repos/2022-603-Project-Hembrickad/data/practice.csv","r");

    while (fgets(row, MAXCHAR, fp))
    {
        int column = 0;
        char* r = strtok(row, ",");

        while(r){    

            if(column == 0){  
                titles = realloc(titles, sizeof(titles) + sizeof(r));
                r = strtok(NULL, ",");
                column++;
            }
            else{
                abstracts = realloc(titles, sizeof(abstracts) + sizeof(r));
                r = strtok(NULL, ",");
                column++;
            }
        }
    }
    
    fclose(fp);
}

int main(){

    
    return 0;
}

