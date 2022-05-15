#ifndef PFC_PROJECT_VERTEX_H

#include "Edge.hpp"

class Vertex {
 public:
  vertexIndex idx;
  vector<Edge*> edges;

  Vertex() {}
  explicit Vertex(vertexIndex _idx) : idx(_idx) {}
  void addEdge(Edge* edge) { edges.push_back(edge); }

  Edge* findEdge(vertexIndex end) {
    Edge* result = nullptr;
    for (auto& edge : edges)
      if (edge->end == end) return edge;
    return result;
  }

  void sortEdges() {
    if (edges.empty()) return;
    sort(edges.begin(), edges.end(),
         [](Edge*& a, Edge*& b) -> bool { return a->weight < b->weight; });
  }

  ~Vertex() {
    for (auto& edge : edges) delete edge;
    edges.clear();
  }
};

#endif  // PFC_PROJECT_VERTEX_H