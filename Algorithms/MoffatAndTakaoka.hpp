#pragma once
#include "Dantzig.hpp"
#include "Spira.hpp"

#include <cmath>
#include <algorithm>

class MoffatAndTakaoka : public BaseAlgorithm {
private:
    Graph *graph;
    unordered_set<vertexIndex> V;
    ImprovedDantzig *dantzig;
    ImprovedSpira *spira;
    pqType* pq;

    unordered_set<vertexIndex> getSetU(){
        unordered_set<vertexIndex> U;
        for(auto& v : V){
            if(dantzig->S->find(v) == dantzig->S->end()) U.emplace(v);
        }
        return U;
    }

    void initializeDantzig(){
        dantzig = new ImprovedDantzig(graph);
    }

    void generateUedges(){

        arrayType elements;
        for (auto& edge: (*dantzig->candidateEdges)) {
            if (dantzig->isUseful(edge.second->end))
                elements.push_back({edge.first, edge.second});
                // pq->push({edge.first, edge.second});
        }

        auto S = dantzig->S;
        
        for(auto& s: (*S)) {
            auto vertex = this->graph->findVertex(s);
            auto edges = vertex->edges;
            size_t i = (*(dantzig->currentUsefulEdge))[s];
            Edge* edgeUseful = nullptr;
            for (; i < edges.size(); ++i) {
                if (dantzig->isUseful(edges[i]->end)) {
                    edgeUseful = edges[i];
                    auto weight = (*(dantzig->D))[edgeUseful->start] + edgeUseful->weight;
                    elements.push_back({weight, edgeUseful});
                    //pq->push({weight, edgeUseful});
                }
            }
        }
        
        pq = new pqType(elements.begin(), elements.end());

    }

    void initializeSpira(){
        auto setU = getSetU();
        generateUedges();
        spira = new ImprovedSpira(graph, pq);
        spira->D = dantzig->D;
        spira->currentUsefulEdge = dantzig->currentUsefulEdge;
        spira->U = setU;
    }

    unordered_map<vertexIndex, weightType>* FastSingleSource(vertexIndex s){
        size_t n = graph->getNumberOfVertices();
        initializeDantzig();
        dantzig->SingleSource(s, n - n/log2(n));
        initializeSpira();
        return spira->algorithmExpand(n);
    }

public:
    MoffatAndTakaoka()=default;
    explicit MoffatAndTakaoka(Graph* _graph): graph(_graph), V(_graph->getVertexIndices()),
                                                dantzig(nullptr), spira(nullptr){
    }

    unordered_map<vertexIndex, weightType>* executeAlgorithm(vertexIndex s) override{
        return FastSingleSource(s);
    }

    string getAlgorithmName() override{
        return "Moffat and Takaoka Algorithm";
    }

    ~MoffatAndTakaoka() override{
        delete spira;
        delete dantzig;
    }
};

class MoffatAndTakaokaIEEE : public Algorithm<arrayType>{
private:
    unordered_set<vertexIndex> purged;

    Edge* getEdgeCandidate(vertexIndex s) override {
        // linked list optimizacion
        Edge* edgeUseful = nullptr;
        auto vertex = this->graph->findVertex(s);
        auto edges = vertex->edges;
        bool isPurgued = false;
        while (!isPurgued) {
            size_t i = (*currentUsefulEdge)[s];
            if(i != edges.size()) edgeUseful = edges[i++];
            (*currentUsefulEdge)[s] = i;
        }
        return edgeUseful;
    }

    void insertCandidate(Edge* candidate) override {
        auto weight = (*D)[candidate->start] + candidate->weight;
        candidateEdges->push_back({weight, candidate}); 
        push_heap(candidateEdges->begin(), candidateEdges->end());
    }

    Edge* getCandidateOfLeastWeight() override {
        assert(candidateEdges->size() != 0);
        Edge* top = (candidateEdges->front()).second;
        pop_heap(candidateEdges->begin(), candidateEdges->end());
        candidateEdges->pop_back();
        return top;
    }
    
    unordered_map<vertexIndex, weightType>* algorithmExpand(size_t limit) override {
        vertexIndex c, t;
        weightType weight;  // C(c, t)
        while(S->size() < limit) {
            this->initializeValues(c, t, weight, this->getCandidateOfLeastWeight());
            addEdgeCandidate(c);
            if (this->S->find(t) == this->S->end()) {
                this->S->emplace(t);
                (*D)[t] = (*D)[c] + weight;
                addEdgeCandidate(t);
            }
        }
    }

    void cleanUpHeap() {
        purged = *S;
        erase_if(*(this->candidateEdges), [this](auto& item) -> bool{
            vertexIndex c, t;
            weightType weight;
            this->initializeValues(c, t, weight, item.second);
            if(S->find(t) != S->end()){
                auto edge = getEdgeCandidate(c);
                if(edge){
                    item.second = edge;
                    item.first = this->getDvector()[edge->start] + edge->weight;
                    return false;
                }
                return true;
            }
            return false;
        });
        make_heap(this->candidateEdges->begin(), this->candidateEdges->end());
    }

    void fastSp(vertexIndex s) {
        assert(this->graph->findVertex(s) != nullptr);
        S->emplace(s);
        (*D)[s] = 0;
        purged.emplace(s);
        size_t n = graph->getNumberOfVertices();
        for (size_t k = 1; k < log2(log2(n)); k++) {
            // n div 2**k
            this->algorithmExpand(n - n/(pow(2, k)));
            this->cleanUpHeap();
        }
        this->algorithmExpand(n);
    }

public:
    MoffatAndTakaokaIEEE(Graph* _graph) : Algorithm<arrayType>(_graph) {

    }

    unordered_map<vertexIndex, weightType>* getDvector() {
        return this->D;
    }

};