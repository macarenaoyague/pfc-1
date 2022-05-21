#pragma once
#include "../Classes/Graph.hpp"

template<typename T>
class Tester{
private:
    T* algorithm;
public:
    explicit Tester(Graph* graph){
        algorithm = new T(graph);
    }
    
    unordered_map<vertexIndex, weightType> executeAlgorithm(vertexIndex s){
        return algorithm->executeAlgorithm(s);
    }

    ~Tester(){
        delete algorithm;
    }
};