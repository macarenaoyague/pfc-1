#include "Algorithm.h"
#include <stdlib.h>  

const int MAX = 10000000;

void printVector(vector<int>& dist){
    for(auto item : dist)
        cout << item << " ";
    cout << endl;
}

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

void readFromFile(string fileName, vector<vector<bool>>& matrixAdjacencyBoolean){
    // TODO colocar este y más grafos en archivos para ser leídos
    matrixAdjacencyBoolean = { { 0, 1, 0, 0, 0, 0, 0, 1, 0 },
                        { 1, 0, 1, 0, 0, 0, 0, 1, 0 },
                        { 0, 1, 0, 1, 0, 1, 0, 0, 1 },
                        { 0, 0, 1, 0, 1, 1, 0, 0, 0 },
                        { 0, 0, 0, 1, 0, 1, 0, 0, 0 },
                        { 0, 0, 1, 1, 1, 0, 1, 0, 0 },
                        { 0, 0, 0, 0, 0, 1, 0, 1, 1 },
                        { 1, 1, 0, 0, 0, 0, 1, 0, 1 },
                        { 0, 0, 1, 0, 0, 0, 1, 1, 0 } };
}

void generateMatrixAdjacency(string fileName, vector<vector<int>>& matrixAdjacency){
    vector<vector<bool>> matrixAdjacencyBoolean;
    readFromFile(fileName, matrixAdjacencyBoolean);
    vector<int> listAdjacency;

    srand(time(NULL));
    float seedFunction;
    for (int i = 0; i < 9; i++){
        seedFunction = float(rand()%100+1)/100;
        for (auto boolean: matrixAdjacencyBoolean[i]){
            if (boolean){
                listAdjacency.push_back((rand()%1000)*seedFunction);
            } else {
                listAdjacency.push_back(0);
            }
        }
        matrixAdjacency.push_back(listAdjacency);
        listAdjacency.clear();
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
    generateMatrixAdjacency(fileName, matrixAdjacency);
    vector<int> vertices; 
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