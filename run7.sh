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

ln -s datafile2.cdat dataset1.cdat
ln -s datafile1.cdat dataset2.cdat

# Check symbolic links
ls -l *.cdat

# Should be slightly quicker if the smaller file (Run7) 
# is set as the first "DATA" one.

NPERMS=${1:-35}
NDATA=${2:-156231}
NMC=${3:-171349}
FIRSTSEED=${4:-3141597}
CHOICE=${5:-4}

echo 'Parameters set to (NDATA,NMC,NPERMS,FIRSTSEED,CHOICE) '${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${CHOICE}
./example7 ${NDATA} ${NMC} ${NPERMS} ${FIRSTSEED} ${CHOICE}

date

exit
