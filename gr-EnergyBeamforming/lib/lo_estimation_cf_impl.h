/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_ENERGYBEAMFORMING_LO_ESTIMATION_CF_IMPL_H
#define INCLUDED_ENERGYBEAMFORMING_LO_ESTIMATION_CF_IMPL_H

#include <EnergyBeamforming/lo_estimation_cf.h>
#include <iostream>

namespace gr {
  namespace EnergyBeamforming {

    class lo_estimation_cf_impl : public lo_estimation_cf
    {
     private:
		double d_sample_rate;
		double d_treshold;
		int d_nbr_samples_packet;
		int d_N;
		int d_K;

		double Ts;
		int cpt_samples;
		int cpt_samples_packet;
		int cpt_k_1;
		int cpt_k_2;
		float sign_cpt_k_1;
		float sign_cpt_k_2;
		float pi;

		gr_complex tx_header [192];
		gr_complex rx_packet [1408];
		uint64_t start_time_recording_packet;
		gr_complex pilotSignal_1;
		gr_complex pilotSignal_2;
		double data_fft_1 [1024];
		double data_fft_2 [1024];
		double eff_Ts;
		double data_abs_1 [512];
		double data_abs_2 [512];
		double pilotSpectrum_1 [512];
		double pilotSpectrum_2 [512];
		double max_val_1;
		double max_val_2;
		double max_val;
		int max_ind_1;
		int max_ind_2;
		int max_ind;
		double max_ind_final;
		double Y_before;
		double Y_after;
		double IPlus;
		double freq_estimate_double_lo;
		float freq_estimate;
		gr_complex corr_output [1408];
		double phase_coeff;
		gr_complex corr_output_max;
		int index_max;
		float phase_estimate;
		uint64_t start_time_packet;
		float delay_estimate;
		uint64_t last_start_time_packet;
		double fracs_pkt;
		double fracs;
		double pps_fracs;
		uint64_t secs_pkt;
		uint64_t secs;
		uint64_t pps_secs;
		uint64_t offs;
      // Nothing to declare in this block.

     public:
      lo_estimation_cf_impl(double sample_rate, double treshold, int nbr_samples_packet, int N, int K);
      ~lo_estimation_cf_impl();
      
      uint64_t get_pps_secs () const {return pps_secs;}
	  double get_pps_fracs ()  const {return pps_fracs;}

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace EnergyBeamforming
} // namespace gr

#endif /* INCLUDED_ENERGYBEAMFORMING_LO_ESTIMATION_CF_IMPL_H */

