#ifndef PFC_PROJECT_DANTZIGV2_H
#define PFC_PROJECT_DANTZIGV2_H
#include "Classes/Graph.h"
#include <unordered_set>
#include <queue>


class Dantzig {
private:
    Graph* graph;
    unordered_set<vertexIndex> S;
    unordered_map<vertexIndex, weightType> D;
    multimap<weightType, Edge*> candidateEdges;
    unordered_map<vertexIndex, size_t> currentUsefulEdge;

    bool isUseful(vertexIndex t) {
        return S.find(t) == S.end();
    }

    Edge* getEdgeCandidate(vertexIndex s){
        Edge* edgeUseful = nullptr;
        size_t i;
        auto vertex = graph->findVertex(s);
        auto edges = vertex->edges;
        for(i = currentUsefulEdge[s]; i < edges.size(); ++i) {
            if (isUseful(edges[i]->end)) {
                edgeUseful = edges[i];
                break;
            }
        }
        currentUsefulEdge[s] = i;
        return edgeUseful;
    }

    void addEdgeCandidate(vertexIndex s){
        auto edgeCandidate = getEdgeCandidate(s);
        if(edgeCandidate != nullptr) {
            auto weight = edgeCandidate->weight;
            candidateEdges.emplace(D[s] + weight, edgeCandidate);
        }
    }

    void initializeValues(vertexIndex& c, vertexIndex& t, weightType& weight, const map<weightType, Edge*>::iterator& item){
        c = item->second->start;
        t = item->second->end;
        weight = item->second->weight;
    }

    void replaceUselessCandidates(){
        vector<map<weightType, Edge*>::iterator> toRemove;
        vertexIndex v, t;
        for(auto it = candidateEdges.begin(); it != candidateEdges.end(); ++it){
            auto edge = it->second;
            t = edge->end;
            if(!isUseful(t)){
                v = it->second->start;
                toRemove.push_back(it);
                addEdgeCandidate(v);
            }
        }
        for(auto it : toRemove){
            candidateEdges.erase(it);
        }
    }

public:
    Dantzig() = default;
    explicit Dantzig(Graph* graph) {
        graph->sortAdjacencyList();
        for(auto index : graph->getVertexIndices()) {
            D[index] = 0;
            currentUsefulEdge[index] = 0;
        }
        this->graph = graph;
    }

    unordered_map<vertexIndex, weightType> SingleSource(vertexIndex s) {
        assert(graph->findVertex(s) != nullptr);
        S.emplace(s);
        D[s] = 0;
        addEdgeCandidate(s);
        return DantzigExpand((int) graph->getNumberOfVertices());
    }

    unordered_map<vertexIndex, weightType> DantzigExpand(int limit){
        vertexIndex c, t, v;
        weightType weight; // C(c, t)
        while(S.size() < limit){
            initializeValues(c, t, weight, candidateEdges.begin());
            S.emplace(t);
            D[t] = D[c] + weight;
            if(S.size() == limit) break;
            addEdgeCandidate(t);
            replaceUselessCandidates();
        }
        return D;
    }

    void print() {
        for (auto e: D)
            cout << e.first << ": " << e.second << endl;
    }

};

#endif //PFC_PROJECT_DANTZIGV2_H