#ifndef PFC_PROJECT_VERTEX_H

#include "Edge.h"

class Vertex {
public:

    // refactor, cambiar las edges a aestto
    map <vertexIndex, weightType> edgesMAP;
    
    vector<Edge*> edges;
    vertexIndex idx;
    
    Vertex(){}
    Vertex(vertexIndex _idx): idx(_idx){}
    void addEdge(Edge* edge){
        edges.push_back(edge);
    }

    /*
    FIX: no funciona

    void sortEdges(){
        if(edges.empty()) return;
        sort(edges.begin(), edges.end(), 
            [](const Edge*& a, const Edge*& b) -> bool
        { 
            return a->weight > b->weight; 
        });
    }
    */
};

#endif // PFC_PROJECT_VERTEX_H