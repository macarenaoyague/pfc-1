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
    else if(op == 3) algorithm = new MoffatAndTakaoka(graph);
    return algorithm;
}

string getNameOfAlgorithm(int op){
    string name;
    if(op == 1) name = "dantzig";
    else if(op == 2) name = "spira";
    else if(op == 3) name = "moffatAndTakaoka";
    return name;
}
int main(int argc, char** argv) {
    if(argc < 3) assert(false);
    int n = strtol(argv[1], NULL, 10);
    int op = strtol(argv[2], NULL, 10);
    string folder = "/home/johan/UTEC/2022.1/PFC_I/Proyecto_1/Code/";
    string filename = folder + "Graphs/graph" + to_string(n) + ".bin";
    auto* graph = readGraphFromFile(filename);

    float time = 0.0f;
    for (int i = 0; i < n; ++i) {
        auto* algorithm = chooseAlgorithm(op, graph);
        if(algorithm == nullptr) assert(false);
        auto start = chrono::system_clock::now();
        algorithm->executeAlgorithm(i);
        auto end = chrono::system_clock::now();
        std::chrono::duration<float,std::milli> duration = end - start;
        time += duration.count();
        delete algorithm;
    }
    string algorithm = getNameOfAlgorithm(op);
    filename = folder + "Results/" + algorithm + "Results.txt";
    ofstream file(filename, ios::out | ios::app);
    if(!file.is_open()) assert(false);
    file << n << "=" << time << endl;
    file.close();
}