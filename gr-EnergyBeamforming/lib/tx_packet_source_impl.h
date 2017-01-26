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

#ifndef INCLUDED_ENERGYBEAMFORMING_TX_PACKET_SOURCE_IMPL_H
#define INCLUDED_ENERGYBEAMFORMING_TX_PACKET_SOURCE_IMPL_H

#include <EnergyBeamforming/tx_packet_source.h>
#include <iostream>
 

namespace gr {
  namespace EnergyBeamforming {

    class tx_packet_source_impl : public tx_packet_source
    {
     private:
		double d_sample_rate;
		double d_time_delay;
		int d_nbr_samples_packet;
		float d_f_in;
		float d_f_out;
		
		int cpt_samples;
		bool fpga_time_available;
		uint64_t secs_rx;
		double fracs_rx;
		uint64_t secs_tx;
		double fracs_tx;
		int cpt_samples_packet;
		gr_complex tx_packet[10000];

		float lo_phase_offset;
		float lo_freq_offset;
		float Ts;
		float output_phase_ref_freq;
		float output_phase_bf_freq;
		float pi;
      // Nothing to declare in this block.

     public:
      tx_packet_source_impl(double sample_rate, double time_delay, int nbr_samples_packet, float f_in, float f_out);
      ~tx_packet_source_impl();
      
      void make_time_tag(const uint64_t tag_count);
	  void make_sob_tag(const uint64_t tag_count);
	  void make_eob_tag(const uint64_t tag_count);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace EnergyBeamforming
} // namespace gr

#endif /* INCLUDED_ENERGYBEAMFORMING_TX_PACKET_SOURCE_IMPL_H */

