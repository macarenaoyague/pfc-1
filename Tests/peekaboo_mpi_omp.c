#include <stdio.h>
#include "mpi.h"
#include <omp.h>
int main(int argc, char *argv[]) {
   int numprocs, rank, namelen;
   char processor_name[MPI_MAX_PROCESSOR_NAME];
   int iam = 0, np = 1;

 MPI_Init(&argc, &argv);
 MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Get_processor_name(processor_name, &namelen);

#pragma omp parallel num_threads(4) default(shared) private(iam, np)
  {
      np = omp_get_num_threads();
      iam = omp_get_thread_num();
     printf("Peekaboo desde el hilo %d de un total de  %d y desde el proceso %d de un total de %d en %s\n", iam, np, rank, numprocs, processor_name);
	        }

  MPI_Finalize();
  }

