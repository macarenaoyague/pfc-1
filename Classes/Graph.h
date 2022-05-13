#ifndef PFC_PROJECT_GRAPH_H
#include "Vertex.h"

class Graph {
private:
    vector<Edge*> E; // optional
    unordered_map <vertexIndex, Vertex*> V;
public:

    Graph(){}

    bool insertVertex(vertexIndex idx) {
        if(V.find(idx) == V.end()){
            auto newVertex = new Vertex(idx);
            V[idx] = newVertex;
            return true;
        }
        return false;
    }

    int getNumberOfVertices(){
        return V.size();
    }

    void insertEdge(vertexIndex idx1, vertexIndex idx2, weightType weight){
        if(V.find(idx1) != V.end() && V.find(idx2) != V.end()){
            auto newEdge = new Edge(idx1, idx2, weight);
            V[idx1]->addEdge(newEdge);
            E.push_back(newEdge); // optional
        }
    }
    
    size_t getVerticesNumber(){
        return V.size();
    }
    
    Vertex* findVertex(vertexIndex idx){
        if(V.find(idx) == V.end()) return nullptr;
        return V[idx];
    }

    void sortAdjacencyList(){
        //FIX: no funca
        return;
        /*
        for(auto item: V){
            item.second->sortEdges();
        }
        */
    } 

    void print() {
        for (auto v: V) {
            cout << v.second->idx << ": ";
            for (auto e: v.second->edges)
                cout << e->end << ":" << e->weight << " ";
            cout << endl;
        }
    }
    
    ~Graph(){
        for (auto v: V) {
            v.second->edges.clear();
            delete v.second;
        }
        V.clear();
        E.clear();
    }
};

#endif // PFC_PROJECT_GRAPH_H