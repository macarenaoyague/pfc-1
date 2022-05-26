#!/bin/bash

#SBATCH --job-name=dantzig_tests
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=32gb

# Cargar el modulo MPI.
module load mpich/4.0

# Configurar el valor de  OMP_NUM_THREADS con el numero de CPUs por task solicitado.
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

mpirun -np 121 ./dantzig.out

module unload mpich/4.0