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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif


#include <gnuradio/io_signature.h>
#include <math.h>
#include <stdio.h>
#include <gnuradio/fxpt.h> 
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_permutation.h>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "ekf_ff_impl.h"

namespace gr {
  namespace EnergyBeamforming {

    ekf_ff::sptr
    ekf_ff::make(double sample_rate, double Tm, double initial_freq, const std::vector<double> &elts_Q, const std::vector<double> &elts_R)
    {
      return gnuradio::get_initial_sptr
        (new ekf_ff_impl(sample_rate, Tm, initial_freq, elts_Q, elts_R));
    }

    /*
     * The private constructor
     */
     
    static int ios[] = {sizeof (float), sizeof (float), sizeof (float), sizeof (uint64_t), sizeof (double)};
	static std::vector<int> iosig(ios, ios+sizeof(ios)/sizeof(int));

	static int ios2[] = {sizeof (float), sizeof (float), sizeof (uint64_t), sizeof (double)};
	static std::vector<int> iosig2(ios2, ios2+sizeof(ios2)/sizeof(int)); 
     
     
    ekf_ff_impl::ekf_ff_impl(double sample_rate, double Tm, double initial_freq, const std::vector<double> &elts_Q, const std::vector<double> &elts_R)
      : gr::block("ekf_ff",
              gr::io_signature::makev(1, 5, iosig),
              gr::io_signature::makev(1, 4, iosig2)),
              d_sample_rate(sample_rate), 
			  d_Tm(Tm),
			  d_initial_freq(initial_freq),
			  d_elts_Q(elts_Q),
			  d_elts_R(elts_R)
   {

	cpt_samples 		= 0;
	freq_estimate 		= 0;
	cos_phi_estimate 	= 0;
	sin_phi_estimate 	= 0;
	pi 			= 3.141592653589793;

	y 		= gsl_matrix_alloc (3, 1); 	gsl_matrix_set_zero(y);
	y_tilde_k	= gsl_matrix_alloc (3, 1);	gsl_matrix_set_zero(y_tilde_k);
	h_x_hat_k_km1	= gsl_matrix_alloc (3, 1);	gsl_matrix_set_zero(h_x_hat_k_km1);
	x_hat_k_km1 	= gsl_matrix_alloc (2, 1);	gsl_matrix_set_zero(x_hat_k_km1);
	H_k 		= gsl_matrix_alloc (3, 2);	gsl_matrix_set_zero(H_k);
	P_k_km1 	= gsl_matrix_alloc (2, 2);	gsl_matrix_set_identity(P_k_km1);
	R 		= gsl_matrix_alloc (3, 3);	gsl_matrix_set_identity(R);
	temp_1 		= gsl_matrix_alloc (3, 2);	gsl_matrix_set_zero(temp_1);
	temp_2 		= gsl_matrix_alloc (3, 3);	gsl_matrix_set_zero(temp_2);
	S_k 		= gsl_matrix_alloc (3, 3);	gsl_matrix_set_identity(S_k);
	temp_3 		= gsl_matrix_alloc (2, 3);	gsl_matrix_set_zero(temp_3);
	inv_S_k 	= gsl_matrix_alloc (3, 3);	gsl_matrix_set_identity(inv_S_k);
	K_k 		= gsl_matrix_alloc (2, 3);	gsl_matrix_set_zero(K_k);
	temp_4 		= gsl_matrix_alloc (2, 1);	gsl_matrix_set_zero(temp_4);
	x_hat_k_k 	= gsl_matrix_alloc (2, 1);	gsl_matrix_set_zero(x_hat_k_k);
	I_2 		= gsl_matrix_alloc (2, 2); 	gsl_matrix_set_identity(I_2);
	temp_5 		= gsl_matrix_alloc (2, 2);	gsl_matrix_set_identity(temp_5);
	temp_6 		= gsl_matrix_alloc (2, 2);	gsl_matrix_set_identity(temp_6);
	P_k_k 		= gsl_matrix_alloc (2, 2);	gsl_matrix_set_identity(P_k_k);
	F 		= gsl_matrix_alloc (2, 2);	gsl_matrix_set_identity(F);
	temp_7 		= gsl_matrix_alloc (2, 2);	gsl_matrix_set_identity(temp_7);
	temp_8 		= gsl_matrix_alloc (2, 2);	gsl_matrix_set_identity(temp_8);
	Q 		= gsl_matrix_alloc (2, 2);	gsl_matrix_set_identity(Q);

	// Initialze Q matrix - measured for fc = 900MHz, sampleRate = 200kHz  and feedbackRate = 20Hz
	gsl_matrix_set(Q,0,0, d_elts_Q[0]); 	gsl_matrix_set(Q,0,1, d_elts_Q[1]);
	gsl_matrix_set(Q,1,0, d_elts_Q[2]); 	gsl_matrix_set(Q,1,1, d_elts_Q[3]);
	// Initialize R matrix - measured for fc = 900MHz, sampleRate = 200kHz  and feedbackRate = 20Hz
	gsl_matrix_set(R,0,0, d_elts_R[0]); 		gsl_matrix_set(R,0,1, 0); 		gsl_matrix_set(R,0,2, 0);
	gsl_matrix_set(R,1,0, 0); 		gsl_matrix_set(R,1,1, d_elts_R[1]); 	gsl_matrix_set(R,1,2, 0);
	gsl_matrix_set(R,2,0, 0); 		gsl_matrix_set(R,2,1, 0); 		gsl_matrix_set(R,2,2, d_elts_R[2]);
	// Initialize F matrix
	gsl_matrix_set(F,0,0, 1); 	gsl_matrix_set(F,0,1, d_Tm);
	gsl_matrix_set(F,1,0, 0); 	gsl_matrix_set(F,1,1, 1);
	// Initialize x_hat_k_km1, P_k_km1, x_hat_k_k and P_k_k
	gsl_matrix_set(x_hat_k_km1,0,0, 0);
	gsl_matrix_set(x_hat_k_km1,1,0, d_initial_freq*2*pi);
	gsl_matrix_set_identity(P_k_km1);
	gsl_matrix_set(x_hat_k_k,0,0, 0);
	gsl_matrix_set(x_hat_k_k,1,0, d_initial_freq*2*pi);
	gsl_matrix_set_identity(P_k_k);
	
}

