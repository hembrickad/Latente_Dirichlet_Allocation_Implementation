#include "Utils.cpp"
#include "ThreadedHousekeeping.cpp"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <pthread.h>
#include <vector>
#include <time.h>

vector<vector<string>>* TWords;

using namespace std;

//Creates a vector of unique words and an associated topic array/pointer
void* setupWordTopicCount(void * data){
    int i = -1;
    struct LDAData* lda = (struct LDAData*)data;
    int a = lda->start;
    int b = lda->end;

    for(int j = lda->start; j < lda->end; j++){
        for(string n : TWords->at(j)){
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
void* setupWordTopicLabel(void * data){
    vector<wordTopics> lines;
    int i = 0;
    struct LDAData* lda = (struct LDAData*)data;

    for(int j = lda->start; j < lda->end; j++){
        for(string n : TWords->at(j)){
            lines.push_back(createWordTopics(n, i));
            totalWords++;
        }
        wordTopicLabel.push_back(lines);
        lines.clear();
        i++;
    }
}

//Selects Random Topic Based on Given Distribution
int randomTopicSelection(vector<float> distribution){
    srand(time(NULL));
    int i = 0;
    float r = ((float) rand() / (RAND_MAX)) + 0.0001;

    while(i < distribution.size()){
        if(distribution.at(i) > r)
            break;
        i++;
    }

    return i;
}

//Normalizes current topic vector into a distribution
vector<float> normalize(vector<float> topics, float num){
    vector<float> topicsNorm;
    float n = 0;

    for(int i = 0; i < topics.size(); i++){
        topicsNorm.push_back((float)topics.at(i)/(float)num);
    }

    for(int i = 0; i < topics.size(); i++){
        topicsNorm.at(i) += n;
        n = topicsNorm.at(i);
    }

    return topicsNorm;  
}

//Multiples the TopicsMatrix objects for each word and returns a topic based on the distribution
int makeDistribution(docuTopicsMatrix d, wordTopicsMatrix w){
    vector<float> topics;
    float num = 0;

    for(int i = 0; i < k; i++ ){
        num += (d.topics[i] * w.topics[i]);
        topics.push_back(d.topics[i] * w.topics[i]);
    }

    return randomTopicSelection(normalize(topics, (float)num));
}

//Decrements the value of an associated words topic for both Count vectors
int decrement(wordTopics str, int index){
    int i = 0;

    for(wordTopicsMatrix w: wordTopicCount){
        if(strcmp(str.word.c_str(), w.word.c_str()) == 0){
            wordTopicCount.at(i).topics[str.t]--;
            break;
        }
        i++;
    }
    docuTopicCount.at(index).topics[str.t]--;

    return i;
}

//Increments the value of an associated words new topic for both Count vectors
void increment(wordTopics str, int index, int newTopic){
    int i = 0;
    for(wordTopicsMatrix w: wordTopicCount){
        if(strcmp(str.word.c_str(), w.word.c_str()) == 0){
            wordTopicCount.at(i).topics[newTopic]++;
        }
        i++;
    }
    docuTopicCount.at(index).topics[newTopic]++;
}

//Print output
void printOutput(){
    int i = 0;
    for(vector<wordTopics> documents: wordTopicLabel){
        printf("Doc #:%d \n", (i+1));
        for(wordTopics w: documents)
            printf("\tWord: %s,  Topic Association: %d\n", w.word.c_str(), (w.t+1));
        i++;
    }
}


int main(int argc, char **argv){

    //./SingleLDA <dataset> <iterations>"
    int n_threads = 2;

    pthread_t *threads;
    int num[n_threads];
    struct LDAData* LDAdata = (struct LDAData*)malloc(n_threads*sizeof(struct LDAData));


    threads = (pthread_t*)malloc(n_threads*sizeof(pthread_t));
    
    vector<string> fp = Read_File("data/train.csv");
    vector<vector<string>> titlesAndAbstracts;
    vector<vector<string>> wordsInAbstracts;    // Vector Of Documents By Words In Abstract(s) (Not Unique)
    TWords = new vector<vector<string>>();

    int i, index, topic;
    int itr = 1;

     // Split Based On Line Delimiter (",")
    for (string line : fp){
        titlesAndAbstracts.push_back(Split_String_By_Delimiter(line, ","));
    }

    // Split Abstracts In Documents To Individual Words
    for (vector<string> titleAndAbstractVtr : titlesAndAbstracts){
        TWords->push_back(Split_String_To_Words(titleAndAbstractVtr));
    }

    int numTest = (TWords->size()/n_threads);
    int remain = (TWords->size()%n_threads);

    for(int i = 0; i < n_threads; i++){
    	num[i] = numTest;
    	if(remain > 0){
    		num[i] ++;	
    		remain --;
    	}
    }

    for(int i = 0; i < n_threads; i++){
        if(i == 0){
    		LDAdata[i].start = 0;
    		LDAdata[i].end = num[i]-1;
    	}	
    	else{
    		LDAdata[i].start = (LDAdata[i-1].end + 1);
    		LDAdata[i].end = (LDAdata[i].start + num[i])-1;
    	}
    }

    for (i = 0; i < n_threads; i++){
    		pthread_create(&threads[i], NULL, setupWordTopicCount, (void*)&LDAdata[i]);
    }
    	
    for (i = 0; i < n_threads; i++){
      		pthread_join(threads[i], NULL);
    }

    for (i = 0; i < n_threads; i++){
    		pthread_create(&threads[i], NULL, setupWordTopicLabel, (void*)&LDAdata[i]);
    }
    	
    for (i = 0; i < n_threads; i++){
      		pthread_join(threads[i], NULL);
    }
    // setupWordTopicCount(wordsInAbstracts);
    // setupDocuTopicCount(titlesAndAbstracts);
    // setupWordTopicLabel(wordsInAbstracts);

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    for(int j = 0; j < itr; j++){
    i = 0;

        for(vector<wordTopics> x: wordTopicLabel){
            for(wordTopics y : x){
                index = decrement(y,i);
                topic = makeDistribution(docuTopicCount.at(i), wordTopicCount.at(index));
                increment(y, i, topic);
            }
           i++;
        }
    }

    printOutput();

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    uint64_t diff = (1000000000L * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec) / 1e6;

    printf("\n\nRuntime: %llu ms", (long long unsigned int) diff);

    cleanUp();
    free(threads);
    free(LDAdata);
  
    printf("\nComplete");

}