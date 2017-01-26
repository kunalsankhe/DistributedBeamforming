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

#ifndef INCLUDED_ENERGYBEAMFORMING_RANDPHPERT3_F_IMPL_H
#define INCLUDED_ENERGYBEAMFORMING_RANDPHPERT3_F_IMPL_H

#include <EnergyBeamforming/randphpert3_f.h>
#include <gnuradio/random.h>
#include <gnuradio/analog/noise_type.h>
#include <complex>

namespace gr {
  namespace EnergyBeamforming {

    class randphpert3_f_impl : public randphpert3_f
    {
     private:
     double d_rand_pert_size; 
		int d_bf_flag; 
     gr_complex d_complex_gain;
	float last_in_i;
	gr::random seed;
	double randp;
	double rand_pert_size_rad;
	double cum_ang;
	double i_out;
	double q_out;
	gr_complex etojphi;
	double rand_pert_from_prev_iter;
	double noise_data;
	
	float tmp;
      // Nothing to declare in this block.

     public:
      randphpert3_f_impl(double rand_pert_size, int bf_flag);
      ~randphpert3_f_impl();
      
      //accessors
  int bf_flag () const { return d_bf_flag; }
 
  //manipulators
  void set_bf_flag (int bf_flag);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace EnergyBeamforming
} // namespace gr

#endif /* INCLUDED_ENERGYBEAMFORMING_RANDPHPERT3_F_IMPL_H */

