#ifndef PFC_PROJECT_SPIRA_H
#define PFC_PROJECT_SPIRA_H

#include "Algorithm.hpp"

template<typename candidateType>
class Spira : public Algorithm<candidateType>{
protected:
    virtual void replaceUselessCandidate() = 0;
public:
    Spira(): Algorithm<candidateType>() {}
    explicit Spira(Graph* _graph) : Algorithm<candidateType>(_graph) {}
    explicit Spira(Graph* _graph, pqType& Uedges) : Algorithm<candidateType>(_graph, Uedges) {}
    unordered_map<vertexIndex, weightType> SpiraAlgorithm(vertexIndex s){
        return this->SingleSource(s, this->graph->getNumberOfVertices());
    }
};

class OriginalSpira : public Spira<mapType>{
private:
    Edge* getCandidateOfLeastWeight() override{
        return candidateEdges.begin()->second;
    }

    void insertCandidate(Edge* candidate) override{
        auto weight = D[candidate->start] + candidate->weight;
        candidateEdges.emplace(weight, candidate);
    }

    Edge* getEdgeCandidate(vertexIndex s) override {
        Edge* edgeUseful = nullptr;
        auto vertex = this->graph->findVertex(s);
        auto edges = vertex->edges;
        size_t i = this->currentUsefulEdge[s];
        if(i != edges.size()) edgeUseful = edges[i++];
        this->currentUsefulEdge[s] = i;
        return edgeUseful;
    }

    void replaceUselessCandidate() override {
        auto it = this->candidateEdges.begin();
        vertexIndex c = it->second->start;
        this->candidateEdges.erase(it);
        this->addEdgeCandidate(c);
    }

    unordered_map<vertexIndex, weightType> algorithmExpand(size_t limit) override{
        vertexIndex c, t, v;
        weightType weight;  // C(c, t)
        while (this->S.size() < limit) {
            this->initializeValues(c, t, weight, this->getCandidateOfLeastWeight());
            if (this->S.find(t) == this->S.end()) {
                this->S.emplace(t);
                this->D[t] = this->D[c] + weight;
                if (this->S.size() == limit) break;
                this->addEdgeCandidate(t);
            }
            replaceUselessCandidate();
        }
        return this->D;
    }

public:
    OriginalSpira() : Spira() {}
    explicit OriginalSpira(Graph* _graph): Spira(_graph) {}
};

class ImprovedSpira : public Spira<mapType>{
private:
    friend class MoffatAndTakaoka;
    unordered_set<vertexIndex> U;

    Edge* getCandidateOfLeastWeight() override{
        return nullptr;
    }

    void insertCandidate(Edge* candidate) override{
    }

    Edge* getEdgeCandidate(vertexIndex s) override {
        Edge* edgeUseful = nullptr;
        auto vertex = this->graph->findVertex(s);
        auto edges = vertex->edges;
        size_t i = this->currentUsefulEdge[s];
        for (; i < edges.size(); ++i) {
            if (this->U.find(edges[i]->end) != this->U.end()) {
                edgeUseful = edges[i];
                break;
            }
        }
        this->currentUsefulEdge[s] = i;
        return edgeUseful;
    }

    void updateCurrentUsefulEdge(vertexIndex s, bool findNext = true){
        size_t i = this->currentUsefulEdge[s];
        if(findNext){
            auto vertex = this->graph->findVertex(s);
            auto edges = vertex->edges;
            if(i != edges.size()) this->currentUsefulEdge[s] += 1;
        }
        getEdgeCandidate(s);
    }

    void replaceUselessCandidate() override {
    }

    bool isCurrentUsefulEdgeNull(vertexIndex s){
        auto vertex = this->graph->findVertex(s);
        auto edges = vertex->edges;
        return (currentUsefulEdge[s] == edges.size());
    }

    void procedure(vertexIndex v){
        vertexIndex c, t;
        weightType weight;
        while(!isCurrentUsefulEdgeNull(v)) {
            auto edgeCandidate = getEdgeCandidate(v);
            if(edgeCandidate == nullptr) break;
            initializeValues(c, t, weight, edgeCandidate);
            auto cost = D[c] + weight;
            if (D[t] == 0) D[t] = cost;
            else if (D[t] > cost) {
                D[t] = cost;
            }
            updateCurrentUsefulEdge(c);
            updateCurrentUsefulEdge(t, false);
        }
    }

    void reviewPreviousCandidates(){
        vertexIndex c, t;
        weightType weight;  // C(c, t)
        for(auto& item : candidateEdges){
            auto edgeCandidate = item.second;
            initializeValues(c, t, weight, edgeCandidate);
            if(isUseful(t)) {
                auto cost = D[c] + weight;
                if (D[t] == 0) D[t] = cost;
                else if (D[t] > cost) {
                    D[t] = cost;
                }
            }
        }
    }

    unordered_map<vertexIndex, weightType> algorithmExpand(size_t limit) override{
        reviewPreviousCandidates();
        for(auto& s : this->S){
            procedure(s);
        }
        for(auto& u : this->U){
            procedure(u);
        }
        return D;
    }
public:
    ImprovedSpira() : Spira<mapType>() {}
    explicit ImprovedSpira(Graph* _graph, unordered_set<vertexIndex>& setU): Spira<mapType>(_graph), U(setU){}
    //explicit ImprovedSpira(Graph* _graph, pqType& Uedges): Spira<pqType>(_graph, Uedges) {}
};
#endif  // PFC_PROJECT_SPIRA_H