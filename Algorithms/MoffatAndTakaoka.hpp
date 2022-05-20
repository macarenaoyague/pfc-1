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
    pqType pq;

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
        auto S = dantzig->S;
        for(auto& s: S) {
            for (auto &u: setU) {
                auto edge = graph->findEdge(s, u);
                if(edge != nullptr) pq.push({dantzig->D[s] + edge->weight, edge});
            }
        }

        for(auto& u1: setU){
            for(auto& u2: setU){
                if(u1 != u2){
                    auto edge = graph->findEdge(u1, u2);
                    if(edge != nullptr) pq.push({dantzig->D[u1] + edge->weight, edge});
                }
            }
        }
    }

    void initializeSpira(){
        auto setU = getSetU();
        generateUedges(setU);
        spira = new ImprovedSpira(graph, pq);
        spira->S = dantzig->S;
        spira->D = dantzig->D;
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