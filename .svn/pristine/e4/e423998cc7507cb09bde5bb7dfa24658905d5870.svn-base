#!/bin/bash

# where am I?
SCRIPT="$(readlink -f ${BASH_SOURCE[0]})"
DIRBIN="$(dirname $SCRIPT)"

# our software is here 
SOFTDIR="$(dirname $DIRBIN)"

export WA105SOFT=${SOFTDIR}
# config directory
export WA105CONFIGDIR=${WA105SOFT}/Qscan

export PATH=$PATH:${SOFTDIR}/bin:${SOFTDIR}/Qscan:${SOFTDIR}/benchmark:${SOFTDIR}/purity
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SOFTDIR}/lib
