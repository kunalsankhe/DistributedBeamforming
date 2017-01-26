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

#include <gnuradio/fxpt.h>
#include <stdio.h>
#include <gnuradio/random.h>
#include <gnuradio/io_signature.h>
#include "randphpert4_f_impl.h"

namespace gr {
  namespace EnergyBeamforming {

    randphpert4_f::sptr
    randphpert4_f::make(double rand_pert_size, int bf_flag)
    {
      return gnuradio::get_initial_sptr
        (new randphpert4_f_impl(rand_pert_size, bf_flag));
    }

    /*
     * The private constructor
     */
    randphpert4_f_impl::randphpert4_f_impl(double rand_pert_size, int bf_flag)
      : gr::sync_block("randphpert4_f",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(0, 0, 0)),
              d_rand_pert_size(rand_pert_size),
		      d_bf_flag(bf_flag)
    { 
	d_complex_gain 		= (gr_complex)1;   	
	last_in_i 		= 0;
	//seed 			= 42;
	randp 			= 0;
	rand_pert_size_rad 	= (3.141592654/180)*d_rand_pert_size;
	cum_ang 		= 0;
	i_out			= 0;
	q_out 			= 0;
	etojphi 		= (gr_complex)1;
	rand_pert_from_prev_iter = 0;
	noise_data=0; //GENESYS: Feedback Data '0' is considered as a noise 
	//tmp=0;
}

    /*
     * Our virtual destructor.
     */
    randphpert4_f_impl::~randphpert4_f_impl()
    {
    }

    int
    randphpert4_f_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      //<+OTYPE+> *out = (<+OTYPE+> *) output_items[0];

      // Do <+signal processing+>

      for (int i = 0; i < noutput_items; i++){
		if (d_bf_flag==0){ 
	   // No beamforming !
	   if (in[i] != noise_data) //GENESYS: Added to avoid compuatation for noise data in[i]==0
			    {
				   if (in[i] != in[i-1] && in[i] != in[i-2]){					
					   //printf ("Feedback from the senosr is : %d \n %d", (int)in[i], i);
					   last_in_i=in[i];
					   seed.reseed(42);
					   randp = ((float)(seed.ran1()))*2;
					   //printf ("Random Value : %f \n", randp);				   
					   if (randp<1) { randp = -rand_pert_size_rad; }
					   else 	    { randp =  rand_pert_size_rad;  }
					   // Add random phase to total cumulative phase
					   cum_ang += randp;
					   // If feedback bit is 0, undo previous random perturbation
					   if (in[i] == -1){ cum_ang -= rand_pert_from_prev_iter; 
						   //printf("feedback bit is 0, undo previous random perturbation");
						   }
					   // Compute modula-2pi cumulative phase
						cum_ang = fmod(cum_ang,6.283185308);
						i_out = gr::fxpt::cos (gr::fxpt::float_to_fixed(cum_ang)); 
						q_out = gr::fxpt::cos (gr::fxpt::float_to_fixed(cum_ang));
						etojphi = gr_complex(i_out, q_out); 
						rand_pert_from_prev_iter = randp; 
						d_complex_gain = etojphi;    
						//printf ("Perturbation: Complex Gain : %f + i%f \n", d_complex_gain.real(), d_complex_gain.imag());		
			       } // End if(in[i] != last_in_i)
			  
			   } 
	   
	   }
	   else {
		   if (in[i] != noise_data) //GENESYS: Added to avoid compuatation for noise data in[i]==0
			    {
				   if (in[i] != in[i-1] && in[i] != in[i-2]){					
					   //printf ("Feedback from the senosr is : %d \n %d", (int)in[i], i);
					   last_in_i=in[i];
					   seed.reseed(42);
					   randp = ((float)(seed.ran1()))*2;
					   //printf ("Random Value : %f \n", randp);				   
					   if (randp<1) { randp = -rand_pert_size_rad; }
					   else 	    { randp =  rand_pert_size_rad;  }
					   // Add random phase to total cumulative phase
					   cum_ang += randp;
					   // If feedback bit is 0, undo previous random perturbation
					   if (in[i] == -1){ cum_ang -= rand_pert_from_prev_iter; 
						   //printf("feedback bit is 0, undo previous random perturbation");
						   }
					   // Compute modula-2pi cumulative phase
						cum_ang = fmod(cum_ang,6.283185308);
						i_out = gr::fxpt::cos (gr::fxpt::float_to_fixed(cum_ang)); 
						q_out = gr::fxpt::sin (gr::fxpt::float_to_fixed(cum_ang));
						etojphi = gr_complex(i_out, q_out); 
						rand_pert_from_prev_iter = randp; 
						d_complex_gain = etojphi;    
						//printf ("Perturbation: Complex Gain : %f + i%f \n", d_complex_gain.real(), d_complex_gain.imag());		
			       } // End if(in[i] != last_in_i)
			  
			   } 
			} // End else-statement of if(d_bf_flag==0)
		} // End for(int i = 0; i < noutput_items; i++)
      

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }
    
    void
	randphpert4_f_impl::set_bf_flag (int bf_flag)
	{
		d_bf_flag = bf_flag;
	}

  } /* namespace EnergyBeamforming */
} /* namespace gr */

