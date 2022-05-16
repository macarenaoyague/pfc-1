#ifndef PFC_PROJECT_SPIRA_H
#define PFC_PROJECT_SPIRA_H

#include "Algorithm.hpp"

class Spira : public Algorithm<mapType>{
protected:
    Edge* getEdgeCandidate(vertexIndex s) override {
        Edge* edgeUseful = nullptr;
        auto vertex = this->graph->findVertex(s);
        auto edges = vertex->edges;
        size_t i = this->currentUsefulEdge[s];
        if(i != edges.size()) edgeUseful = edges[i++];
        this->currentUsefulEdge[s] = i;
        return edgeUseful;
    }

    void replaceUselessCandidate() {
        auto it = this->candidateEdges.begin();
        vertexIndex c = it->second->start;
        this->candidateEdges.erase(it);
        this->addEdgeCandidate(c);
    }

    unordered_map<vertexIndex, weightType> algorithmExpand(int limit) override{
        vertexIndex c, t, v;
        weightType weight;  // C(c, t)
        while (this->S.size() < limit) {
            initializeValues(c, t, weight, this->candidateEdges.begin());
            if (this->S.find(t) == this->S.end()) {
                this->S.emplace(t);
                this->D[t] = this->D[c] + weight;
                if (this->S.size() == limit) break;
                this->addEdgeCandidate(t);
            }
            replaceUselessCandidate();
        }
        return this->D;
    }
public:
    Spira(): Algorithm<mapType>() {}
    explicit Spira(Graph* _graph) : Algorithm<mapType>(_graph) {};
    unordered_map<vertexIndex, weightType> SpiraAlgorithm(vertexIndex s){
        return this->SingleSource(s);
    }
};

class OriginalSpira : public Spira{
protected:
    mapType::iterator getCandidateOfLeastWeight() override{
        return candidateEdges.begin();
    }
    void insertCandidate(Edge* candidate) override{
        auto weight = D[candidate->start] + candidate->weight;
        candidateEdges.emplace(weight, candidate);
    }

public:
    OriginalSpira() : Spira() {}
    explicit OriginalSpira(Graph* _graph): Spira(_graph) {}
};
#endif  // PFC_PROJECT_SPIRA_H