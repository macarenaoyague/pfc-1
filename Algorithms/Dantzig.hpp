#ifndef PFC_PROJECT_DANTZIG_H

using arrayType = vector<pair<weightType, Edge*>>;
using mapType = multimap<weightType, Edge*>;

template<typename candidateType>
class Dantzig{
protected:
    Graph* graph;
    unordered_set<vertexIndex> S;
    unordered_map<vertexIndex, weightType> D;
    unordered_map<vertexIndex, size_t> currentUsefulEdge;
    candidateType candidateEdges;

    void setDefaultValues() {
        S.clear();
        for (auto index : graph->getVertexIndices()) {
            D[index] = 0;
            currentUsefulEdge[index] = 0;
        }
    }
    void restart(){
        setDefaultValues();
        candidateEdges.clear();
    }

    bool isUseful(vertexIndex t) { return S.find(t) == S.end(); }
    virtual Edge* getEdgeCandidate(vertexIndex s) = 0;
    virtual void addEdgeCandidate(vertexIndex s) = 0;
    virtual void replaceUselessCandidates() = 0;
    virtual void initializeValues(vertexIndex& c, vertexIndex& t, weightType& weight,
                          const typename candidateType::iterator& item) = 0;

    unordered_map<vertexIndex, weightType> SingleSource(vertexIndex s){
        assert(graph->findVertex(s) != nullptr);
        S.emplace(s);
        D[s] = 0;
        addEdgeCandidate(s);
        return DantzigExpand((int)graph->getNumberOfVertices());
    }

    unordered_map<vertexIndex, weightType> DantzigExpand(int limit) {
        vertexIndex c, t, v;
        weightType weight;  // C(c, t)
        while (S.size() < limit) {
            initializeValues(c, t, weight, candidateEdges.begin());
            S.emplace(t);
            D[t] = D[c] + weight;
            if (S.size() == limit) break;
            addEdgeCandidate(t);
            replaceUselessCandidates();
        }
        return D;
    }

public:
    Dantzig(): graph(nullptr){};
    explicit Dantzig(Graph* _graph) : graph(_graph){
        graph->sortAdjacencyList();
        setDefaultValues();
    }
    unordered_map<vertexIndex, weightType> DantzigAlgorithm(vertexIndex s){
        return SingleSource(s);
    }
};


class OriginalDantzig : public Dantzig<arrayType>{
private:
    Edge* getEdgeCandidate(vertexIndex s) override {

    }

    void addEdgeCandidate(vertexIndex s) override {

    }

    void replaceUselessCandidates() override {

    }

    void initializeValues(vertexIndex& c, vertexIndex& t, weightType& weight,
                                  const arrayType::iterator& item) override {

    }
public:
    OriginalDantzig()= default;
    explicit OriginalDantzig(Graph* graph): Dantzig<arrayType>(graph){}
};


class ImprovedDantzig : public Dantzig<mapType>{
private:
    void initializeValues(vertexIndex& c, vertexIndex& t, weightType& weight,
                          const mapType::iterator& item) override {
        c = item->second->start;
        t = item->second->end;
        weight = item->second->weight;
    }

    Edge* getEdgeCandidate(vertexIndex s) override{
        Edge* edgeUseful = nullptr;
        auto vertex = graph->findVertex(s);
        auto edges = vertex->edges;
        size_t i = currentUsefulEdge[s];
        for (; i < edges.size(); ++i) {
            if (isUseful(edges[i]->end)) {
                edgeUseful = edges[i];
                break;
            }
        }
        currentUsefulEdge[s] = i;
        return edgeUseful;
    }
    void addEdgeCandidate(vertexIndex s) override{
        auto edgeCandidate = getEdgeCandidate(s);
        if (edgeCandidate != nullptr) {
            auto weight = edgeCandidate->weight;
            candidateEdges.emplace(D[s] + weight, edgeCandidate);
        }
    }
    void replaceUselessCandidates() override{
        vector<mapType::iterator> toRemove;
        vertexIndex v, t;
        for (auto it = candidateEdges.begin(); it != candidateEdges.end(); ++it) {
            t = it->second->end;
            if (!isUseful(t)) {
                v = it->second->start;
                toRemove.push_back(it);
                addEdgeCandidate(v);
            }
        }
        for (auto it : toRemove) {
            candidateEdges.erase(it);
        }
    }
public:
    ImprovedDantzig()= default;
    explicit ImprovedDantzig(Graph* graph): Dantzig<mapType>(graph){}

};
#endif  // PFC_PROJECT_DANTZIG_H