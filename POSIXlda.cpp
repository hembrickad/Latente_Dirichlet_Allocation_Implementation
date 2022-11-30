#include "Utils.cpp"
#include "ThreadedHousekeeping.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

#define NTHREADS 5

int main ()
{
    thread threads[NTHREADS];
    vector<string> fp = Read_File("data/practice.csv");
    
    // Split Based On Line Delimiter (",")
    for (string line : fp){
        titlesAndAbstracts.push_back(Split_String_By_Delimiter(line, ","));
    }

    // Split Abstracts In Documents To Individual Words
    for (vector<string> titleAndAbstractVtr : titlesAndAbstracts){
        wordsInAbstracts.push_back(Split_String_To_Words(titleAndAbstractVtr));
    }

    for(int i = 0; i < NTHREADS; i++)
        threads[i] = thread(setupDocuTopicCount);

    for(int i = 0; i < NTHREADS; i++)
        threads[i].join();
    
    for(int i = 0; i < NTHREADS; i++)
        threads[i] = thread(setupWordTopicLabel);

    for(int i = 0; i < NTHREADS; i++)
        threads[i].join();

    int i = 1;
}