#!/bin/sh

V=18

./run${V}.sh MeanECM >run${V}-MeanECM.out
./run${V}.sh MeanEdiff >run${V}-MeanEdiff.out
./run${V}.sh MeanX1 >run${V}-MeanX1.out
./run${V}.sh MeanX2 >run${V}-MeanX2.out
./run${V}.sh KSECM >run${V}-KSECM.out
./run${V}.sh KSEdiff >run${V}-KSEdiff.out
./run${V}.sh KSX1 >run${V}-KSX1.out
./run${V}.sh KSX2 >run${V}-KSX2.out
./run${V}.sh ChiSquaredX1 >run${V}-ChiSquaredX1.out
./run${V}.sh ChiSquaredX2 >run${V}-ChiSquaredX2.out
./run${V}.sh ChiSquaredX1X2 >run${V}-ChiSquaredX1X2.out
./run${V}.sh ChiSquaredX1+X2 >run${V}-ChiSquaredX1+X2.out

exit

