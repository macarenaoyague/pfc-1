#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

typedef unsigned vertexIndex;
typedef unsigned weightType;


class Vertex {
public:
    vector<Edge*> edges;
    vertexIndex idx;
    
    Vertex(){}
    Vertex(vertexIndex _idx): idx(_idx){}
    void addEdge(Edge* edge){
        edges.push_back(edge);
    }
    void sortEdges(){
        if(edges.empty()) return;
        sort(edges.begin(), edges.end(), 
            [](const Edge*& a, const Edge*& b) -> bool
        { 
            return a->weight > b->weight; 
        });
    }

};

class Edge {
public:
    vertexIndex start;
    vertexIndex end;
    weightType weight;
    
    Edge(): start(0), end(0), weight(-1){}
    Edge(vertexIndex _start, vertexIndex _end, weightType _weight):
        start(_start), end(_end), weight(_weight){}
};

class Graph {
private:
    vector<Edge*> E; // optional
    unordered_map <vertexIndex, Vertex*> V;
public:
    Graph(){

    }
    void insertVertex(vertexIndex idx) {
        if(V.find(idx) == V.end()){
            auto newVertex = new Vertex(idx);
            V[idx] = newVertex;
        }
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
        for(auto item: V){
            item.second->sortEdges();
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