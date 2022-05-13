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

    bool insertNextUsefulEdge(vertexIndex cIdx) {
        Vertex* c = findVertex(cIdx);
        vertexIndex tIdx;
        cout << cIdx << ": ";
        if (c == nullptr) return false;
        for (auto edge: c->edges) {
            tIdx = edge->end;
            if (isUseful(tIdx)) {
                if (mapT.find(tIdx) == mapT.end()) {
                    vector<vertexIndex> v;
                    mapT[tIdx] = v;
                };
                mapT[tIdx].push_back(cIdx);
                cout << "si con " << tIdx << " y w " << D[cIdx]+edge->weight << endl;
                //w, c, t
                edgeT.push({D[cIdx]+edge->weight, {cIdx, tIdx}});
                return true;
            } 
        }
        cout << "no" << endl;
        return false;
    }

    //vertex is already labelled
    void DantzigExpand(int limit){
        pwvv t;
        weightType weightT;
        vertexIndex vertexC, vertexT;

        while(S.size() < limit) {
            if (edgeT.empty()) break;
            t = edgeT.top();
            edgeT.pop();
            cout << "top" << endl;
            cout << "w: " << t.first << ", c: " << t.second.first << ", t: " << t.second.second << endl;
            int i = 0;
            while(!isUseful(t.second.second)){
                i++;
                if (i == 10) return;
                cout << "hola";
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
            cout << "list de t: ";
            for(vertexIndex uVertex: u) {
                cout << uVertex << " ";
                if (uVertex != vertexC)
                    insertNextUsefulEdge(uVertex);
            }
            cout << endl;
        }
    }


public:

    Dantzig(Graph* graph) {
        this->graph = graph;
    }

    void SingleSource(vertexIndex s) {
        // graph->sortAdjacencyList();
        S.insert(s);
        if(insertNextUsefulEdge(s) == false) return;
        D[s] = 0;
        DantzigExpand(graph->getNumberOfVertices());
    }

    void print() {
        cout << D.size();
        for (auto e: D)
            cout << e.first << ": " << e.second << endl;
    }

    /*

    Vertex* cVertex;
        vertexIndex t; 
        for (vertexIndex c: S) {
            cVertex = graph->findVertex(c);
            if (cVertex == nullptr) continue;
            for (auto edge: cVertex->edges) {
                t = edge->end;
                if (isUseful(t)) {
                    T.insert(t);
                }
            }
        }
    
    */
};
