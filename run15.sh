#!/bin/sh
#
# Here we no longer necessarily make the data/MC distinction
#
date
# Might as well recompile it here too
#g++ -g -o example8 example8.cpp `root-config --cflags --glibs`

export OMP_NUM_THREADS=6

echo 'OMP_NUM_THREADS: '$OMP_NUM_THREADS

g++ -g -O1 -fopenmp -o example example10.cpp `root-config --cflags --glibs`

# Set up symbolic links used in code.
VDATA=${1:-1}
VMC=${2:-0}

echo 'VDATA '${VDATA}
echo 'VMC   '${VMC}

rm dataset1.cdat
rm dataset2.cdat

ln -s datasubset${VDATA}.cdat dataset1.cdat
ln -s mcsubset${VMC}.cdat dataset2.cdat

# Check symbolic links
ls -l *.cdat

# Can be quicker for some estimators if the smaller file 
# is set as the first "DATA" set.

NPERMS=${3:-10000}
NDATA=${4:-1000}
NMC=${5:-1000}
FIRSTSEED=${6:-123456}
# Now specify statistic choice using Estimator enum with corresponding EstimatorNames string
ESTIMATOR=${7:-Energy2}

echo 'Parameters set to (NDATA,NMC,NPERMS,FIRSTSEED,ESTIMATOR) '${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${ESTIMATOR}
time ./example ${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${ESTIMATOR}

date

exit
