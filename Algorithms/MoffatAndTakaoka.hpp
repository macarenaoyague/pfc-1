#pragma once
#include "Dantzig.hpp"
#include "Spira.hpp"

#include <cmath>

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
            if(dantzig->S.find(v) == dantzig->S.end()) U.emplace(v);
        }
        return U;
    }

    void initializeDantzig(){
        dantzig = new ImprovedDantzig(graph);
    }

    void generateUedges(unordered_set<vertexIndex> setU){

        for (auto& edge: (*dantzig->candidateEdges)) {
            if (dantzig->isUseful(edge.second->end))
                pq->push({edge.first, edge.second});
        }

        auto S = dantzig->S;
        
        for(auto& s: S) {
            auto vertex = this->graph->findVertex(s);
            auto edges = vertex->edges;
            size_t i = dantzig->currentUsefulEdge[s];
            Edge* edgeUseful = nullptr;
            for (; i < edges.size(); ++i) {
                if (dantzig->isUseful(edges[i]->end)) {
                    edgeUseful = edges[i];
                    auto weight = dantzig->D[edgeUseful->start] + edgeUseful->weight;
                    pq->push({weight, edgeUseful});
                }
            }
        }

    }

    void initializeSpira(){
        auto setU = getSetU();
        generateUedges(setU);
        spira = new ImprovedSpira(graph, pq);
        spira->D = dantzig->D;
        spira->currentUsefulEdge = dantzig->currentUsefulEdge;
        spira->U = setU;
    }

    unordered_map<vertexIndex, weightType> FastSingleSource(vertexIndex s){
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
        pq = new pqType();
    }

    unordered_map<vertexIndex, weightType> executeAlgorithm(vertexIndex s) override{
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