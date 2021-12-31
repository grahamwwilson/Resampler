#!/bin/sh

# Run each combination of (data/MC) with 1000*1000

./run11.sh 1 1 >run11-1-1.out
./run11.sh 1 2 >run11-1-2.out
./run11.sh 1 3 >run11-1-3.out
./run11.sh 1 4 >run11-1-4.out

./run11.sh 2 1 >run11-2-1.out
./run11.sh 2 2 >run11-2-2.out
./run11.sh 2 3 >run11-2-3.out
./run11.sh 2 4 >run11-2-4.out

exit
