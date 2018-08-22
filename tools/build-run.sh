#!/bin/bash

# setup environment
export CPU=arm
export OS=linux
export VARIANT=debug
export CTA_ROOT=~/src/CTA-2045-UCM-CPP-Library
export CTALIB=$CTA_ROOT/build/debug
export CTAINC=$CTA_ROOT/cea2045
export LD_LIBRARY_PATH=$CTALIB:$LD_LIBRARY_PATH

# build
export SRC=ewh
make -C ../build

# run
./../build/bin/debug/$SRC
