#include "Algorithm.h"
#include <stdlib.h>  
#include <fstream>

const int MAX = 10000000;

// copiado de geeks for geeks
int minDistance(vector<bool> sptSet, vector<int>& dist, int& size) {
    int min = MAX, min_index;
    for (int v = 0; v < size; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}

vector<int> dijkstra(vector<vector<int>> graph, int src, int size) {
    vector<int> dist(size);
    vector<bool> sptSet(size);
    for (int i = 0; i < size; i++)
        dist[i] = MAX, sptSet[i] = false;
    dist[src] = 0;
    for (int count = 0; count < size - 1; count++) {
        int u = minDistance(sptSet, dist, size);
        sptSet[u] = true;
        for (int v = 0; v < size; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
    return dist;
}

void readFromFile(string fileName, vector<vector<int>>& matrixAdjacency){
    ifstream file(fileName);
    if(file.is_open()){
        int n, weight;
        vector<int> v;
        file >> n;
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                file >> weight;
                v.emplace_back(weight);
            }
            matrixAdjacency.emplace_back(v);
            v.clear();
        }
    }
    else {
        cout << "ERROR AL ABRIR ARCHIVO" << endl;
        assert(false);
    }
}

void generateVerticesIndexes(int size, vector<int>& vertices){
    for (int i = 1; i <= size; i++){
        vertices.push_back(i);
    }
}

void Testing(int idx, string fileName) {
    cout << "Probando i=" << idx << endl;
                 
    vector<vector<int>> matrixAdjacency;
    vector<int> vertices;

    readFromFile(fileName, matrixAdjacency);
    generateVerticesIndexes(matrixAdjacency.size(), vertices);

    vector<int> dist = dijkstra(matrixAdjacency, idx, vertices.size());

    Graph graph(vertices, matrixAdjacency, 0);
    Algorithm algorithm(&graph);
    
    auto dantzigAns = algorithm.DantzigAlgorithm(vertices[idx]);
    algorithm.restart();
    auto spiraAns = algorithm.SpiraAlgorithm(vertices[idx]);
    bool funciona = true;

    for (int i = 0; i < vertices.size(); i++) {
        if (dist[i] != dantzigAns[vertices[i]] || dist[i] != spiraAns[vertices[i]]) {
            cout << i << ": " << dist[i] << "!=" << dantzigAns[i+1] << endl;
            funciona = false;
        }
    }

    if (funciona) cout << "******** SUCCESS";
    else cout << "******** NO SUCCESS";
    cout << endl;
}