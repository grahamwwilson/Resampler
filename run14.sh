#!/bin/sh
#
# Add CHOICE as an option
# Here we no longer make the data/MC distinction 
# comparing simply GP Run5 with GP Run7.
#
date
# Might as well recompile it here too
#g++ -g -o example8 example8.cpp `root-config --cflags --glibs`

export OMP_NUM_THREADS=6

echo 'OMP_NUM_THREADS: '$OMP_NUM_THREADS

g++ -g -O1 -fopenmp -o example example9.cpp `root-config --cflags --glibs`

# Set up symbolic links used in code.
VDATA=${1:-1}
VMC=${2:-1}

echo 'VDATA '${VDATA}
echo 'VMC   '${VMC}

rm dataset1.cdat
rm dataset2.cdat

ln -s datasubset${VDATA}.cdat dataset1.cdat
ln -s mcsubset${VMC}.cdat dataset2.cdat

# Check symbolic links
ls -l *.cdat

# Should be slightly quicker if the smaller file (Run7) 
# is set as the first "DATA" one.

NPERMS=${3:-1000}
NDATA=${4:-1000}
NMC=${5:-1000}
FIRSTSEED=${6:-153456}
CHOICE=${7:-3}

echo 'Parameters set to (NDATA,NMC,NPERMS,FIRSTSEED,CHOICE) '${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${CHOICE}
time ./example ${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${CHOICE}

date

exit
