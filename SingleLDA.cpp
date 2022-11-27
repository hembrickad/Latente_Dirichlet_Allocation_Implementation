#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <time.h>
#include "Utils.cpp"

using namespace std;

const int k = 6;
int total = 67;


vector<double> normalize(vector<double> topics){
    vector<double> topicsNorm;
    double n = 0;

    for(int i = 0; i < topics.size(); i++){
        topicsNorm.push_back((double)topics.at(i)/(double)total);
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



int main(int argc, char **argv ){

    randomTopicSelection(normalize());

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

    // For Each Document, Print Words In Document
    for (vector<string> abstractWords : wordsInAbstracts)
    {
        for (string word : abstractWords)
        {
            printf("%s, ",word.c_str());
        }
        printf("\n");
    }

    printf("Complete");

    // wordsInAbstracts = Split_words(titlesAndAbstracts);  
}