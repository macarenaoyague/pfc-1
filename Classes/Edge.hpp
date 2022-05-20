#pragma once
#include "Global.hpp"

class Edge {
public:
    vertexIndex start;
    vertexIndex end;
    weightType weight;

    Edge() : start(0), end(0), weight(-1) {}

    Edge(vertexIndex _start, vertexIndex _end, weightType _weight)
            : start(_start), end(_end), weight(_weight) {}

    bool operator==(Edge &other) const {
        return other.start == start && other.end == end;
    }
};