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
    vector<float> executionTime = {0, 0};
    vector<int> dist;
    cout << "Graph of " << n << " nodes" << endl;
    for(int i = 0; i < n; ++i){
        int idx = i;
        if(checkResults)
            dist = dijkstra(matrixAdjacency, idx, vertices.size());

        vector<BaseAlgorithm*> algorithms{new OriginalMoffatAndTakaoka(graph),
                                          new BoostMoffatAndTakaoka<fibonacci_heap<pairType, boost::heap::compare<compareItem>>>(graph),
                                          new BoostMoffatAndTakaoka<binomial_heap<pairType, boost::heap::compare<compareItem>>>(graph)};
        vector<float> time;
        for(auto & algorithm : algorithms){
            auto start = chrono::system_clock::now();
            auto result = algorithm->executeAlgorithm(vertices[idx]);
            auto end = chrono::system_clock::now();
            std::chrono::duration<float,std::milli> duration = end - start;
            time.push_back(duration.count());

            if(checkResults && !equalResults(vertices, dist, result)){
                cout << "FAIL " <<  algorithm->getAlgorithmName() << ": idx = " << vertices[idx] << "\n";
            }

            delete algorithm;
        }
        for (int j = 0; j < executionTime.size(); j++) executionTime[j] += time[j];
    }

    for (int i = 0; i < executionTime.size(); i++) {
        cout << "Execution time N°" << i+1 << ": " << executionTime[i] << "ms" << endl;
    }
    
    delete graph;
}