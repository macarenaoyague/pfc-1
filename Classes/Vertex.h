#ifndef PFC_PROJECT_VERTEX_H

#include "Edge.h"

class Vertex {
public:
    vector<Edge*> edges;
    vertexIndex idx;
    
    Vertex(){}
    explicit Vertex(vertexIndex _idx): idx(_idx){}
    void addEdge(Edge* edge){
        edges.push_back(edge);
    }

    void sortEdges(){
        if(edges.empty()) return;
        sort(edges.begin(), edges.end(), 
            [](Edge*& a, Edge*& b) -> bool
        { 
            return a->weight < b->weight;
        });
    }

    ~Vertex(){
        for(auto & edge : edges) delete edge;
        edges.clear();
    }
};

#endif // PFC_PROJECT_VERTEX_H