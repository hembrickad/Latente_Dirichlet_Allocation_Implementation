#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <time.h>
#include "Utils.cpp"

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

//Document: Highest probable topic
struct docuTopics{
    string document;
    int t;
};

// ~~~~~~~ Global Variables ~~~~~~~~//
const int k = 6;
int totalWords = 0;
vector<docuTopicsMatrix> docuTopicCount;//Count for each topic
vector<docuTopics> docuTopicLabel;//Final General Topic For the Document
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
    for(wordTopicsMatrix str: wordTopicCount)
        if(str.word.c_str(), w.word.c_str() == 0)
            str.topics[w.t]++;
    
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
    word.t = (rand()%6);
    count(word, i);
    return word;
}

//Creates wordTopics structs for wordTopicLabel
docuTopics createDocuTopics(string str){
    docuTopics docu;
    docu.document= str;
    docu.t = 0;    
    return docu;
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

//Creates a list of every word for every document and initializes with random topic
void setupDocuTopicLabel(vector<vector<string>> titlesAndAbstracts){
    vector<docuTopics> lines;

    for(vector<string> k: titlesAndAbstracts){
        docuTopicLabel.push_back(createDocuTopics(k.at(0)));  
    }
}


int main(int argc, char **argv ){

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

    setupWordTopicCount(wordsInAbstracts);
    setupDocuTopicCount(titlesAndAbstracts);
    setupWordTopicLabel(wordsInAbstracts);
    setupDocuTopicLabel(titlesAndAbstracts);
    
    cleanUp();
  
    printf("Complete");
}