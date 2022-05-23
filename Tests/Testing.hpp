#pragma once
#include "../Algorithms/Dantzig.hpp"
#include "../Algorithms/Spira.hpp"
#include "../Algorithms/MoffatAndTakaoka.hpp"
#include "Util.hpp"
#include "Tester.hpp"
#include <cstdlib>
#include <fstream>
#include <chrono>
#include <thread>

void UnitTest(string filename) {
    int n;
    vector<vector<int>> matrixAdjacency;
    vector<int> vertices;
    readFromFile(filename, n, matrixAdjacency);
    generateVerticesIndexes(matrixAdjacency.size(), vertices);
    Graph* graph = createGraph(vertices, matrixAdjacency);
    vector<float> executionTime = {0, 0, 0};

    for(int i = 0; i < n; ++i){
        int idx = i;
        vector<int> dist = dijkstra(matrixAdjacency, idx, vertices.size());
        vector<BaseAlgorithm*> algorithms{new OriginalDantzig(graph), // new ImprovedDantzig(graph),
                                          new OriginalSpira(graph), new MoffatAndTakaoka(graph)};
        vector<float> time;
        for(auto & algorithm : algorithms){
            auto start = chrono::system_clock::now();
            auto result = algorithm->executeAlgorithm(vertices[idx]);
            auto end = chrono::system_clock::now();
            std::chrono::duration<float,std::milli> duration = end - start;
            time.push_back(duration.count());

            if(!equalResults(vertices, dist, result)){
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