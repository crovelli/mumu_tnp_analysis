#!/bin/bash 

## add python lib
#######. /opt/rh/python27/enable
export LD_LIBRARY_PATH=/opt/rh/python27/root/usr/lib64/:$LD_LIBRARY_PATH

export PYTHONPATH=.:$PYTHONPATH

## add ROOT path
export ROOTSYS=/cvmfs/cms.cern.ch/slc7_amd64_gcc630/lcg/root/6.10.09-omkpbe4/
#export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH

. $ROOTSYS/bin/thisroot.sh
