#pragma once
#include "Algorithm.hpp"

#include <algorithm>

template<typename candidateType>
class Dantzig : public Algorithm<candidateType>{
protected:
    Edge* getEdgeCandidate(vertexIndex s) override {
        Edge* edgeUseful = nullptr;
        auto vertex = this->graph->findVertex(s);
        auto edges = vertex->edges;
        size_t i = this->currentUsefulEdge[s];
        for (; i < edges.size(); ++i) {
            if (this->isUseful(edges[i]->end)) {
                edgeUseful = edges[i];
                break;
            }
        }
        this->currentUsefulEdge[s] = i;
        return edgeUseful;
    }

    void replaceUselessCandidates() {
        vector<vertexIndex> insertCandidatesUsefulFromV;
        erase_if(*(this->candidateEdges), [this, &insertCandidatesUsefulFromV](auto& item) -> bool{
            vertexIndex t = item.second->end;
            if(!this->isUseful(t)){
                vertexIndex v = item.second->start;
                insertCandidatesUsefulFromV.emplace_back(v);
                return true;
            }
            return false;
        });

        for(auto& v : insertCandidatesUsefulFromV)
            this->addEdgeCandidate(v);
    }

    unordered_map<vertexIndex, weightType> algorithmExpand(size_t limit) override{
        vertexIndex c, t, v;
        weightType weight;  // C(c, t)
        while (this->S->size() < limit) {
            this->initializeValues(c, t, weight, this->getCandidateOfLeastWeight());
            this->S->emplace(t);
            this->D[t] = this->D[c] + weight;
            if (this->S->size() == limit) break;
            this->addEdgeCandidate(t);
            replaceUselessCandidates();
        }
        return this->D;
    }

public:
    Dantzig(): Algorithm<candidateType>() {}
    explicit Dantzig(Graph* _graph) : Algorithm<candidateType>(_graph) {};
};


class OriginalDantzig : public Dantzig<arrayType>{
private:
    Edge* getCandidateOfLeastWeight() override{
        return min_element(candidateEdges->begin(), candidateEdges->end(), [](const auto& lhs, const auto& rhs) {
            return lhs.first < rhs.first;
        })->second;
    }

    void insertCandidate(Edge* candidate) override{
        auto weight = D[candidate->start] + candidate->weight;
        candidateEdges->emplace_back(weight, candidate);
    }

public:
    OriginalDantzig()= default;
    explicit OriginalDantzig(Graph* graph): Dantzig<arrayType>(graph) {
    }
    string getAlgorithmName() override{
        return "Original Dantzig Algorithm";
    }
};


class ImprovedDantzig : public Dantzig<mapType>{
private:
    Edge* getCandidateOfLeastWeight() override{
        return candidateEdges->begin()->second;
    }
    void insertCandidate(Edge* candidate) override{
        auto weight = D[candidate->start] + candidate->weight;
        candidateEdges->emplace(weight, candidate);
    }
public:
    ImprovedDantzig()= default;
    explicit ImprovedDantzig(Graph* graph): Dantzig<mapType>(graph){}
    string getAlgorithmName() override{
        return "Improved Dantzig Algorithm";
    }
};