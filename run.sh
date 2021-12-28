#!/bin/sh
#
# Started from RobustResolution code in ILDbench_WWqqlnu/graham
#
date
# Might as well recompile it here too
g++ -g -o example example.cpp `root-config --cflags --glibs`

NDATA=${1:-1000}
NMC=${2:-10000}
NPERMS=${3:-10}
FIRSTSEED=${4:-3141596}

echo 'Parameters set to (NDATA,NMC,NPERMS,FIRSTSEED) '${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED}

./example ${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED}

date

exit
