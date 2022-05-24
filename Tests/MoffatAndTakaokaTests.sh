#!/bin/bash
moffatAndTakaoka=3

g++ -std=c++2a Tests.cpp -o moffatAndTakaoka.out
for((i = 10; i <= 990; i += 10)); do
  ./moffatAndTakaoka.out $i $moffatAndTakaoka
done

for((i = 1000; i <= 5000; i += 200)); do
  ./moffatAndTakaoka.out $i $moffatAndTakaoka
done