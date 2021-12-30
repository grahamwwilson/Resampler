#!/bin/sh
#
# Add CHOICE as an option
# Here we no longer make the data/MC distinction 
# comparing simply GP Run5 with GP Run7.
#
date
# Might as well recompile it here too
g++ -g -o example7 example7.cpp `root-config --cflags --glibs`

# Set up symbolic links used in code.
rm dataset1.cdat
rm dataset2.cdat

ln -s datasubset1.cdat dataset1.cdat
ln -s datasubset2.cdat dataset2.cdat

# Check symbolic links
ls -l *.cdat

# Should be slightly quicker if the smaller file (Run7) 
# is set as the first "DATA" one.

NPERMS=${1:-999}
NDATA=${2:-1000}
NMC=${3:-1000}
FIRSTSEED=${4:-123456}
CHOICE=${5:-4}

echo 'Parameters set to (NDATA,NMC,NPERMS,FIRSTSEED,CHOICE) '${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${CHOICE}
./example7 ${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${CHOICE}

date

exit
