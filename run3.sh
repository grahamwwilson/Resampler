#!/bin/sh
#
# Add CHOICE as an option
#
date
# Might as well recompile it here too
g++ -g -o example example5.cpp `root-config --cflags --glibs`

# Check symbolic links
ls -l *.cdat

NPERMS=${1:-999}
NDATA=${2:-156231}
NMC=${3:-1000000}
FIRSTSEED=${4:-3141596}
CHOICE=${5:-1}

echo 'Parameters set to (NDATA,NMC,NPERMS,FIRSTSEED,CHOICE) '${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${CHOICE}
./example ${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${CHOICE}

date

exit
