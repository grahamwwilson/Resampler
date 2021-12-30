#!/bin/sh

# Run each combination of (data/MC) with 1000*1000

./run9.sh 1 1 >run9-1-1.out
./run9.sh 1 2 >run9-1-2.out
./run9.sh 1 3 >run9-1-3.out
./run9.sh 1 4 >run9-1-4.out

./run9.sh 2 1 >run9-2-1.out
./run9.sh 2 2 >run9-2-2.out
./run9.sh 2 3 >run9-2-3.out
./run9.sh 2 4 >run9-2-4.out

exit
