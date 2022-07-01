#pragma once
#include "../Classes/Graph.hpp"

#include <queue>

using pairType = pair<weightType, Edge*>;
using arrayType = vector<pairType>;
using mapType = multimap<weightType, Edge*>;
using pqType = priority_queue<pairType, arrayType, greater<>>;

class BaseAlgorithm{
public:
    pair<float, int> pushOperation;
    pair<float, int> popOperation;
    pair<float, int> cleanUpOperation;
    
    BaseAlgorithm()= default;
    virtual unordered_map<vertexIndex, weightType>* executeAlgorithm(vertexIndex s) = 0;
    virtual string getAlgorithmName() = 0;
    virtual ~BaseAlgorithm() = default;
};

template <typename candidateType>
class Algorithm : public BaseAlgorithm{
protected:
    Graph* graph;
    unordered_set<vertexIndex>* S;
    unordered_map<vertexIndex, weightType>* D;
    unordered_map<vertexIndex, size_t>* currentUsefulEdge;
    candidateType* candidateEdges;

    bool isUseful(vertexIndex t) { return S->find(t) == S->end(); }

    void setDefaultValues() {
        S->clear();
        for (auto index : graph->getVertexIndices()) {
            (*D)[index] = 0;
            (*currentUsefulEdge)[index] = 0;
        }
    }

    unordered_map<vertexIndex, weightType>* SingleSource(vertexIndex s, size_t n){
        assert(this->graph->findVertex(s) != nullptr);
        S->emplace(s);
        (*D)[s] = 0;
        addEdgeCandidate(s);
        return algorithmExpand(n);
    }

    void initializeValues(vertexIndex& c, vertexIndex& t, weightType& weight,
                          Edge* edge) {
        c = edge->start;
        t = edge->end;
        weight = edge->weight;
    }

    void addEdgeCandidate(vertexIndex s) {
        auto edgeCandidate = getEdgeCandidate(s);
        if (edgeCandidate != nullptr) {
            insertCandidate(edgeCandidate);
        }
    }

    virtual void insertCandidate(Edge* candidate) = 0;
    virtual Edge* getCandidateOfLeastWeight() = 0;
    virtual unordered_map<vertexIndex, weightType>* algorithmExpand(size_t limit) = 0;
    virtual Edge* getEdgeCandidate(vertexIndex s) = 0;

public:
    Algorithm() : graph(nullptr) {
        this->candidateEdges = new candidateType();
        this->S = new unordered_set<vertexIndex>();
        this->D = new unordered_map<vertexIndex, weightType>();
        this->currentUsefulEdge = new unordered_map<vertexIndex, size_t>();
    }
    explicit Algorithm(Graph* _graph) : graph(_graph){
        
        this->candidateEdges = new candidateType();
        this->S = new unordered_set<vertexIndex>();
        this->D = new unordered_map<vertexIndex, weightType>();
        this->currentUsefulEdge = new unordered_map<vertexIndex, size_t>();
        setDefaultValues();
    }
    explicit Algorithm(Graph* _graph, pqType*& Uedges) : graph(_graph), candidateEdges(Uedges){
        this->candidateEdges = Uedges;
        this->S = new unordered_set<vertexIndex>();
        this->D = new unordered_map<vertexIndex, weightType>();
        this->currentUsefulEdge = new unordered_map<vertexIndex, size_t>();
    }

    ~Algorithm() override{
        S->clear();
        D->clear();
        currentUsefulEdge->clear();
    }
};