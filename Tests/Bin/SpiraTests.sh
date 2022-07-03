#!/bin/bash
spira=2

for((i = 10; i <= 990; i += 10)); do
  ./spira.out $i $spira
done

for((i = 1000; i <= 5000; i += 200)); do
  ./spira.out $i $spira
done