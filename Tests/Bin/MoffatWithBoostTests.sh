#!/bin/bash
moffatWithBoost=4

for((i = 10; i <= 990; i += 10)); do
  ./moffatWithBoost.out $i $moffatWithBoost
done

for((i = 1000; i <= 5000; i += 200)); do
  ./moffatWithBoost.out $i $moffatWithBoost
done