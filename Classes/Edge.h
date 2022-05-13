#ifndef PFC_PROJECT_EDGE_H

#include "Global.h"

class Edge {
public:
    vertexIndex start;
    vertexIndex end;
    weightType weight;
    
    Edge(): start(0), end(0), weight(-1){}
    Edge(vertexIndex _start, vertexIndex _end, weightType _weight):
        start(_start), end(_end), weight(_weight){}
};

#endif // PFC_PROJECT_EDGE_H