#!/bin/bash
dantzig=1

g++ -std=c++2a Tests.cpp -o dantzig.out
for((i = 10; i <= 990; i += 10)); do
  ./dantzig.out $i $dantzig
done

for((i = 1000; i <= 5000; i += 200)); do
  ./dantzig.out $i $dantzig
done