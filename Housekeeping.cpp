//#include "Utils.cpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <time.h>


using namespace std;

// ~~~~~~~ Structs ~~~~~~~~//
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

// ~~~~~~~ Global Variables ~~~~~~~~//
const int k = 1;
int totalWords = 0;
vector<docuTopicsMatrix> docuTopicCount;//Count for each topic
vector<wordTopicsMatrix> wordTopicCount; //Count for each topic
vector<vector<wordTopics>> wordTopicLabel;//Final General Topic for each

// ~~~~~~~ Methods ~~~~~~~~//
//Free's all pointers in docuTopicCount and wordTopicCount
void cleanUp(){
    for(wordTopicsMatrix n : wordTopicCount){
        free(n.topics);
    }

    for(docuTopicsMatrix n : docuTopicCount){
        free(n.topics);
    }
}

//Counts the randomized topic given to wordTopics for both topic count variables
void count(wordTopics w, int indexD){
    int i = 0;
    int val;
    for(wordTopicsMatrix str: wordTopicCount){
        if(strcmp(str.word.c_str(), w.word.c_str())==0){
            wordTopicCount.at(i).topics[w.t]++;
        }
        i++;
    }
    docuTopicCount.at(indexD).topics[w.t]++;
}

//Creates wordTopicsMatrix structs for the wordTopicCount
wordTopicsMatrix createWordTopicMatrix(string str){
    int* ptr = (int*) calloc(k, sizeof(int));
    wordTopicsMatrix word;
    word.word = str;
    word.topics = ptr;    
    return word;
}

//Creates docuTopicsMatrix structs for the docuTopicCount
docuTopicsMatrix createDocuTopicMatrix(string str){
    int* ptr = (int*) calloc(k, sizeof(int));
    docuTopicsMatrix docu;
    docu.document= str;
    docu.topics = ptr;    
    return docu;
}

//Creates wordTopics structs for wordTopicLabel
wordTopics createWordTopics(string str, int i){
    srand((unsigned)time(0)*totalWords);
    wordTopics word;
    word.word = str;
    word.t = (rand() % k);
    count(word, i);
    return word;
}

//Creates a vector of unique words and an associated topic array/pointer
void setupWordTopicCount(vector<vector<string>> wordsInAbstracts){
    int i = -1;

    for(vector<string> k: wordsInAbstracts){
        for(string n : k){
            for(wordTopicsMatrix x : wordTopicCount){
                i = strcmp(n.c_str(), x.word.c_str());
                if(i == 0)
                    break;
            }
            if(i != 0)
                wordTopicCount.push_back(createWordTopicMatrix(n));
        }
    }
}

//Creates a list of every word for every document and initializes with random topic
void setupWordTopicLabel(vector<vector<string>> wordsInAbstracts){
    vector<wordTopics> lines;
    int i = 0;

    for(vector<string> k: wordsInAbstracts){
        for(string n : k){
            lines.push_back(createWordTopics(n, i));
            totalWords++;
        }
        wordTopicLabel.push_back(lines);
        lines.clear();
        i++;
    }
}

//Creates a vector of document titles and an associated topic array/pointer
void setupDocuTopicCount(vector<vector<string>> titlesAndAbstracts){
    for(vector<string> k: titlesAndAbstracts){
            docuTopicCount.push_back(createDocuTopicMatrix(k.at(0)));
    }
}
