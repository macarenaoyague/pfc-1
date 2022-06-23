#pragma once
#include "Dantzig.hpp"
#include "Spira.hpp"

#include <cmath>
#include <algorithm>
#include <boost/heap/fibonacci_heap.hpp>

using namespace boost::heap;

struct compareItem {
    bool operator()(const pairType& lhs, const pairType& rhs) const {
        return lhs.first > rhs.first;
    }
};

template<typename heap>
class MoffatAndTakaoka : public Algorithm<heap>{
protected:
    unordered_set<vertexIndex> purged;

    virtual void push(pairType edge) = 0;
    virtual Edge* pop() = 0;
    virtual void cleanUpHeap() = 0;

    Edge* getEdgeCandidate(vertexIndex s) override {
        // linked list optimizacion
        Edge* edgeUseful = nullptr;
        auto vertex = this->graph->findVertex(s);
        auto edges = vertex->edges;
        size_t i = (*(this->currentUsefulEdge))[s];
        for (; i < edges.size(); ++i) {
            if (this->purged.find(edges[i]->end) == this->purged.end()) {
                edgeUseful = edges[i++];
                break;
            }
        }
        (*(this->currentUsefulEdge))[s] = i;
        return edgeUseful;
    }

    void insertCandidate(Edge* candidate) override {
        auto weight = (*(this->D))[candidate->start] + candidate->weight;
        push({weight, candidate});
    }

    Edge* getCandidateOfLeastWeight() override {
        return pop();
    }

    unordered_map<vertexIndex, weightType>* algorithmExpand(size_t limit) override {
        vertexIndex c, t;
        weightType weight;  // C(c, t)
        while(this->S->size() < limit) {
            this->initializeValues(c, t, weight, this->getCandidateOfLeastWeight());
            this->addEdgeCandidate(c);
            if (this->S->find(t) == this->S->end()) {
                this->S->emplace(t);
                (*(this->D))[t] = (*(this->D))[c] + weight;
                this->addEdgeCandidate(t);
            }
        }
        return this->D; // necessary to avoid warning
    }

    unordered_map<vertexIndex, weightType>* fastSp(vertexIndex s) {
        assert(this->graph->findVertex(s) != nullptr);
        this->S->emplace(s);
        (*(this->D))[s] = 0;
        purged.emplace(s);
        this->addEdgeCandidate(s);
        size_t n = this->graph->getNumberOfVertices();
        for (size_t k = 1; k <= log2(log2(n)); k++) {
            // n div 2**k
            this->algorithmExpand(n - n/pow(2, k));
            this->cleanUpHeap();
        }
        this->algorithmExpand(n);
        return this->D;
    }

public:
    MoffatAndTakaoka(){}
    MoffatAndTakaoka(Graph* _graph): Algorithm<heap>(_graph) {}
};



class OriginalMoffatAndTakaoka : public MoffatAndTakaoka<arrayType>{
private:
    void push(pairType edge) override {
        candidateEdges->push_back(edge);
        push_heap(candidateEdges->begin(), candidateEdges->end(), std::greater<>{});
    }

    Edge* pop() override {
        if (candidateEdges->empty()) return nullptr;
        Edge* top = (candidateEdges->front()).second;
        pop_heap(candidateEdges->begin(), candidateEdges->end(), std::greater<>{});
        candidateEdges->pop_back();
        return top;
    }

    void cleanUpHeap() override {
        purged = *(this->S);
        erase_if(*(this->candidateEdges), [this](auto& item) -> bool {
            vertexIndex c, t;
            weightType weight;
            this->initializeValues(c, t, weight, item.second);
            if(this->S->find(t) != this->S->end()){
                auto edge = this->getEdgeCandidate(c);
                if(edge == nullptr) return true;
                item.second = edge;
                item.first = (*(this->D))[edge->start] + edge->weight;
                return false;
            }
            return false;
        });
        make_heap(this->candidateEdges->begin(), this->candidateEdges->end(), std::greater<>{});
    }

public:
    OriginalMoffatAndTakaoka() : MoffatAndTakaoka<arrayType>() {}

    explicit OriginalMoffatAndTakaoka(Graph* _graph) : MoffatAndTakaoka<arrayType>(_graph) {}

    string getAlgorithmName() override {
        return "Moffat and Takaoka Algorithm";
    }

    unordered_map<vertexIndex, weightType>* executeAlgorithm(vertexIndex s) override{
        return this->fastSp(s);
    }
};

template<typename boostHeap>
class BoostMoffatAndTakaoka : public MoffatAndTakaoka<boostHeap>{
private:
    void push(pairType item) override {
        this->candidateEdges->push(item);
    }

    Edge* pop() override {
        if (this->candidateEdges->empty()) return nullptr;
        Edge* top = (this->candidateEdges->top().second);
        this->candidateEdges->pop();
        return top;
    }

    void cleanUpHeap() override {
        this->purged = *(this->S);
        vector<typename boostHeap::handle_type> toUpdate, toRemove;
        vector<pairType> values;
        vertexIndex c, t;
        weightType weight;
        for(auto it = this->candidateEdges->begin(); it != this->candidateEdges->end(); ++it){
            auto item = *it;
            this->initializeValues(c, t, weight, item.second);
            if(this->S->find(t) != this->S->end()) {
                auto edge = this->getEdgeCandidate(c);
                auto handle = boostHeap::s_handle_from_iterator(it);
                if(edge == nullptr) {
                    toRemove.push_back(handle);
                    return;
                }
                weight = (*(this->D))[edge->start] + edge->weight;
                toUpdate.push_back(handle);
                values.push_back({weight, edge});
            }
        }

        for(size_t i = 0; i < toRemove.size(); ++i)
            this->candidateEdges->erase(toRemove[i]);

        for(size_t i = 0; i < toUpdate.size(); ++i)
            this->candidateEdges->update(toUpdate[i], values[i]);
    }

public:
    BoostMoffatAndTakaoka() : MoffatAndTakaoka<boostHeap>() {}

    explicit BoostMoffatAndTakaoka(Graph* _graph) : MoffatAndTakaoka<boostHeap>(_graph) {}

    string getAlgorithmName() override {
        return "Moffat and Takaoka Algorithm, with Boost";
    }

    unordered_map<vertexIndex, weightType>* executeAlgorithm(vertexIndex s) override{
        return this->fastSp(s);
    }
};