#include "Graph.h"
#include <set>

class Dantzig {
private:
    Graph* graph;

    // nodos ya visitados (incluídos en D)
    set<vertexIndex> S;
    unordered_map <vertexIndex, unsigned> D;
    set<vertexIndex> T;

    bool isUseful(vertexIndex t) {
        return S.find(t) != S.end();
    }

    void sortAdjacencyList() {
        graph->sortAdjacencyList();
    }

public:
    Dantzig(Graph* graph) {
        this->graph = graph;
    }

    void SingleSource(vertexIndex s) {
        // ordenar todas las listas de aristas de los vertices del grafo
        sortAdjacencyList();
        S.insert(s);
        D[s] = 0;
        Vertex* cVertex;
        vertexIndex t; 
        for (vertexIndex c: S) {
            cVertex = graph->findVertex(c);
            if (cVertex == nullptr) continue;
            for (auto edge: cVertex->edges) {
                t = edge->end;
                if (isUseful(t)) {
                    T.insert(t);
                }
            }
        }
    }
};
