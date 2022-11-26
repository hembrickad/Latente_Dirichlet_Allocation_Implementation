#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

// Reads File As Vector Of Lines
vector<string> Read_File(string file_path)
{
    ifstream input_file(file_path.c_str());
    vector<string> file_data_vtr;

    if (input_file.is_open())
    {
        string line = "";

        while (getline(input_file, line))
        {
            file_data_vtr.push_back(line);
        }

        input_file.close();
    }

    return file_data_vtr;
}

// Split String Using A Delimiter
vector<string> Split_Line(string str, string delimiter){
    vector<string> split_line_tokens;
    string str_token = "";
    int position = 0;
    regex extra_spaces("\\s+"); // Remove Extra Spaces Within String/Line
    regex trim("^\\s+|\\s+$");  // Remove Pre-and-Post White Space

    while ((position = str.find(delimiter)) != string::npos)
    {
        // Extract The Current Delimited String Within The Line
        str_token = str.substr(0, position);

        // Clean String Token
        str_token = regex_replace(str_token, trim, "");

        // Add It To The String Array/Vector
        split_line_tokens.push_back(str_token);

        // Remove It From The String
        str.erase(0, position + delimiter.length());
    }

    // If Tokens Remaining Within 'str' Add The Last Line
    if (str.length() > 0)
    {
        // Clean String Token First
        str = regex_replace(str, trim, "");
        split_line_tokens.push_back(str);
    }

    return split_line_tokens;
}

// Splits all abstracts in to separate words
vector<vector<string>> Split_words(vector<vector<string>> document){
    vector<vector<string>> words;

    //Loop through all abstracts and spl
     for( int i = 0; i < document.size(); i++ ){
        //Only read Abstracts
        words.push_back(Split_Line(document.at(i).at(1), " "));
     }

    return words;
}

// int main(int argc, char **argv ){

//     vector<string> fp = Read_File("data/practice.csv");
//     vector<vector<string>> titlesAndAbstracts;
//     vector<vector<string>> wordsInAbstracts;

//     // Split Based On Line Delimiter (",")
//     for (string line : fp){
//         titlesAndAbstracts.push_back(Split_Line(line, ","));
//     }

//     wordsInAbstracts = Split_words(titlesAndAbstracts);  
//  }