folder=../

all:
	@echo "Mirar los comandos del makefile"

dantzig:
	g++ -std=c++2a $(folder)Tests.cpp -o dantzig.out
	bash DantzigTests.sh

spira:
	g++ -std=c++2a $(folder)Tests.cpp -o spira.out
	bash SpiraTests.sh

moffat:
	g++ -std=c++2a $(folder)Tests.cpp -o moffat.out
	bash MoffatTests.sh

moffatWithBoost:
	g++ -std=c++2a $(folder)Tests.cpp -o moffatWithBoost.out
	bash MoffatWithBoostTests.sh
