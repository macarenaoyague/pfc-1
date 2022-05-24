All: compile moving

compile:
	mpic++ -std=c++2a -fopenmp DantzigTests.cpp -o dantzig.out

moving:
	mv dantzig.out ../

moving2:
	mv dantzig.out ../../

khipu: compile moving2
	cp dantzig.sh ../../dantzig.sh