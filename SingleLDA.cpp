#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <time.h>
#include "Utils.cpp"
#include "Housekeeping.cpp"

using namespace std;

const int k = 6;
int totalWords = 0;
vector<vector<int>> dTopics;
vector<vector<int>> wTopics;
vector<wordTopicsMatrix> wordTopicCount;

//Unique word : count to each topic
struct wordTopicsMatrix{
    string word;
    int* topics;
};

//Document : count to each topic
struct docuTopicsMatrix{
    string document;
    int* topics;
};

//Word : Highest probable topic
struct wordTopics{
    string word;
    int t;
};

//Document: Highest probable topic
struct docuTopics{
    string document;
    int t;
};


//Normalizes current topic vector into a distribution
vector<double> normalize(vector<double> topics){
    vector<double> topicsNorm;
    double n = 0;

    for(int i = 0; i < topics.size(); i++){
        topicsNorm.push_back((double)topics.at(i)/(double)totalWords);
    }

    for(int i = 0; i < topics.size(); i++){
        topicsNorm.at(i) += n;
        n = topicsNorm.at(i);
    }

    for(int i = 0; i < topics.size(); i++){
        printf("%f\n", topicsNorm.at(i));
    }

    return topicsNorm;  
}

//Selects Random Topic Based on Given Distribution
int randomTopicSelection(vector<double> distribution){
    srand(time(NULL));
    int i = 0;
    float r = ((double) rand() / (RAND_MAX)) + 0.0001;

    while(i < distribution.size()){
        if(distribution.at(i) > r)
            break;
        i++;
    }

    return i;
}

wordTopicsMatrix createWordTopic(string str){
    int* ptr = (int*) calloc(k, sizeof(int));
    wordTopicsMatrix word;
    word.word = str;
    word.topics = ptr;    
    return word;
}

wordTopicsMatrix createWordTopic(string str){
    int* ptr = (int*) calloc(k, sizeof(int));
    wordTopicsMatrix word;
    word.word = str;
    word.topics = ptr;    
    return word;
}

void cleanUp(){
    for(wordTopicsMatrix n : wordTopicCount){
        free(n.topics);
    }

    for(wordTopicsMatrix n : wordTopicCount){
        free(n.topics);
    }
}


//Extracts Unique Words
vector<wordTopicsMatrix> setupTopicMatrix(vector<vector<string>> wordsInAbstracts){
    vector<wordTopicsMatrix> words;
    int i = -1;

    for(vector<string> k: wordsInAbstracts){
        for(string n : k){
            for(wordTopicsMatrix x : words){
                i = strcmp(n.c_str(), x.word.c_str());
                if(i == 0)
                    break;
            }
            if(i != 0)
                words.push_back(createWordTopic(n));
        }
    }
    return words;
}







int main(int argc, char **argv ){

    //randomTopicSelection(normalize());

    vector<string> fp = Read_File("data/practice.csv");
    vector<vector<string>> titlesAndAbstracts;
    vector<vector<string>> wordsInAbstracts;    // Vector Of Documents By Words In Abstract(s) (Not Unique)

    // Split Based On Line Delimiter (",")
    for (string line : fp){
        titlesAndAbstracts.push_back(Split_String_By_Delimiter(line, ","));
    }

    // Split Abstracts In Documents To Individual Words
    for (vector<string> titleAndAbstractVtr : titlesAndAbstracts)
    {
        wordsInAbstracts.push_back(Split_String_To_Words(titleAndAbstractVtr));
    }

    vector<wordTopicsMatrix> words = setupTopicMatrix(wordsInAbstracts);

    for(wordTopicsMatrix n: words){
        printf("%s, \n",n.word.c_str());
    }



    // For Each Document, Print Words In Document
    // for (vector<string> abstractWords : wordsInAbstracts)
    // {
    //     for (string word : abstractWords)
    //     {
    //         printf("%s, ",word.c_str());
    //     }
    //     printf("\n");
    // }

    printf("Complete");

    // wordsInAbstracts = Split_words(titlesAndAbstracts);  
}