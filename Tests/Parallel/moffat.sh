#!/bin/bash

# Cargar el modulo MPI.
module load mpich/4.0

mpicxx -std=c++2a TestsParallel.cpp -o moffat.out

mpirun -np 131 ./moffat.out 1

module unload mpich/4.0