#!/bin/sh
#
# Started from RobustResolution code in ILDbench_WWqqlnu/graham
#
date
# Might as well recompile it here too
g++ -g -o example example4.cpp `root-config --cflags --glibs`

# Check symbolic links
ls -l *.cdat

NPERMS=${1:-999}
NDATA=${2:-171349}
NMC=${3:-1000000}
FIRSTSEED=${4:-3141596}

echo 'Parameters set to (NDATA,NMC,NPERMS,FIRSTSEED) '${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED}
./example ${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED}

date

exit
