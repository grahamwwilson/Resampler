#!/bin/sh

V=${1-18}
VO=${V}${2-A}

echo 'V  set to '${V}
echo 'VO set to '${VO}


./run${V}.sh KSX2 >run${VO}-KSX2.out
./run${V}.sh KSECM >run${VO}-KSECM.out
./run${V}.sh KSEdiff >run${VO}-KSEdiff.out
./run${V}.sh KSX1 >run${VO}-KSX1.out
./run${V}.sh MeanECM >run${VO}-MeanECM.out
./run${V}.sh MeanEdiff >run${VO}-MeanEdiff.out
./run${V}.sh MeanX1 >run${VO}-MeanX1.out
./run${V}.sh MeanX2 >run${VO}-MeanX2.out
./run${V}.sh ChiSquaredX1 >run${VO}-ChiSquaredX1.out
./run${V}.sh ChiSquaredX2 >run${VO}-ChiSquaredX2.out
./run${V}.sh ChiSquaredX1X2 >run${VO}-ChiSquaredX1X2.out
./run${V}.sh ChiSquaredX1+X2 >run${VO}-ChiSquaredX1+X2.out

exit

