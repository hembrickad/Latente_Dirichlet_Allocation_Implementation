#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>


const int k = 6;
int *totalT;
struct wordTopics* words;

//Unique word tied to topic popularity
struct wordTopics{
    char word[40];
    int *topics;
};

struct wordDocuments{
    int document;
    int popTopic;
};

int normalize(){
    int num = 0;
    float rand = randNum();
    float* norm = (float*) calloc(k, sizeof(float));

    for(int i = 0; i < k; i++){

    }
    free(norm);
}

float randNum(){
    return (double)rand() / (double)RAND_MAX;
}

void topicDistribution(){


}

void wordDistribution(){



}

void documentToTopic(){


}

void wordToTopic(){


}

void setUpWordArray(){




}

int main(int argc, char *argv[]){

/*Potential terminal in take for iterations 

*/

words = (struct wordTopics*) malloc(sizeof(struct wordTopics));

setUpWordArray();





}