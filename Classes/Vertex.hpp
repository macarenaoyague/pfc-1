#pragma once
#include "Edge.hpp"

class Vertex {
public:
    vertexIndex idx;
    vector<Edge*> edges;
    unordered_map<vertexIndex, Edge*> mappedEdges;

    Vertex() {}
    explicit Vertex(vertexIndex _idx) : idx(_idx) {}
    void addEdge(Edge* edge, bool toMap) {
        edges.push_back(edge);
        if(toMap) mappedEdges[edge->end] = edge;
    }

    Edge* findEdge(vertexIndex end){
        Edge* result = nullptr;
        if(!mappedEdges.empty()){
            auto it = mappedEdges.find(end);
            if(it != mappedEdges.end()) return it->second;
        }
        else{
            for(auto& edge : edges)
                if(edge->end == end) return edge;
        }
        return result;
    }

    void sortEdges() {
        if (edges.empty()) return;
        sort(edges.begin(), edges.end(),
             [](Edge*& a, Edge*& b) -> bool { return a->weight < b->weight; });
    }

    ~Vertex() {
        for (auto& edge : edges) delete edge;
        edges.clear();
    }
};