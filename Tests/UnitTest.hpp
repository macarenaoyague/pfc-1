#pragma once
#include "../Algorithms/Dantzig.hpp"
#include "../Algorithms/Spira.hpp"
#include "../Algorithms/MoffatAndTakaoka.hpp"
#include "Util.hpp"
#include <chrono>

void UnitTest(string filename, bool checkResults = false) {
    int n;
    vector<vector<int>> matrixAdjacency;
    vector<int> vertices;
    readFromFile(filename, n, matrixAdjacency);
    generateVerticesIndexes(matrixAdjacency.size(), vertices);
    Graph* graph = createGraph(vertices, matrixAdjacency);
    vector<float> executionTime(2);
    vector<vector<float>> times(executionTime.size(), vector<float>(3, 0));
    vector<vector<int>> counts(executionTime.size(), vector<int>(3, 0));
    vector<int> dist;
    cout << "Graph of " << n << " nodes" << endl;
    for(int i = 0; i < n; ++i){
        int idx = i;
        if(checkResults)
            dist = dijkstra(matrixAdjacency, idx, vertices.size());

        vector<BaseAlgorithm*> algorithms{new OriginalMoffatAndTakaoka(graph),
                                          new BoostMoffatAndTakaoka<fibonacci_heap<pairType, boost::heap::compare<compareItem>>>(graph)};
        vector<float> time;
        vector<float> pushTime = {0,0,0}, popTime = {0,0,0}, cleanUpTime;
        int k = 0;
        for(auto & algorithm : algorithms){
            auto start = chrono::system_clock::now();
            auto result = algorithm->executeAlgorithm(vertices[idx]);
            auto end = chrono::system_clock::now();
            std::chrono::duration<float,std::milli> duration = end - start;
            time.push_back(duration.count());
            
            times[k][0] += algorithm->pushOperation.first;
            times[k][1] += algorithm->popOperation.first;
            times[k][2] += algorithm->cleanUpOperation.first; 

            counts[k][0] += algorithm->pushOperation.second;
            counts[k][1] += algorithm->popOperation.second;
            counts[k][2] += algorithm->cleanUpOperation.second; 
            
            k++;

            if(checkResults && !equalResults(vertices, dist, result)){
                cout << "FAIL " <<  algorithm->getAlgorithmName() << ": idx = " << vertices[idx] << "\n";
            }

            delete algorithm;
        }
        for (int j = 0; j < executionTime.size(); j++) executionTime[j] += time[j];
    }

    for (int i = 0; i < executionTime.size(); i++) {
        cout << "Execution time N°" << i+1 << ": " << executionTime[i] << "ms" << endl;
        cout << "push_time = " << times[i][0] <<  ", push_count = " << counts[i][0] << ", average = " << times[i][0]*1000/counts[i][0] << endl;
        cout << "pop_time = " << times[i][1] <<  ", pop_count = " << counts[i][1] << ", average =  " << times[i][1]*1000/counts[i][1] << endl;
        cout << "cleanUp_time = " << times[i][2] << ", cleanUp_count =  " << counts[i][2] << ", average =  " << times[i][2]*1000/counts[i][2] << endl;
    }
    
    delete graph;
}