#!/bin/sh

# Run each combination of (data/MC) with 1000*1000

V=15

./run${V}.sh 1 1 >run${V}-1-1.out 2>&1
./run${V}.sh 1 2 >run${V}-1-2.out 2>&1
./run${V}.sh 1 3 >run${V}-1-3.out 2>&1
./run${V}.sh 1 4 >run${V}-1-4.out 2>&1

./run${V}.sh 2 1 >run${V}-2-1.out 2>&1
./run${V}.sh 2 2 >run${V}-2-2.out 2>&1
./run${V}.sh 2 3 >run${V}-2-3.out 2>&1
./run${V}.sh 2 4 >run${V}-2-4.out 2>&1

exit