    /*
     * Our virtual destructor.
     */
    ekf_ff_impl::~ekf_ff_impl()
    {
    }

    void
    ekf_ff_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    ekf_ff_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const float *in1    = (const float *) input_items[0];
	  const float *in2    = (const float *) input_items[1];
	  const float *in3    = (const float *) input_items[2];
	  const uint64_t *in4 = (const uint64_t *) input_items[3];
	  const double *in5   = (const double *) input_items[4];
	  float *out1 = (float *)output_items[0];
	  float *out2 = (float *)output_items[1];
	  uint64_t *out3 = (uint64_t *)output_items[2];
	  double *out4 = (double *)output_items[3];

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      for (int i = 0; i < noutput_items; i++){  
      	cpt_samples++;
      	if (cpt_samples < 10000){
	    out1[i] = 0;
	    out2[i] = 0;
	    out3[i] = 0;
	    out4[i] = 0;}
      	else{
	    out1[i]  = 0;
	    out2[i]  = 0; 
	    out3[i] = 0;
	    out4[i] = 0;
	    if (in1[i] != 0){
		//std::cout <<  boost::format("[Rx packet] Full seconds %u, Frac seconds %f")% in4[i]% in5[i] << std::endl;

		// Read input LO frequency and phase offset
		freq_estimate    = in1[i];
		cos_phi_estimate = std::cos(in2[i]);
		sin_phi_estimate = std::sin(in2[i]);
		// Set state-space transition matrix with input delay
		gsl_matrix_set(F,0,0, 1); 	gsl_matrix_set(F,0,1, in3[i]);
		gsl_matrix_set(F,1,0, 0); 	gsl_matrix_set(F,1,1, 1);

		// Extended Kalman filter (EKF)
		// ----------------------------

		// ----- EKF - prediction ----- 
		// x_hat_k_km1 = F * x_hat_k_k;
		gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1, F, x_hat_k_k, 0, x_hat_k_km1);
		// P_k_km1 = F*P_k_k*F.' + Q;
		gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1, F, P_k_k, 0, temp_7);
		gsl_blas_dgemm (CblasNoTrans, CblasTrans,1, temp_7, F, 0, temp_8);
		gsl_matrix_memcpy(P_k_km1, temp_8);
		gsl_matrix_add(P_k_km1, Q);

		// ----- EKF - update ----- 
		gsl_matrix_set(y,0,0, cos_phi_estimate);
		gsl_matrix_set(y,1,0, sin_phi_estimate);
		gsl_matrix_set(y,2,0, 2*pi*freq_estimate);
		// y_tilde_k = y - h(x_hat_k_km1)
		gsl_matrix_set(h_x_hat_k_km1,0,0, std::cos(gsl_matrix_get(x_hat_k_km1,0,0)));
		gsl_matrix_set(h_x_hat_k_km1,1,0, std::sin(gsl_matrix_get(x_hat_k_km1,0,0)));
		gsl_matrix_set(h_x_hat_k_km1,2,0, gsl_matrix_get(x_hat_k_km1,1,0));
		gsl_matrix_memcpy(y_tilde_k,y);
		gsl_matrix_sub(y_tilde_k,h_x_hat_k_km1);
		// H = [...]
		gsl_matrix_set(H_k,0,0, -std::sin(gsl_matrix_get(x_hat_k_km1,0,0))); 	gsl_matrix_set(H_k,0,1, 0);
		gsl_matrix_set(H_k,1,0, std::cos(gsl_matrix_get(x_hat_k_km1,0,0))); 	gsl_matrix_set(H_k,1,1, 0);
		gsl_matrix_set(H_k,2,0, 0); 						gsl_matrix_set(H_k,2,1, 1);
		// S_k = H_k * P_k_km1 * H_k.' + R
		gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1, H_k, P_k_km1, 0, temp_1);
		gsl_blas_dgemm (CblasNoTrans, CblasTrans,1, temp_1, H_k, 0, temp_2);
		gsl_matrix_memcpy(S_k, temp_2);
		gsl_matrix_add(S_k, R);
		// K_k = P_k_km1 * H_k.' * inv(S_k)
		int s; 
		gsl_permutation* p; 
		p = gsl_permutation_calloc (3);
		gsl_linalg_LU_decomp(S_k, p, &s);	// /!\ S_k is not preserved during this operation!
		gsl_linalg_LU_invert(S_k, p, inv_S_k);
		gsl_blas_dgemm (CblasNoTrans, CblasTrans,1, P_k_km1, H_k, 0, temp_3);
		gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1, temp_3, inv_S_k, 0, K_k);
		// x_hat_k_k = x_hat_k_km1 + K_k*y_tilde_k
		gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1, K_k, y_tilde_k, 0, temp_4);
		gsl_matrix_memcpy(x_hat_k_k, x_hat_k_km1);
		gsl_matrix_add(x_hat_k_k, temp_4);
		// P_k_k = (eye(2)-K_k*H_k) * P_k_km1
		gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1, K_k, H_k, 0, temp_5);
		gsl_matrix_memcpy(temp_6,I_2);
		gsl_matrix_sub(temp_6,temp_5);
		gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1, temp_6, P_k_km1, 0, P_k_k);

		/*if (std::abs(gsl_matrix_get(x_hat_k_k,0,0)) > 3000000){ 
		    printf(" -- Phase reset -- \n\n");
		    gsl_matrix_set(x_hat_k_k,0,0,0); } // Reset absolute phase EKF */

		// ----- Output ----- 
		out1[i] = gsl_matrix_get(x_hat_k_k,0,0);	
		out2[i] = gsl_matrix_get(x_hat_k_k,1,0);
		out3[i] = in4[i];
	    	out4[i] = in5[i];

		//printf("EKF BLOCK::Filtered frequency estimate = %f \n", gsl_matrix_get(x_hat_k_k,1,0)/(2*pi));

	    } // End if(in1[i] != 0)
	} // End else-statement of if(cpt_samples < 10000)
  } // End for(int i = 0; i < noutput_items; i++)

  // Tell runtime system how many input items we consumed on
  // each input stream.

  consume_each (noutput_items);

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

  } /* namespace EnergyBeamforming */
} /* namespace gr */

