#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/python
export PATH=/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/build/python:$PATH
export LD_LIBRARY_PATH=/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/build/swig:$PYTHONPATH
/usr/bin/python2 /home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/python/qa_correct_gmsk_cc.py 
