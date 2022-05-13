#include "Dantzig.h"

const int V = 9;
const int MAX = 100000;

int minDistance(bool sptSet[], vector<int>& dist) {
    int min = MAX, min_index;
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}
 
void dijkstra(int graph[V][V], int src, vector<int>& dist) {
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



void Testing() {

    vector<int> dist(9);
    int graph1[V][V] = { { 0, 4, 0, 0, 0, 0, 0, 8, 0 },
                        { 4, 0, 8, 0, 0, 0, 0, 11, 0 },
                        { 0, 8, 0, 7, 0, 4, 0, 0, 2 },
                        { 0, 0, 7, 0, 9, 14, 0, 0, 0 },
                        { 0, 0, 0, 9, 0, 10, 0, 0, 0 },
                        { 0, 0, 4, 14, 10, 0, 2, 0, 0 },
                        { 0, 0, 0, 0, 0, 2, 0, 1, 6 },
                        { 8, 11, 0, 0, 0, 0, 1, 0, 7 },
                        { 0, 0, 2, 0, 0, 0, 6, 7, 0 } };

    dijkstra(graph1, 0, dist);
    
    cout << "dijkstra: " << endl;
    for (int i = 0; i<dist.size(); i++)
        cout << i << ": " << dist[i] << endl;

    Graph graph;
    for (int i = 0; i<V; i++)
        graph.insertVertex(i);

    for (int i = 0; i < V; i++) {
        vector<pair<int, int>> p;
        for (int j = 0; j< V; j++) {
            if (graph1[i][j] != 0) {
                p.push_back({graph1[i][j], j});
            }
        }
        sort(p.begin(), p.end());
        for (auto e: p) {
            graph.insertEdge(i, e.second, e.first);
        }
        cout << endl;
    }
    Dantzig dantzig(&graph);
    dantzig.SingleSource(0);
    cout << "dantzig: " << endl;
    dantzig.print();
}