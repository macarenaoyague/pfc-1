#include "DantzigV2.h"

const int V = 9;
const int MAX = 100000;

// copiado de geeks for geeks
int minDistance(bool sptSet[], vector<int>& dist) {
    int min = MAX, min_index;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}

void dijkstra(vector<vector<int>> graph, int src, vector<int>& dist) {
    bool sptSet[V];
    for (int i = 0; i < V; i++)
        dist[i] = MAX, sptSet[i] = false;
    dist[src] = 0;
    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(sptSet, dist);
        sptSet[u] = true;
        for (int v = 0; v < V; v++)
            if (!sptSet[v] && graph[u][v] && dist[u] != MAX
                && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }
}

void Testing(int idx) {
    cout << "Probando i=" << idx << endl;

    vector<int> dist(9);
    vector<int> vertices = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    vector<vector<int>> graph1 = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };

    dijkstra(graph1, idx, dist);
    Graph graph(vertices, graph1, 0);
    Dantzig dantzig(&graph);
    auto dantzigAns = dantzig.SingleSource(idx);
    
    bool funciona = true;
    for (int i = 0; i < V; i++) {
        if (dist[i] != dantzigAns[i]) {
            cout << i << ": " << dist[i] << "!=" << dantzigAns[i+1] << endl;
            funciona = false;
        }
    }
    if (funciona) cout << "******** SUCCESS";
    else cout << "******** NO SUCCESS";
    cout << endl;
}