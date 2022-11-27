#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

using namespace std;

// Reads File As Vector Of Lines
vector<string> Read_File(string file_path);
// Split String Using A Delimiter
vector<string> Split_String_By_Delimiter(string str = "", string delimiter = ",", bool remove_extra_whitespace = false);
// Splits all abstracts in to separate words
vector<string> Split_String_To_Words(vector<string> document);