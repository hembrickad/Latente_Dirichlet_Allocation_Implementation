#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <time.h>
#include "Utils.cpp"

using namespace std;

const int k = 6;
int totalWords = 0;
vector<docuTopicsMatrix> docuTopicCount;
vector<docuTopics> docuTopicLabel;
vector<wordTopicsMatrix> wordTopicCount;
vector<wordTopics> wordTopicLabel;

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

// ~~~~~~~ Methods ~~~~~~~~//
//Creates wordTopicsMatrix structs for the wordTopicCount
wordTopicsMatrix createWordTopic(string str){
    int* ptr = (int*) calloc(k, sizeof(int));
    wordTopicsMatrix word;
    word.word = str;
    word.topics = ptr;    
    return word;
}

//Creates docuTopicsMatrix structs for the docuTopicCount
docuTopicsMatrix createDocuTopic(string str){
    int* ptr = (int*) calloc(k, sizeof(int));
    docuTopicsMatrix docu;
    docu.document= str;
    docu.topics = ptr;    
    return docu;
}

//Free's all pointers in docuTopicCount and wordTopicCount
void cleanUp(){
    for(wordTopicsMatrix n : wordTopicCount){
        free(n.topics);
    }

    for(docuTopicsMatrix n : docuTopicCount){
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