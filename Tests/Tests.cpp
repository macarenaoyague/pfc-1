#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include "../Algorithms/Dantzig.hpp"
#include "../Algorithms/Spira.hpp"
#include "../Algorithms/MoffatAndTakaoka.hpp"
#include "Util.hpp"
#include <chrono>
using namespace std;

BaseAlgorithm* chooseAlgorithm(int op, Graph* graph){
    BaseAlgorithm* algorithm = nullptr;
    if(op == 1) algorithm = new OriginalDantzig(graph);
    else if(op == 2) algorithm = new OriginalSpira(graph);
    else if(op == 3) algorithm = new OriginalMoffatAndTakaoka(graph);
    else if(op == 4) algorithm = new BoostMoffatAndTakaoka<fibonacci_heap<pairType, boost::heap::compare<compareItem>>>(graph);
    return algorithm;
}

string getNameOfAlgorithm(int op){
    string name;
    if(op == 1) name = "dantzig";
    else if(op == 2) name = "spira";
    else if(op == 3) name = "moffat";
    else if(op == 4) name = "moffatWithBoost";
    return name;
}

int main(int argc, char** argv) {
    if(argc < 3) assert(false);
    int n = strtol(argv[1], NULL, 10);
    int op = strtol(argv[2], NULL, 10);

    string folder = "../../";
    string filename = folder + "Graphs/graph" + to_string(n) + ".bin";
    auto* graph = readGraphFromFile(filename);

    float time = 0.0f;
    vector<float> executionTimePerOperation(3, 0.0f);
    vector<int> callsPerOperation(3, 0);

    for (int i = 0; i < n; ++i) {
        auto* algorithm = chooseAlgorithm(op, graph);
        if(algorithm == nullptr) assert(false);
        auto start = chrono::system_clock::now();
        algorithm->executeAlgorithm(i);
        auto end = chrono::system_clock::now();
        std::chrono::duration<float,std::milli> duration = end - start;

        time += duration.count();
        auto operations = algorithm->getOperations();
        for(size_t i = 0; i < operations.size(); ++i){
            executionTimePerOperation[i] += operations[i].first;
            callsPerOperation[i] += operations[i].second;
        }
        
        delete algorithm;
    }

    string algorithm = getNameOfAlgorithm(op);
    filename = folder + "Results/" + algorithm + "Results.txt";
    ofstream file(filename, ios::out | ios::app);
    if(!file.is_open()) assert(false);

    file << n << "|"; 
    file << "execution_time=" << time << "|";

    file << "push_time=" << executionTimePerOperation[0] << "|";
    file << "push_count=" << callsPerOperation[0] << "|";
    file << "average=" << executionTimePerOperation[0] * 1000.0 / callsPerOperation[0] << "|";
    
    file << "pop_time=" << executionTimePerOperation[1] << "|";
    file << "pop_count=" << callsPerOperation[1] << "|";
    file << "average=" << executionTimePerOperation[1] * 1000.0 / callsPerOperation[1] << "|";

    file << "cleanUp_time=" << executionTimePerOperation[2] << "|";
    file << "cleanUp_count=" << callsPerOperation[2] << "|";
    file << "average=" << executionTimePerOperation[2] * 1000.0 / callsPerOperation[2];

    file << endl;

    file.close();
}