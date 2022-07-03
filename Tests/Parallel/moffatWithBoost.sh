#!/bin/bash

# Cargar el modulo MPI.
module load mpich/4.0

mpicxx -std=c++2a TestsParallel.cpp -o moffatWithBoost.out

mpirun -np 131 ./moffatWithBoost.out 2

module unload mpich/4.0