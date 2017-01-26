#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/lib
export PATH=/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/build/lib:$PATH
export LD_LIBRARY_PATH=/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-EnergyBeamforming 
