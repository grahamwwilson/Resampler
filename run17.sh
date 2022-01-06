#!/bin/sh
#
# Here we no longer necessarily make the data/MC distinction
#
date
# Might as well recompile it here too
#g++ -g -o example8 example8.cpp `root-config --cflags --glibs`

export OMP_NUM_THREADS=6

echo 'OMP_NUM_THREADS: '$OMP_NUM_THREADS

g++ -g -O1 -fopenmp -o example example11.cpp `root-config --cflags --glibs`

# Set up symbolic links used in code.

rm dataset1.cdat
rm dataset2.cdat

ln -s datafile1.cdat dataset1.cdat
ln -s datafile2.cdat dataset2.cdat

# Check symbolic links
ls -l *.cdat

# Can be quicker for some estimators if the smaller file 
# is set as the first "DATA" set.

ESTIMATOR=${1:-ChiSquaredX1}
NPERMS=${2:-10000}
NDATA=${3:-171349}
NMC=${4:-156231}
FIRSTSEED=${5:-213456}
# Now specify statistic choice using Estimator enum with corresponding EstimatorNames string

echo 'Parameters set to (NDATA,NMC,NPERMS,FIRSTSEED,ESTIMATOR) '${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${ESTIMATOR}
time ./example ${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${ESTIMATOR}

date

exit
