#!/bin/sh
#
# randomize.sh
#
date
# Might as well recompile it here too
g++ -g -o randomize randomize.cpp

VERSION=${1:-1}
NDATA=${2:-1000}
FIRSTSEED=${3:-3141597}

echo 'Parameters set to (VERSION,NDATA,FIRSTSEED) '${VERSION} ${NDATA} ${FIRSTSEED}

# Set up symbolic links used in code.
MASTERFILE=datafile${VERSION}.cdat
SUBSETFILE=datasubset${VERSION}.cdat
CODEINFILE=MasterDataSet.cdat
CODEOUTFILE=Subset.cdat

ln -s ${MASTERFILE} ${CODEINFILE}

# Check symbolic links
ls -l *.cdat

./randomize ${NDATA} ${FIRSTSEED}

date

# Clean up
rm ${CODEINFILE}
mv ${CODEOUTFILE} ${SUBSETFILE}

exit
