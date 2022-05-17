#ifndef PFC_PROJECT_MOFFATANDTAKAOKA_HPP
#define PFC_PROJECT_MOFFATANDTAKAOKA_HPP

#include "Dantzig.hpp"
#include "Spira.hpp"
#include <cmath>

class MoffatAndTakaoka {
private:
    Graph *graph;
    unordered_set<vertexIndex> V;
    ImprovedDantzig *dantzig;
    ImprovedSpira *spira;

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

    void initializeSpira(){
        auto setU = getSetU();
        spira = new ImprovedSpira(graph, setU);
        spira->S = dantzig->S;
        spira->D = dantzig->D;
        spira->candidateEdges = dantzig->candidateEdges;
        spira->currentUsefulEdge = dantzig->currentUsefulEdge;
        /*for(auto& v : V){
            spira->currentUsefulEdge[v] = 0;
        }*/
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
    explicit MoffatAndTakaoka(Graph* _graph): graph(_graph), V(_graph->getVertexIndices()){

    }

    unordered_map<vertexIndex, weightType> MoffatAndTakaokaAlgorithm(vertexIndex s){
        return FastSingleSource(s);
    }
};

#endif // PFC_PROJECT_MOFFATANDTAKAOKA_HPP