#ifndef PFC_PROJECT_SPIRA_H
#define PFC_PROJECT_SPIRA_H

#include "Algorithm.hpp"

template<typename candidateType>
class Spira : public Algorithm<candidateType>{
protected:
    virtual void replaceUselessCandidate() = 0;
public:
    Spira(): Algorithm<candidateType>() {}
    explicit Spira(Graph* _graph) : Algorithm<candidateType>(_graph) {}
    explicit Spira(Graph* _graph, pqType& Uedges) : Algorithm<candidateType>(_graph, Uedges) {}
    unordered_map<vertexIndex, weightType> SpiraAlgorithm(vertexIndex s){
        return this->SingleSource(s, this->graph->getNumberOfVertices());
    }
};

class OriginalSpira : public Spira<mapType>{
private:
    Edge* getCandidateOfLeastWeight() override{
        return candidateEdges.begin()->second;
    }

    void insertCandidate(Edge* candidate) override{
        auto weight = D[candidate->start] + candidate->weight;
        candidateEdges.emplace(weight, candidate);
    }

    Edge* getEdgeCandidate(vertexIndex s) override {
        Edge* edgeUseful = nullptr;
        auto vertex = this->graph->findVertex(s);
        auto edges = vertex->edges;
        size_t i = this->currentUsefulEdge[s];
        if(i != edges.size()) edgeUseful = edges[i++];
        this->currentUsefulEdge[s] = i;
        return edgeUseful;
    }

    void replaceUselessCandidate() override {
        auto it = this->candidateEdges.begin();
        vertexIndex c = it->second->start;
        this->candidateEdges.erase(it);
        this->addEdgeCandidate(c);
    }

    unordered_map<vertexIndex, weightType> algorithmExpand(size_t limit) override{
        vertexIndex c, t, v;
        weightType weight;  // C(c, t)
        while (this->S.size() < limit) {
            this->initializeValues(c, t, weight, this->getCandidateOfLeastWeight());
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
    OriginalSpira() : Spira() {}
    explicit OriginalSpira(Graph* _graph): Spira(_graph) {}
};

class ImprovedSpira : public Spira<pqType>{
private:
    friend class MoffatAndTakaoka;

    Edge* getCandidateOfLeastWeight() override{
        return candidateEdges.top().second;
    }

    void insertCandidate(Edge* candidate) override{
        auto weight = D[candidate->start] + candidate->weight;
        candidateEdges.emplace(weight, candidate);
    }

    Edge* getEdgeCandidate(vertexIndex s) override {
        return nullptr;
    }

    void replaceUselessCandidate() override {
        this->candidateEdges.pop();
    }

    void lastRevision(unordered_set<Edge*>& aux){
        vertexIndex a, b;
        while(!aux.empty()){
            unordered_set<Edge*> temp;
            for(auto& edge: aux){
                a = edge->start, b = edge->end;
                if(S.find(a) != S.end()){
                    auto minCost = D[a] + edge->weight;
                    if(D[b] == 0) {
                        S.emplace(b);
                        D[b] = minCost;
                    }
                    else if(D[b] > minCost) D[b] = minCost;
                }else{
                    temp.emplace(edge);
                }
            }
            aux = temp;
        }
    }

    unordered_map<vertexIndex, weightType> algorithmExpand(size_t limit) override{
        vertexIndex c, t;
        weightType weight;  // C(c, t)
        unordered_set<Edge*> aux;
        while (this->S.size() < limit && !candidateEdges.empty()) {
            this->initializeValues(c, t, weight, this->getCandidateOfLeastWeight());
            if(this->S.find(c) != this->S.end()){
                if (this->S.find(t) == this->S.end()) {
                    this->S.emplace(t);
                    this->D[t] = this->D[c] + weight;
                    if (this->S.size() == limit) break;
                }
                replaceUselessCandidate();
            }else{
                aux.emplace(candidateEdges.top().second);
                candidateEdges.pop();
            }
        }
        lastRevision(aux);
        return D;
    }
public:
    ImprovedSpira() : Spira<pqType>() {}
    explicit ImprovedSpira(Graph* _graph, pqType& Uedges): Spira<pqType>(_graph, Uedges) {}
};
#endif  // PFC_PROJECT_SPIRA_H