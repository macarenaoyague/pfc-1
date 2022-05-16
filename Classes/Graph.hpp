#ifndef PFC_PROJECT_GRAPH_H
#define PFC_PROJECT_GRAPH_H
#include <cassert>

#include "Vertex.hpp"
class Graph {
 private:
  vector<Edge*> E;  // optional
  unordered_map<vertexIndex, Vertex*> V;

 public:
  Graph() = default;
  Graph(vector<vertexIndex> vertices,
        vector<vector<weightType>> matrixAdjacency, weightType nullValue) {
    if (!vertices.empty() && vertices.size() == matrixAdjacency.size()) {
      size_t rows = matrixAdjacency.size();
      size_t columns = matrixAdjacency[0].size();
      assert(rows == columns);

      for (auto vertex : vertices) {
        insertVertex(vertex);
      }

      vertexIndex u, v;
      weightType w;
      for (int i = 0; i < rows; i++) {
        u = vertices[i];
        for (int j = 0; j < columns; j++) {
          v = vertices[j];
          w = matrixAdjacency[i][j];
          if (w != nullValue) insertEdge(u, v, w);
        }
      }
    }
  }

  bool insertVertex(vertexIndex idx) {
    if (V.find(idx) == V.end()) {
      auto newVertex = new Vertex(idx);
      V[idx] = newVertex;
      return true;
    }
    return false;
  }

  size_t getNumberOfVertices() { return V.size(); }

  void insertEdge(vertexIndex idx1, vertexIndex idx2, weightType weight) {
    if (V.find(idx1) != V.end() && V.find(idx2) != V.end()) {
      auto newEdge = new Edge(idx1, idx2, weight);
      V[idx1]->addEdge(newEdge);
      E.push_back(newEdge);  // optional
    }
  }

  Vertex* findVertex(vertexIndex idx) {
    if (V.find(idx) == V.end()) return nullptr;
    return V[idx];
  }

  Edge* findEdge(vertexIndex idx1, vertexIndex idx2) {
    auto it = V.find(idx1);
    if (it != V.end()) return it->second->findEdge(idx2);
    return nullptr;
  }

  void sortAdjacencyList() {
    for (auto item : V) {
      item.second->sortEdges();
    }
  }

  void print() {
    for (auto v : V) {
      cout << v.second->idx << ": ";
      for (auto e : v.second->edges) cout << e->end << ":" << e->weight << " ";
      cout << endl;
    }
  }

  vector<vertexIndex> getVertexIndices() {
    vector<vertexIndex> result;
    for (auto item : V) result.emplace_back(item.first);
    return result;
  }

  ~Graph() {
    for (auto vertex : V) {
      delete vertex.second;
    }
    V.clear();
    E.clear();
  }
};

#endif  // PFC_PROJECT_GRAPH_H