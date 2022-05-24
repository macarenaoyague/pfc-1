#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <iterator>
#include "../Classes/Graph.hpp"
const int MAX = 10000000;
using namespace std;

void printVector(vector<int>& vector, ostream& out = cout, const string& delimiter=" ") {
    for (auto item : vector) out << item << delimiter;
}

void printMatrix(vector<vector<int>>& matrix, ostream& out = cout) {
    for (auto& row : matrix) {
        printVector(row, out);
    }
    out << endl;
}

int minDistance(vector<bool> sptSet, vector<int>& dist, int& size) {
    int min = MAX, min_index;
    for (int v = 0; v < size; v++)
        if (sptSet[v] == false && dist[v] <= min) min = dist[v], min_index = v;

    return min_index;
}

vector<int> dijkstra(vector<vector<int>> graph, int src, int size) {
    vector<int> dist(size);
    vector<bool> sptSet(size);
    for (int i = 0; i < size; i++) dist[i] = MAX, sptSet[i] = false;
    dist[src] = 0;
    for (int count = 0; count < size - 1; count++) {
        int u = minDistance(sptSet, dist, size);
        sptSet[u] = true;
        for (int v = 0; v < size; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != MAX &&
                dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
    return dist;
}

void generateVerticesIndexes(int size, vector<int>& vertices) {
    for (int i = 0; i < size; i++) {
        vertices.push_back(i);
    }
}

void readFromFile(string filename, int& n, vector<vector<int>>& matrixAdjacency) {
    ifstream file(filename, ios_base::in | ios_base::binary);
    if (file.is_open()) {
        file.read((char*) &n, sizeof(short));
        for(int i = 0; i < n; ++i){
            short arr[n];
            file.read((char *)& arr, sizeof(short) * n);
            vector<int> v(arr, arr + n);
            matrixAdjacency.emplace_back(v);
        }
    } else {
        cout << "ERROR AL ABRIR ARCHIVO" << endl;
        assert(false);
    }
}

vector<int> readNVertices(const string& filename){
    ifstream file(filename);
    if(file.is_open()) {
        vector<int> r;
        short n;
        while (!file.eof()) {
            file.read((char*)&n, sizeof(short));
            r.emplace_back(n);
        }
        return r;
    }
    else assert(false);
}

Graph* createGraph(vector<vertexIndex>& vertices, vector<vector<weightType>>& matrixAdjacency){
    auto* graph = new Graph(vertices, matrixAdjacency, 0);
    graph->sortAdjacencyList();
    return graph;
}

Graph* readGraphFromFile(string filename){
    int n;
    vector<vector<int>> matrixAdjacency;
    vector<int> vertices;
    readFromFile(filename, n, matrixAdjacency);
    generateVerticesIndexes(matrixAdjacency.size(), vertices);
    return createGraph(vertices, matrixAdjacency);
}

bool equalResults(vector<vertexIndex>& vertices, vector<int>& dist,
                     unordered_map<vertexIndex, weightType>& result){
    bool ok = true;
    for(int i = 0; i < vertices.size(); i++){
        if(dist[i] != result[vertices[i]]) {
            cout << "to " << vertices[i] << ": " << dist[i] << "!=" << result[vertices[i]] << "\n";
            ok = false;
        }
    }
    return ok;
}

void saveResults(const string& filename, vector<double>& results, vector<int> nvertices){
    ofstream file(filename);
    if(file.is_open()) {
        for (int i = 0; i < results.size(); i++) {
            file << nvertices[i] << " = " << results[i] << endl;
        }
        file.close();
    }else{
        cout << "NO SE PUDO ABRIR EL ARCHIVO " << endl;
    }
}