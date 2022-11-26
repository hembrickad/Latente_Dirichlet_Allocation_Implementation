#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include "setUp.cpp"






int main(int argc, char **argv ){

    vector<string> fp = Read_File("data/practice.csv");
    vector<vector<string>> titlesAndAbstracts;
    vector<vector<string>> wordsInAbstracts;

    // Split Based On Line Delimiter (",")
    for (string line : fp){
        titlesAndAbstracts.push_back(Split_Line(line, ","));
    }

    wordsInAbstracts = Split_words(titlesAndAbstracts);  
 }