#pragma once
#include "Algorithm.hpp"

template<typename candidateType>
class Spira : public Algorithm<candidateType>{
protected:
    virtual void replaceUselessCandidate() = 0;
public:
    Spira(): Algorithm<candidateType>() {
    }
    explicit Spira(Graph* _graph) : Algorithm<candidateType>(_graph) {
    }
    explicit Spira(Graph* _graph, pqType*& Uedges) : Algorithm<candidateType>(_graph, Uedges) {}
    unordered_map<vertexIndex, weightType>* executeAlgorithm(vertexIndex s) override{
        return this->SingleSource(s, this->graph->getNumberOfVertices());
    }
};

class OriginalSpira : public Spira<mapType>{
private:
    Edge* getCandidateOfLeastWeight() override{
        return candidateEdges->begin()->second;
    }

    void insertCandidate(Edge* candidate) override{
        auto weight = (*D)[candidate->start] + candidate->weight;
        candidateEdges->emplace(weight, candidate);
    }

    Edge* getEdgeCandidate(vertexIndex s) override {
        Edge* edgeUseful = nullptr;
        auto vertex = this->graph->findVertex(s);
        auto edges = vertex->edges;
        size_t i = (*currentUsefulEdge)[s];
        if(i != edges.size()) edgeUseful = edges[i++];
        (*currentUsefulEdge)[s] = i;
        return edgeUseful;
    }

    void replaceUselessCandidate() override {
        auto it = this->candidateEdges->begin();
        vertexIndex c = it->second->start;
        this->candidateEdges->erase(it);
        this->addEdgeCandidate(c);
    }

    unordered_map<vertexIndex, weightType>* algorithmExpand(size_t limit) override{
        vertexIndex c, t, v;
        weightType weight;  // C(c, t)
        while (this->S->size() < limit) {
            this->initializeValues(c, t, weight, this->getCandidateOfLeastWeight());
            if (this->S->find(t) == this->S->end()) {
                this->S->emplace(t);
                (*D)[t] = (*D)[c] + weight;
                if (this->S->size() == limit) break;
                this->addEdgeCandidate(t);
            }
            replaceUselessCandidate();
        }
        return this->D;
    }

public:
    OriginalSpira() : Spira() {}
    explicit OriginalSpira(Graph* _graph): Spira(_graph) {
    }
    string getAlgorithmName() override{
        return "Original Spira Algorithm";
    }
};

class ImprovedSpira : public Spira<pqType>{
private:
    friend class MoffatAndTakaoka;

    unordered_set<vertexIndex> U;

    Edge* getCandidateOfLeastWeight() override{
        return candidateEdges->top().second;
    }

    void insertCandidate(Edge* candidate) override{
        auto weight = (*D)[candidate->start] + candidate->weight;
        candidateEdges->emplace(weight, candidate);
    }

    Edge* getEdgeCandidate(vertexIndex s) override {
        Edge* edge = nullptr;
        auto vertex = this->graph->findVertex(s);
        auto edges = vertex->edges;
        size_t i = (*currentUsefulEdge)[s];
        
        
        // if(i != edges.size()) edge = edges[i++];
        // (*(this->currentUsefulEdge))[s] = i;
        // return edge;
        
        while (i < edges.size()) {
            edge = edges[i++];
            auto t = edge->end;
            if(this->S->find(t) == this->S->end() && 
                this->U.find(t) != this->U.end()) break;
        }

        (*currentUsefulEdge)[s] = i;

        if (i != edges.size()) return edge;
        else return nullptr;
        
    }

    void replaceUselessCandidate() override {
        auto it = this->candidateEdges->top();
        vertexIndex c = it.second->start;
        this->candidateEdges->pop();
        if (this->U.find(c) != this->U.end()) {
            this->addEdgeCandidate(c);
        }
    }

    unordered_map<vertexIndex, weightType>* algorithmExpand(size_t limit) override{
        vertexIndex c, t, v;
        weightType weight;  // C(c, t)
        int uSize = U.size();
        while (this->S->size() < uSize) {
            this->initializeValues(c, t, weight, this->getCandidateOfLeastWeight());
            if (this->S->find(t) == this->S->end() && 
                this->U.find(t) != this->U.end()) {
                this->S->emplace(t);
                (*D)[t] = (*D)[c] + weight;
                if (this->S->size() == uSize) break;
                this->addEdgeCandidate(t);
            }
            replaceUselessCandidate();
        }
        return this->D;
    }
public:
    ImprovedSpira() : Spira<pqType>() {}
    explicit ImprovedSpira(Graph* _graph, pqType*& Uedges): Spira<pqType>(_graph, Uedges) {}
    string getAlgorithmName() override{
        return "Improved Spira Algorithm";
    }
};