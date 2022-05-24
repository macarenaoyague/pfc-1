#!/bin/bash

#SBATCH --job-name=testing
#SBATCH --nodes=1
#SBATCH --mem-per-cpu=32gb
#SBATCH --output=testing_output.out

mpirun -np 2 ./testing.out