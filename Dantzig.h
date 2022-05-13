#include "Graph.h"
#include <set>
#include <queue>

// w, c, t
typedef pair<weightType, pair<vertexIndex, vertexIndex>> pwvv;

class Dantzig {

private:

    Graph* graph;

    // nodos ya visitados (incluídos en D)
    set<vertexIndex> S;
    unordered_map <vertexIndex, unsigned> D;

    // map con t: c
    unordered_map<vertexIndex, vector<vertexIndex>> mapT;
    // D[c] + C(c, t) y t
    priority_queue<pwvv, vector<pwvv>, greater<pwvv>> edgeT;
    //priority_queue <pair<weightType, vertexIndex>> edgeT;

    
    Vertex* findVertex(vertexIndex idx){
        return graph->findVertex(idx);
    }

    bool isUseful(vertexIndex t) {
        return S.find(t) == S.end();
    }

    weightType C(vertexIndex c, vertexIndex t){
        return findVertex(c)->edgesMAP[t];
    }

    bool insertNextUsefulU(vertexIndex uIdx) {
        Vertex* c = findVertex(uIdx);
        vertexIndex tIdx;
        if (c == nullptr) return false;
        for (auto edge: c->edges) {
            tIdx = edge->end;
            if (isUseful(tIdx)) {
                if (mapT.find(tIdx) == mapT.end()) {
                    vector<vertexIndex> v;
                    mapT[tIdx] = v;
                };
                mapT[tIdx].push_back(uIdx);
                edgeT.push({D[uIdx]+edge->weight, {uIdx, tIdx}});
                return true;
            } 
        }
        return false;
    }

    bool insertNextUsefulEdge(vertexIndex cIdx) {
        Vertex* c = findVertex(cIdx);
        vertexIndex tIdx;
        if (c == nullptr) return false;
        for (auto edge: c->edges) {
            tIdx = edge->end;
            if (isUseful(tIdx)) {
                if (mapT.find(tIdx) == mapT.end()) {
                    vector<vertexIndex> v;
                    mapT[tIdx] = v;
                };
                mapT[tIdx].push_back(cIdx);
                //w, c, t
                edgeT.push({D[cIdx]+edge->weight, {cIdx, tIdx}});
                return true;
            } else {
                insertNextUsefulU(tIdx);
            }
        }
        return false;
    }

    //vertex is already labelled
    unordered_map <vertexIndex, unsigned> DantzigExpand(int limit){
        pwvv t;
        weightType weightT;
        vertexIndex vertexC, vertexT;

        while(S.size() < limit) {
            if (edgeT.empty()) break;
            t = edgeT.top();
            edgeT.pop();
            while(!isUseful(t.second.second)){
                if(edgeT.empty()) break;
                t = edgeT.top();
                edgeT.pop();
            }
            weightT = t.first;
            vertexC = t.second.first;
            vertexT = t.second.second;
            S.insert(vertexT);
            D[vertexT] = weightT;
            if (S.size() == limit) break;
            insertNextUsefulEdge(vertexT);
            vector<vertexIndex> u = mapT[vertexT];
            for(vertexIndex uVertex: u) {
                if (uVertex != vertexC)
                    insertNextUsefulEdge(uVertex);
            }
        }
        return D;
    }


public:

    Dantzig(Graph* graph) {
        this->graph = graph;
    }

    unordered_map <vertexIndex, unsigned> SingleSource(vertexIndex s) {
        // graph->sortAdjacencyList();
        S.insert(s);
        if(insertNextUsefulEdge(s) == false) return D;
        D[s] = 0;
        return DantzigExpand(graph->getNumberOfVertices());
    }

    void print() {
        for (auto e: D)
            cout << e.first << ": " << e.second << endl;
    }

};
