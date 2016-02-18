#!/bin/bash
# Build MWSim

# call this script only from the build/ subdirectory

if [ -z "${MAKEOPTS}" ]
then
    MAKEOPTS="-j5"
fi

cmake .. -DPREFIX=~/local
make ${MAKEOPTS}
make install

