#pragma once
#include "Dantzig.hpp"
#include "Spira.hpp"

#include <cmath>
#include <algorithm>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/heap/binomial_heap.hpp>
#include <chrono>
#include <fstream>

using namespace boost::heap;

struct compareItem {
    bool operator()(const pairType& lhs, const pairType& rhs) const {
        return lhs.first > rhs.first;
    }
};

typedef std::chrono::time_point<std::chrono::system_clock> chronoTime;

template<typename heap>
class MoffatAndTakaoka : public Algorithm<heap>{
protected:
    unordered_set<vertexIndex> purged;

    virtual void push(pairType edge) = 0;
    virtual Edge* pop() = 0;
    virtual void cleanUpHeap() = 0;
    
    chronoTime start() {
        return chrono::system_clock::now();
    }

    void end(pair<float, int>& variableTime, chronoTime start) {
        auto end_ = chrono::system_clock::now();
        std::chrono::duration<float,std::milli> duration = end_ - start;
        variableTime.first += duration.count();
        variableTime.second++;
    }

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

    vector<float> getTimes(){
        return {this->pushOperation.first, this->popOperation.first, this->cleanUpOperation.first};
    }

    vector<int> getCounter(){
        return {this->pushOperation.second, this->popOperation.second, this->cleanUpOperation.second};
    }

public:
    MoffatAndTakaoka(){}
    MoffatAndTakaoka(Graph* _graph): Algorithm<heap>(_graph) {
        this->pushOperation.first = this->popOperation.first = this->cleanUpOperation.first = 0;
        this->pushOperation.second = this->popOperation.second = this->cleanUpOperation.second = 0;
    }
};



class OriginalMoffatAndTakaoka : public MoffatAndTakaoka<arrayType>{
private:
    void push(pairType edge) override {
        chronoTime start = this->start();
        candidateEdges->push_back(edge);
        push_heap(candidateEdges->begin(), candidateEdges->end(), std::greater<>{});
        this->end(this->pushOperation, start);
    }

    Edge* pop() override {
        chronoTime start = this->start();
        if (candidateEdges->empty()) return nullptr;
        Edge* top = (candidateEdges->front()).second;
        pop_heap(candidateEdges->begin(), candidateEdges->end(), std::greater<>{});
        candidateEdges->pop_back();
        this->end(this->popOperation, start);
        return top;
    }

    void cleanUpHeap() override {
        chronoTime start = this->start();
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
        this->end(this->cleanUpOperation, start);
    }

public:
    OriginalMoffatAndTakaoka() : MoffatAndTakaoka<arrayType>() {}

    explicit OriginalMoffatAndTakaoka(Graph* _graph) : MoffatAndTakaoka<arrayType>(_graph) {}

    string getAlgorithmName() override {
        return "Moffat&TakaokaAlgorithm";
    }

    unordered_map<vertexIndex, weightType>* executeAlgorithm(vertexIndex s) override{
        return this->fastSp(s);
    }
};

template<typename boostHeap>
class BoostMoffatAndTakaoka : public MoffatAndTakaoka<boostHeap>{
private:
    void push(pairType item) override {
        chronoTime start = this->start();
        this->candidateEdges->push(item);
        this->end(this->pushOperation, start);
    }

    Edge* pop() override {
        chronoTime start = this->start();
        // if (this->candidateEdges->empty()) return nullptr;
        Edge* top = (this->candidateEdges->top().second);
        this->candidateEdges->pop();
        this->end(this->popOperation, start);
        return top;
    }

    void cleanUpHeap() override {
        chronoTime start = this->start();
        this->purged = *(this->S);
        vector<typename boostHeap::handle_type> toRemove;
        vector<pair<typename boostHeap::handle_type, pairType>> toUpdate;
        vertexIndex c, t;
        weightType weight;
        for(auto it = this->candidateEdges->begin(); it != this->candidateEdges->end(); ++it){
            auto item = *it;
            this->initializeValues(c, t, weight, item.second);
            if(this->S->find(t) != this->S->end()) {
                auto edge = this->getEdgeCandidate(c);
                auto handle = boostHeap::s_handle_from_iterator(it);
                if(edge == nullptr) {
                    toRemove.emplace_back(handle);
                    continue;
                }
                weight = (*(this->D))[edge->start] + edge->weight;
                toUpdate.emplace_back(handle, make_pair(weight, edge));
            }
        }

        for(size_t i = 0; i < toRemove.size(); ++i)
            this->candidateEdges->erase(toRemove[i]);

        for(size_t i = 0; i < toUpdate.size(); ++i)
            this->candidateEdges->update(toUpdate[i].first, toUpdate[i].second);
        this->end(this->cleanUpOperation, start);
    }

public:
    BoostMoffatAndTakaoka() : MoffatAndTakaoka<boostHeap>() {}

    explicit BoostMoffatAndTakaoka(Graph* _graph) : MoffatAndTakaoka<boostHeap>(_graph) {}

    string getAlgorithmName() override {
        return "Moffat&TakaokaAlgorithmBoost";
    }

    unordered_map<vertexIndex, weightType>* executeAlgorithm(vertexIndex s) override{
        return this->fastSp(s);
    }
};