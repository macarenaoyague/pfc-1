#include <mpi.h>
#include <vector>
#include <string>
#include "../Util.hpp"
#include "../../Algorithms/MoffatAndTakaoka.hpp"
#include "../../Algorithms/Dantzig.hpp"

using namespace std;

BaseAlgorithm* chooseAlgorithm(int op, Graph* graph){
    BaseAlgorithm* algorithm = nullptr;
    if(op == 1) algorithm = new OriginalMoffatAndTakaoka(graph);
    else if(op == 2) algorithm = new BoostMoffatAndTakaoka<fibonacci_heap<pairType, boost::heap::compare<compareItem>>>(graph);
    return algorithm;
}

string getNameOfAlgorithm(int op){
    string name;
    if(op == 1) name = "moffat";
    else if(op == 2) name = "moffatWithBoost";
    return name;
}

int main(int argc, char** argv) {
    int world_size, world_rank;
    int n = -1;
    if(argc < 2) assert(false);
    int op = strtol(argv[1], NULL, 10);
    double sum = 0.0;
    string folder = "../../";
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_rank == 0){
        vector<int> nvalues = readNVertices(folder + "Graphs/nvertices.bin");
        int i, j;
        for(i = 0, j = 1; i < nvalues.size() && j < world_size; ++i, ++j){
            cout << nvalues[i] << " to = " << j << endl;
            MPI_Send(&nvalues[i], 1, MPI_INT, j, 601, MPI_COMM_WORLD);
        }
    }else{
        MPI_Recv(&n, 1, MPI_INT, 0, 601, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        string filename = folder + "Graphs/graph" + to_string(n) + ".bin";
        auto* graph = readGraphFromFile(filename);
        float time = 0.0f;
        vector<float> executionTimePerOperation(3, 0.0f);
        vector<int> callsPerOperation(3, 0);

        for (int i = 0; i < n; ++i) {
            double start, end;
            BaseAlgorithm* algorithm = chooseAlgorithm(op, graph);
            start = MPI_Wtime();
            algorithm->executeAlgorithm(i);
            end = MPI_Wtime();

            time += end - start;
            auto operations = algorithm->getOperations();
            for(size_t i = 0; i < operations.size(); ++i){
                executionTimePerOperation[i] += operations[i].first;
                callsPerOperation[i] += operations[i].second;
            }

            delete algorithm;
        }
        string algorithm = getNameOfAlgorithm(op);
        filename = "Results/" + algorithm + to_string(n) + ".txt";

        ofstream file(filename, ios::out);
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
    MPI_Finalize();
}