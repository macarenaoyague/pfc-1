#ifndef PFC_PROJECT_ALGORITHM_H
#define PFC_PROJECT_ALGORITHM_H
#include "Classes/Graph.h"
#include <unordered_set>
#include <queue>


class Algorithm {
private:
    enum algorithmType {Dantzig, Spira};
    Graph* graph;
    unordered_set<vertexIndex> S;
    unordered_map<vertexIndex, weightType> D;
    multimap<weightType, Edge*> candidateEdges;
    unordered_map<vertexIndex, size_t> currentUsefulEdge;

    bool isUseful(vertexIndex t) {
        return S.find(t) == S.end();
    }

    Edge* getEdgeCandidate(vertexIndex s, algorithmType algorithm){
        Edge* edgeUseful = nullptr;
        auto vertex = graph->findVertex(s);
        auto edges = vertex->edges;
        size_t i = currentUsefulEdge[s];
        if(algorithm == Spira){
            if(i != edges.size()){
                edgeUseful = edges[i++];
            }
        }else{
            for(;i < edges.size(); ++i) {
                if (isUseful(edges[i]->end)) {
                    edgeUseful = edges[i];
                    break;
                }
            }
        }
        currentUsefulEdge[s] = i;
        return edgeUseful;
    }

    void addEdgeCandidate(vertexIndex s, algorithmType algorithm){
        auto edgeCandidate = getEdgeCandidate(s, algorithm);
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
            t = it->second->end;
            if(!isUseful(t)){
                v = it->second->start;
                toRemove.push_back(it);
                addEdgeCandidate(v, Dantzig);
            }
        }
        for(auto it : toRemove){
            candidateEdges.erase(it);
        }
    }

    void replaceUselessCandidate(){
        auto it = candidateEdges.begin();
        vertexIndex c = it->second->start;
        candidateEdges.erase(it);
        addEdgeCandidate(c, Spira);
    }

public:
    Algorithm() = default;
    explicit Algorithm(Graph* graph) {
        graph->sortAdjacencyList();
        for(auto index : graph->getVertexIndices()) {
            D[index] = 0;
            currentUsefulEdge[index] = 0;
        }
        this->graph = graph;
    }

    unordered_map<vertexIndex, weightType> DantzigAlgorithm(vertexIndex s){
        return SingleSource(s, Dantzig);
    }

    unordered_map<vertexIndex, weightType> SpiraAlgorithm(vertexIndex s){
        return SingleSource(s, Spira);
    }

    unordered_map<vertexIndex, weightType> SingleSource(vertexIndex s, algorithmType algorithm) {
        assert(graph->findVertex(s) != nullptr);
        S.emplace(s);
        D[s] = 0;
        addEdgeCandidate(s, algorithm);
        if(algorithm == Dantzig) return DantzigExpand((int) graph->getNumberOfVertices());
        return SpiraExpand((int) graph->getNumberOfVertices());
    }

    unordered_map<vertexIndex, weightType> DantzigExpand(int limit){
        vertexIndex c, t, v;
        weightType weight; // C(c, t)
        while(S.size() < limit){
            initializeValues(c, t, weight, candidateEdges.begin());
            S.emplace(t);
            D[t] = D[c] + weight;
            if(S.size() == limit) break;
            addEdgeCandidate(t, Dantzig);
            replaceUselessCandidates();
        }
        return D;
    }

    unordered_map<vertexIndex, weightType> SpiraExpand(int limit){
        vertexIndex c, t, v;
        weightType weight; // C(c, t)
        while(S.size() < limit) {
            initializeValues(c, t, weight, candidateEdges.begin());
            if(S.find(t) == S.end()){
                S.emplace(t);
                D[t] = D[c] + weight;
                if(S.size() == limit) break;
                addEdgeCandidate(t, Spira);
            }
            replaceUselessCandidate();
        }
        return D;
    }

    void print() {
        for (auto e: D)
            cout << e.first << ": " << e.second << endl;
    }

};

#endif //PFC_PROJECT_ALGORITHM_H