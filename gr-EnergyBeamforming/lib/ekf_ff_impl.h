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

#ifndef INCLUDED_ENERGYBEAMFORMING_EKF_FF_IMPL_H
#define INCLUDED_ENERGYBEAMFORMING_EKF_FF_IMPL_H

#include <EnergyBeamforming/ekf_ff.h>
#include <iostream>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>

namespace gr {
  namespace EnergyBeamforming {

    class ekf_ff_impl : public ekf_ff
    {
     private:     
		double d_sample_rate;
		std::vector<double> d_elts_Q;
		std::vector<double> d_elts_R;
		double d_Tm;
		double d_initial_freq;

		int cpt_samples;
		float freq_estimate;
		float cos_phi_estimate;
		float sin_phi_estimate;
		float pi;

		gsl_matrix*  y;
		gsl_matrix*  y_tilde_k;
		gsl_matrix*  h_x_hat_k_km1;
		gsl_matrix*  x_hat_k_km1;
		gsl_matrix*  H_k;
		gsl_matrix*  P_k_km1;
		gsl_matrix*  R;
		gsl_matrix*  temp_1;
		gsl_matrix*  temp_2;
		gsl_matrix*  S_k;
		gsl_matrix*  temp_3;
		gsl_matrix*  inv_S_k;
		gsl_matrix*  K_k;
		gsl_matrix*  temp_4;
		gsl_matrix*  x_hat_k_k;
		gsl_matrix*  I_2;
		gsl_matrix*  temp_5;
		gsl_matrix*  temp_6;
		gsl_matrix*  P_k_k;
		gsl_matrix*  F;
		gsl_matrix*  temp_7;
		gsl_matrix*  temp_8;
		gsl_matrix*  Q;
      // Nothing to declare in this block.

     public:
      ekf_ff_impl(double sample_rate, double Tm, double initial_freq, const std::vector<double> &elts_Q, const std::vector<double> &elts_R);
      ~ekf_ff_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace EnergyBeamforming
} // namespace gr

#endif /* INCLUDED_ENERGYBEAMFORMING_EKF_FF_IMPL_H */

