#include "Utils.cpp"
#include "MultiThreadedHousekeeping.cpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <time.h>
#define n_threads 2


using namespace std;

struct LDAData* threading(int size){
    int num[n_threads];
    int numTest = (size/n_threads);
    int remain = (size%n_threads);

    struct LDAData* data = (struct LDAData*)malloc(n_threads*sizeof(struct LDAData));
    	
    for(int i = 0; i < n_threads; i++){
    	num[i] = numTest;
    	if(remain > 0){
    		num[i] ++;	
    		remain --;
    	}
    }
    	
    for(int i = 0; i < n_threads; i++){
    	if(i == 0){
    		data[i].start = 0;
    		data[i].end = num[i]-1;
    	}	
    	else{
    		data[i].start = (data[i-1].end + 1);
    		data[i].end = (data[i].start +num[i])-1;
    	}
    }

    return data;
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
    str.t = newTopic;
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
    pthread_t *threads = (pthread_t*)malloc(n_threads*sizeof(pthread_t));

    //struct timespec start, start1, start3, end3, end1, end;
    int i, index, topic;
    int itr = 5;

    vector<string> fp = Read_File("data/data_small_demo.csv");

    //clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // Split Based On Line Delimiter (",")
    for (string line : fp){
        titlesAndAbstracts.push_back(Split_String_By_Delimiter(line, ","));
    }

    
    struct LDAData* docu = threading(titlesAndAbstracts.size());
    docuTopicCount = vector<docuTopicsMatrix>(titlesAndAbstracts.size());
    wordsInAbstracts = vector<vector<string>>(titlesAndAbstracts.size());

    for (i = 0; i < n_threads; i++){
    	pthread_create(&threads[i], NULL, setDataSet, (void*)&docu[i]);
    }
    	
    for (i = 0; i < n_threads; i++){
      	pthread_join(threads[i], NULL);
    }


    struct LDAData* words = threading(wordsInAbstracts.size());

    wordTopicLabel = vector<vector<wordTopics>>(wordsInAbstracts.size());
    

    setupWordTopicCount(wordsInAbstracts);

    for (i = 0; i < n_threads; i++){
    	pthread_create(&threads[i], NULL, setupDocuTopicCount, (void*)&docu[i]);
    }
    	
    for (i = 0; i < n_threads; i++){
      	pthread_join(threads[i], NULL);
    }


    for (i = 0; i < n_threads; i++){
    	pthread_create(&threads[i], NULL, setupWordTopicLabel, (void*)&words[i]);
    }
    	
    for (i = 0; i < n_threads; i++){
      	pthread_join(threads[i], NULL);
    }

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

    cleanUp();
  
    // printf("\nComplete");

    free(threads);
}