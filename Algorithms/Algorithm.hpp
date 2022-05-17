#ifndef PFC_PROJECT_ALGORITHM_HPP
#define PFC_PROJECT_ALGORITHM_HPP

#include "../Classes/Graph.hpp"

#include <queue>

using pairType = pair<weightType, Edge*>;
using arrayType = vector<pairType>;
using mapType = multimap<weightType, Edge*>;
using pqType = priority_queue<pairType, arrayType, greater<>>;

class MoffatAndTakaoka;

template <typename candidateType>
class Algorithm{
protected:
    friend class MoffatAndTakaoka;

    Graph* graph;
    unordered_set<vertexIndex> S;
    unordered_map<vertexIndex, weightType> D;
    unordered_map<vertexIndex, size_t> currentUsefulEdge;
    candidateType candidateEdges;

    bool isUseful(vertexIndex t) { return S.find(t) == S.end(); }

    void setDefaultValues() {
        S.clear();
        for (auto index : graph->getVertexIndices()) {
            D[index] = 0;
            currentUsefulEdge[index] = 0;
        }
    }

    unordered_map<vertexIndex, weightType>  SingleSource(vertexIndex s, size_t n){
        assert(this->graph->findVertex(s) != nullptr);
        S.emplace(s);
        D[s] = 0;
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
    virtual unordered_map<vertexIndex, weightType> algorithmExpand(size_t limit) = 0;
    virtual Edge* getEdgeCandidate(vertexIndex s) = 0;

public:
    Algorithm() : graph(nullptr){}
    explicit Algorithm(Graph* _graph) : graph(_graph){
        graph->sortAdjacencyList();
        setDefaultValues();
    }
    explicit Algorithm(Graph* _graph, pqType& Uedges) : graph(_graph), candidateEdges(Uedges){
    }
};

#endif //PFC_PROJECT_ALGORITHM_HPP
