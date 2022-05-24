#!/bin/bash

#SBATCH --job-name=dantzig_tests
#SBATCH --nodes=1
#SBATCH --mem-per-cpu=32gb

mpirun -np 121 ./dantzig.out