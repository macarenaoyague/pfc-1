#!/bin/bash
moffat=3

for((i = 10; i <= 990; i += 10)); do
  ./moffat.out $i $moffat
done

for((i = 1000; i <= 5000; i += 200)); do
  ./moffat.out $i $moffat
done