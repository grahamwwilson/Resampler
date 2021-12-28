#!/bin/sh
#
# Started from RobustResolution code in ILDbench_WWqqlnu/graham
#
date
# Might as well recompile it here too
g++ -g -o example example2.cpp `root-config --cflags --glibs`

NDATA=${1:-171349}
NMC=${2:-1000000}
NPERMS=${3:-999}
FIRSTSEED=${4:-3141596}

echo 'Parameters set to (NDATA,NMC,NPERMS,FIRSTSEED) '${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED}

./example ${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED}

date

exit
