#include "Utils.h"
#include <cctype>

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
vector<string> Split_String_By_Delimiter(string str, string delimiter, bool clean_words){
    vector<string> split_line_tokens;
    string str_token = "";
    int position = 0;
    regex extra_spaces("\\s+"); // Remove Extra Spaces Within String/Line
    regex trim("^\\s+|\\s+$");  // Remove Pre-and-Post White Space
    regex punctuation("s/[^\\w+|\\s+|\\d+]//g");
    regex latex("\\$[\\w+|\\W+]+?\\$");

 
    if(clean_words)
    {
        
        //Remove More Than One Whitespace
        str = regex_replace(str, extra_spaces, " ");

        //Removes Latex Terms
        str = regex_replace(str, latex, "");

        //Remove Punctuation
        str = regex_replace(str, punctuation, "");



        str.erase(remove(str.begin(), str.end(), ')'), str.end());
        str.erase(remove(str.begin(), str.end(), '('), str.end());
        str.erase(remove(str.begin(), str.end(), '.'), str.end());
        str.erase(remove(str.begin(), str.end(),'"'), str.end());

        //Changes a strings to lowercase
        transform(str.begin(), str.end(), str.begin(), ::tolower);
    }

    while ((position = str.find(delimiter)) != string::npos){

        // Extract The Current Delimited String Within The Line
        str_token = str.substr(0, position);

        // Clean String Tokens
        str_token = regex_replace(str_token, trim, "");

        // Add It To The String Array/Vector
        split_line_tokens.push_back(str_token);

        // Remove It From The String
        str.erase(0, position + delimiter.length());
    }

    // If Tokens Remaining Within 'str' Add The Last Line
    if (str.length() > 0){
        // Clean String Token First
        str = regex_replace(str, trim, "");
        split_line_tokens.push_back(str);
    }

    return split_line_tokens;
}

// Splits all abstracts in to separate words
vector<string> Split_String_To_Words(vector<string> document){
    vector<string> words;

    //Loop through all abstracts and spl
     for(int i = 1; i < document.size(); i++){
        //Only read Abstracts
        vector<string> abstractWords = Split_String_By_Delimiter(document.at(i), " ", true);

        for(string word : abstractWords)
        {
            words.push_back(word);
        }
     }

    return words;
}