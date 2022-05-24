All: compile moving

compile:
	mpic++ -std=c++2a -fopenmp DantzigTests.cpp -o dantzig.out
	mpic++ -fopenmp peekaboo_mpi_omp.c -o testing.out

moving:
	mv dantzig.out ../
	mv testing.out ../

moving2:
	mv dantzig.out ../../
	mv testing.out ../../

khipu: compile moving2
	cp dantzig.sh ../../dantzig.sh