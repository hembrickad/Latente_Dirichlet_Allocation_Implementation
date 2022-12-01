#include "Utils.cpp"
#include "Housekeeping.cpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <time.h>


using namespace std;

//Selects Random Topic Based on Given Distribution
int randomTopicSelection(vector<float> distribution){
    srand(time(NULL));
    int i = 0;
    float r = ((float) rand() / (RAND_MAX)) + 0.0001;

    while(i < distribution.size()){
        if(distribution.at(i) > r)
            break;
        i++;
    }

    return i;
}

//Normalizes current topic vector into a distribution
vector<float> normalize(vector<float> topics, float num){
    vector<float> topicsNorm;
    float n = 0;

    for(int i = 0; i < topics.size(); i++){
        topicsNorm.push_back((float)topics.at(i)/(float)num);
    }

    for(int i = 0; i < topics.size(); i++){
        topicsNorm.at(i) += n;
        n = topicsNorm.at(i);
    }

    return topicsNorm;  
}

//Multiples the TopicsMatrix objects for each word and returns a topic based on the distribution
int makeDistribution(docuTopicsMatrix d, wordTopicsMatrix w){
    vector<float> topics;
    float num = 0;

    for(int i = 0; i < k; i++ ){
        num += (d.topics[i] * w.topics[i]);
        topics.push_back(d.topics[i] * w.topics[i]);
    }

    return randomTopicSelection(normalize(topics, (float)num));
}

//Decrements the value of an associated words topic for both Count vectors
int decrement(wordTopics str, int index){
    int i = 0;

    for(wordTopicsMatrix w: wordTopicCount){
        if(strcmp(str.word.c_str(), w.word.c_str()) == 0){
            wordTopicCount.at(i).topics[str.t]--;
            break;
        }
        i++;
    }
    docuTopicCount.at(index).topics[str.t]--;

    return i;
}

//Increments the value of an associated words new topic for both Count vectors
void increment(wordTopics str, int index, int newTopic){
    int i = 0;
    for(wordTopicsMatrix w: wordTopicCount){
        if(strcmp(str.word.c_str(), w.word.c_str()) == 0){
            wordTopicCount.at(i).topics[newTopic]++;
        }
        i++;
    }
    docuTopicCount.at(index).topics[newTopic]++;
}

//Print output
void printOutput(){
    int i = 0;
    for(vector<wordTopics> documents: wordTopicLabel){
        printf("Doc #:%d \n", (i+1));
        for(wordTopics w: documents)
            printf("\tWord: %s,  Topic Association: %d\n", w.word.c_str(), (w.t+1));
        i++;
    }
}

int main(int argc, char **argv){

    //./SingleLDA <dataset> <iterations>"
    struct timespec start, end;
    struct timespec startWSet;
    vector<string> fp = Read_File("data/data_small.csv");
    vector<vector<string>> titlesAndAbstracts;
    vector<vector<string>> wordsInAbstracts;

        // Vector Of Documents By Words In Abstract(s) (Not Unique)
    int i, index, topic;
    int itr = 20;

    clock_gettime(CLOCK_MONOTONIC_RAW, &startWSet);

    // Split Based On Line Delimiter (",")
    for (string line : fp){
        titlesAndAbstracts.push_back(Split_String_By_Delimiter(line, ","));
    }

    // Split Abstracts In Documents To Individual Words
    for (vector<string> titleAndAbstractVtr : titlesAndAbstracts){
        wordsInAbstracts.push_back(Split_String_To_Words(titleAndAbstractVtr));
    }

    setupWordTopicCount(wordsInAbstracts);
    setupDocuTopicCount(titlesAndAbstracts);
    setupWordTopicLabel(wordsInAbstracts);

    

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for(int j = 0; j < itr; j++){
    i = 0;

        for(vector<wordTopics> x: wordTopicLabel){
            for(wordTopics y : x){
                index = decrement(y,i);
                topic = makeDistribution(docuTopicCount.at(i), wordTopicCount.at(index));
                increment(y, i, topic);
            }
           i++;
        }
    }

    printOutput();

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    uint64_t diff = (1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec) / 1e6;
    uint64_t diff2 = (1000000000L * (end.tv_sec - startWSet.tv_sec) + end.tv_nsec - startWSet.tv_nsec) / 1e6;

    

    printf("\n\nRuntime LDA: %llu ms", (long long unsigned int) diff);
    printf("\nRuntime Total: %llu ms", (long long unsigned int) diff2);

    cleanUp();
  
    printf("\nComplete");

}