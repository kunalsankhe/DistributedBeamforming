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

#ifndef INCLUDED_ENERGYBEAMFORMING_ONE_BIT_FEEDB2_CF_IMPL_H
#define INCLUDED_ENERGYBEAMFORMING_ONE_BIT_FEEDB2_CF_IMPL_H

#include <EnergyBeamforming/one_bit_feedb2_cf.h>
#include <gnuradio/random.h>

namespace gr {
  namespace EnergyBeamforming {

    class one_bit_feedb2_cf_impl : public one_bit_feedb2_cf
    {
     private:
     
		double d_sample_rate;
		int d_decim_factor;
		int d_nbr_output_floats;
		float d_treshold;
		int d_nbr_samples_packet;
		int d_nbr_samples_cycle;
		int cpt_samples;
		int cpt_samples_packet;
		int feedback_symbol;
		int packet_number;
		double avg_rss;
		double current_rss;
		double rss_max;
		double past_rss [4];
      // Nothing to declare in this block.

     public:
      one_bit_feedb2_cf_impl(double sample_rate, int decim_factor);
      ~one_bit_feedb2_cf_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace EnergyBeamforming
} // namespace gr

#endif /* INCLUDED_ENERGYBEAMFORMING_ONE_BIT_FEEDB2_CF_IMPL_H */

