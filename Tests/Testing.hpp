#pragma once
#include "Dantzig.hpp"
#include "Spira.hpp"
#include "MoffatAndTakaoka.hpp"
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

    for(int i = 0; i < n; ++i){
        int idx = i;
        vector<int> dist = dijkstra(matrixAdjacency, idx, vertices.size());
        vector<BaseAlgorithm*> algorithms{new OriginalDantzig(graph), // new ImprovedDantzig(graph),
                                          new OriginalSpira(graph), new MoffatAndTakaoka(graph)};
        for(auto & algorithm : algorithms){
            auto result = algorithm->executeAlgorithm(vertices[idx]);
            if(!equalResults(vertices, dist, result)){
                cout << "FAIL " <<  algorithm->getAlgorithmName() << ": idx = " << vertices[idx] << "\n";
            }
            delete algorithm;
        }
    }

    delete graph;
}