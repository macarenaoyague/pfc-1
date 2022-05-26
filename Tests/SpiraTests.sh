#!/bin/bash
spira=2

g++ -std=c++2a Tests.cpp -o spira.out
for((i = 10; i <= 990; i += 10)); do
  ./spira.out $i $spira
done

for((i = 1000; i <= 5000; i += 200)); do
  ./spira.out $i $spira
done