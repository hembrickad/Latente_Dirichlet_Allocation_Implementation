#include "Utils.cpp"
#include "Housekeeping.cpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <time.h>


using namespace std;

// const int k = 6;
// int totalWords = 0;
// vector<docuTopicsMatrix> docuTopicCount;//Count for each topic
// vector<docuTopics> docuTopicLabel;//Final General Topic For the Document
// vector<wordTopicsMatrix> wordTopicCount; //Count for each topic
// vector<vector<wordTopics>> wordTopicLabel;//Final General Topic for each


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
int makeDistribution(docuTopicsMatrix d, wordTopicsMatrix w ){
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

//Finds the final label for each document
int max(docuTopics label , docuTopicsMatrix topics){
    int max = -1;
    int index = 0;
    int num;

    for(int i = 0; i < k; i++){
        num = topics.topics[i];
        if (topics.topics[i] > max){
            max = topics.topics[i];
            index++;
        }
    }

    return index;
}

//Places labels into docuTopicLabel
void labelDocuments(){
    int i = 0;

    for(docuTopics label: docuTopicLabel){
        docuTopicLabel.at(i).t = max(label, docuTopicCount.at(1));
        i++;
    }
}

//Print output
void printOutput(){
    for(docuTopics label: docuTopicLabel){
        printf("Title: %s --> Topic Group: %d\n", label.document.c_str(), label.t);
    }
}


int main(int argc, char **argv ){

    //./SingleLDA <dataset> <iterations>"

    vector<string> fp = Read_File("data/practice.csv");
    vector<vector<string>> titlesAndAbstracts;
    vector<vector<string>> wordsInAbstracts;    // Vector Of Documents By Words In Abstract(s) (Not Unique)
    int i, index, topic;
    int itr = 1;

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
    setupDocuTopicLabel(titlesAndAbstracts);

    struct timespec start, end;

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

    labelDocuments();

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    uint64_t diff = (1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec) / 1e6;

    printOutput();
    
    printf("\n\n\nRuntime: %llu ms", (long long unsigned int) diff);

    cleanUp();
  
    printf("\nComplete");

}