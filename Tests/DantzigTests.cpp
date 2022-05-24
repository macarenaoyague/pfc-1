#include <omp.h>
#include <mpi.h>
#include <vector>
#include <string>
#include "Util.hpp"
#include "../Algorithms/Dantzig.hpp"

using namespace std;

int main(int argc, char** argv) {
    //int thread_count = strtol(argv[1], NULL, 10);
    int thread_count = 4;
    int world_size, world_rank;
    int n = -1;
    double sum = 0.0;
    string folder = "/home/fabrizio.vasquez/pfc-1/";
    //string folder = "/home/johan/UTEC/2022.1/PFC_I/Proyecto_1/Code/";
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_rank == 0){
        vector<int> nvalues = readNVertices(folder + "Graphs/nvertices.bin");
        int i, j;
        for(int i = 0, j = 1; i < nvalues.size() && j < world_size; ++i, ++j){
            MPI_Send(&nvalues[1], 1, MPI_INT, j, 601, MPI_COMM_WORLD);
        }
        vector<double> results;
        for(j = 1; j < world_size; ++j){
            MPI_Recv(&sum, 1, MPI_DOUBLE, j, 601, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            results.emplace_back(sum);
        }
        string algorithm = "dantzig";
        string filename = folder + "Results/" + algorithm + "Results.txt";
        saveResults(filename, results, {20});
    }else{
        MPI_Recv(&n, 1, MPI_INT, 0, 601, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        string filename = folder + "Graphs/graph" + to_string(n) + ".bin";
        auto* graph = readGraphFromFile(filename);
        int chunk = n / thread_count;
        if(chunk >= 5){
            int i;
            double sum_temp = 0.0;
            #pragma omp parallel num_threads(thread_count) private(i) shared(sum_temp)
            {
                #pragma omp for schedule(dynamic, chunk) reduction(+:sum_temp)
                for(i = 0; i < n; ++i){
                    double start, end;
                    BaseAlgorithm* algorithm = new OriginalDantzig(graph);
                    start = omp_get_wtime();
                    algorithm->executeAlgorithm(i);
                    end = omp_get_wtime();
                    sum_temp += end - start;
                    delete algorithm;
                }
            }
            sum = sum_temp;
        }else {
            for (int i = 0; i < n; ++i) {
                double start, end;
                BaseAlgorithm* algorithm = new OriginalDantzig(graph);
                start = MPI_Wtime();
                algorithm->executeAlgorithm(i);
                end = MPI_Wtime();
                sum += end - start;
                delete algorithm;
            }
        }
        MPI_Send(&sum, 1, MPI_DOUBLE, 0, 601, MPI_COMM_WORLD);
    }
    MPI_Finalize();
}