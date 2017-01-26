/* -*- c++ -*- */

#define ENERGYBEAMFORMING_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "EnergyBeamforming_swig_doc.i"

%{
#include "EnergyBeamforming/lo_estimation_cf.h"
#include "EnergyBeamforming/ekf_ff.h"
#include "EnergyBeamforming/tx_packet_source.h"
#include "EnergyBeamforming/bfweight_cc.h"
#include "EnergyBeamforming/randphpert_f.h"
#include "EnergyBeamforming/correct_gmsk_cc.h"
#include "EnergyBeamforming/randphpert2_f.h"
#include "EnergyBeamforming/one_bit_feedb2_cf.h"
#include "EnergyBeamforming/multi_slicer_fi.h"
#include "EnergyBeamforming/randphpert3_f.h"
#include "EnergyBeamforming/randphpert4_f.h"
#include "EnergyBeamforming/bfweight2_cc.h"

%}


%include "EnergyBeamforming/lo_estimation_cf.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, lo_estimation_cf);
%include "EnergyBeamforming/ekf_ff.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, ekf_ff);
%include "EnergyBeamforming/tx_packet_source.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, tx_packet_source);
%include "EnergyBeamforming/bfweight_cc.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, bfweight_cc);
%include "EnergyBeamforming/randphpert_f.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, randphpert_f);
%include "EnergyBeamforming/correct_gmsk_cc.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, correct_gmsk_cc);

%include "EnergyBeamforming/randphpert2_f.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, randphpert2_f);
%include "EnergyBeamforming/one_bit_feedb2_cf.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, one_bit_feedb2_cf);
%include "EnergyBeamforming/multi_slicer_fi.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, multi_slicer_fi);
%include "EnergyBeamforming/randphpert3_f.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, randphpert3_f);
%include "EnergyBeamforming/randphpert4_f.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, randphpert4_f);
%include "EnergyBeamforming/bfweight2_cc.h"
GR_SWIG_BLOCK_MAGIC2(EnergyBeamforming, bfweight2_cc);
