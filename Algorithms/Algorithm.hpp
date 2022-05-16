#ifndef PFC_PROJECT_ALGORITHM_HPP
#define PFC_PROJECT_ALGORITHM_HPP

#include "../Classes/Graph.hpp"

using arrayType = vector<pair<weightType, Edge*>>;
using mapType = multimap<weightType, Edge*>;

template <typename candidateType>
class Algorithm{
protected:
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

    unordered_map<vertexIndex, weightType> SingleSource(vertexIndex s){
        assert(this->graph->findVertex(s) != nullptr);
        S.emplace(s);
        D[s] = 0;
        addEdgeCandidate(s);
        return algorithmExpand((int)graph->getNumberOfVertices());
    }

    void initializeValues(vertexIndex& c, vertexIndex& t, weightType& weight,
                          const typename candidateType::iterator& item) {
        c = item->second->start;
        t = item->second->end;
        weight = item->second->weight;
    }

    void restart(){
        this->setDefaultValues();
        this->candidateEdges.clear();
    }

    void addEdgeCandidate(vertexIndex s) {
        auto edgeCandidate = getEdgeCandidate(s);
        if (edgeCandidate != nullptr) {
            insertCandidate(edgeCandidate);
        }
    }

    virtual void insertCandidate(Edge* candidate) = 0;
    virtual typename candidateType::iterator getCandidateOfLeastWeight() = 0;
    virtual unordered_map<vertexIndex, weightType> algorithmExpand(int limit) = 0;
    virtual Edge* getEdgeCandidate(vertexIndex s) = 0;

public:
    Algorithm() : graph(nullptr){}
    explicit Algorithm(Graph* _graph) : graph(_graph){
        graph->sortAdjacencyList();
        setDefaultValues();
    }
};

#endif //PFC_PROJECT_ALGORITHM_HPP
