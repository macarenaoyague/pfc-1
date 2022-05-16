#ifndef PFC_PROJECT_SPIRA_H

#include "../Classes/Graph.hpp"

class Spira {
protected:
    Graph *graph;
    unordered_set <vertexIndex> S;
    unordered_map <vertexIndex, weightType> D;
    unordered_map <vertexIndex, size_t> currentUsefulEdge;
    multimap<weightType, Edge*> candidateEdges;

    void setDefaultValues() {
        S.clear();
        for (auto index : graph->getVertexIndices()) {
            D[index] = 0;
            currentUsefulEdge[index] = 0;
        }
    }
    void restart(){
        setDefaultValues();
        candidateEdges.clear();
    }
}

#endif  // PFC_PROJECT_SPIRA_H