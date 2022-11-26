#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// Forward Declaration
vector<string> Read_File( string file_path );
vector<string> Split_Line( string str = "", string delimiter = "," );

int main( int argc, char **argv )
{

    vector<string> file_contents_vtr = Read_File( "data/practice.csv" );
    vector<vector<string>> title_content_vtr;

    //Split Based On Line Delimiter (",")
    for( string line : file_contents_vtr )
    {
        title_content_vtr.push_back( Split_Line( line ) );
    }

    //Print all Titles
    /*for( int i = 0; i < title_content_vtr.size(); i++ )
    {
        string title = title_content_vtr.at( i ).at( 0 );
        printf( "%s\n", title.c_str() );
    }*/


    //Print all abstracts
    for( int i = 0; i < title_content_vtr.size(); i++ )
    {
        // Get The Current Document
        vector<string> document = title_content_vtr.at( i );

        // Only Print The Document Contents, Not The Title
        //    i.e. Skip The Title (Index 0)
        for( int j = 1; j < document.size(); j++ )
        {
            printf( "%s\n", document.at( j ).c_str() );
        }
    }

    // You Can Also Access The Array/Vector Elements Using Standard Syntax
    // string str = title_content_vtr.at( i ).at( j );
    // string str = title_content_vtr[i][j];

    return 0;
}

// Reads File As Vector Of Lines
vector<string> Read_File( string file_path )
{
    ifstream input_file( file_path.c_str() );
    vector<string> file_data_vtr;

    if( input_file.is_open() )
    {
        string line = "";

        while( getline( input_file, line ) )
        {
            file_data_vtr.push_back( line );
        }

        input_file.close();
    }

    return file_data_vtr;
}

// Split String Using A Delimiter
vector<string> Split_Line( string str, string delimiter )
{
    vector<string> split_line_tokens;
    string str_token = "";
    int position = 0;
    regex extra_spaces( "\\s+" );   // Remove Extra Spaces Within String/Line
    regex trim( "^\\s+|\\s+$" );    // Remove Pre-and-Post White Space

    while( ( position = str.find( delimiter ) ) != string::npos )
    {
        // Extract The Current Delimited String Within The Line
        str_token = str.substr( 0, position );

        // Clean String Token
        str_token = regex_replace( str_token, trim, "" );

        // Add It To The String Array/Vector
        split_line_tokens.push_back( str_token );

        // Remove It From The String
        str.erase( 0, position + delimiter.length() );
    }

    // If Tokens Remaining Within 'str' Add The Last Line
    if( str.length() > 0 )
    {
        // Clean String Token First
        str = regex_replace( str, trim, "" );
        split_line_tokens.push_back( str );
    }

    return split_line_tokens;
}