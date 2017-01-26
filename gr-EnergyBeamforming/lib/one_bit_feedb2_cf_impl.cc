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
#include <math.h>
#include <algorithm>
#include <gnuradio/io_signature.h>
#include "one_bit_feedb2_cf_impl.h"


namespace gr {
  namespace EnergyBeamforming {

    one_bit_feedb2_cf::sptr
    one_bit_feedb2_cf::make(double sample_rate, int decim_factor)
    {
      return gnuradio::get_initial_sptr
        (new one_bit_feedb2_cf_impl(sample_rate, decim_factor));
    }

    /*
     * The private constructor
     */
    one_bit_feedb2_cf_impl::one_bit_feedb2_cf_impl(double sample_rate, int decim_factor)
      : gr::sync_decimator("one_bit_feedb2_cf",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(float)), decim_factor),
              d_sample_rate(sample_rate),
		      d_decim_factor(decim_factor)
    { 
		d_nbr_output_floats 	= 16;	
		d_treshold 		= 0.005;
		d_nbr_samples_packet 	= 900;
		d_nbr_samples_cycle 	= 10000;
		cpt_samples 		= 0;
		cpt_samples_packet 	= 0;
		feedback_symbol 	= 0;
		packet_number 		= 0;
		avg_rss 		= 0;
		current_rss 		= 0;
		rss_max 		= 0;
		//auto init = std::initializer_list<double>({0,0,0,0});
		//std::copy(init.begin(), init.end(), past_rss);
		//past_rss 		= {0,0,0,0}; 
	}

    /*
     * Our virtual destructor.
     */
    one_bit_feedb2_cf_impl::~one_bit_feedb2_cf_impl()
    {
    }

    int
    one_bit_feedb2_cf_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0]; 
	  float *out = (float *) output_items[0]; 
	  int j=0; 	// i corresponds to input index in[i], j corresponds to output index out[j]

      // Do <+signal processing+>
		for (int i = 0; i < noutput_items*d_decim_factor; i++){
	cpt_samples++;
	if (cpt_samples == 1000001){
	    cpt_samples = 1;}

	
	// Generate output
	// ---------------
	if ( (cpt_samples)%d_nbr_samples_cycle <= d_nbr_output_floats-1 ){
	  	if ( (cpt_samples)%d_nbr_samples_cycle  < d_nbr_output_floats-1 ){ 
		    out[j] = feedback_symbol; 
		    //printf ("out[%d] = %d\n", j, (int)out[j]); 
		    j+=1;}
	  	if ( (cpt_samples)%d_nbr_samples_cycle == d_nbr_output_floats-1 ){ 	
		    out[j] = feedback_symbol; 
		    printf ("Output = %d\n", (int)out[j]); 
		    j+=1; }
	} // End of if((i)%d_decim_factor <= d_nbr_output_floats-1)

	
	// Monitor input	
	// -------------
    	if (std::abs(in[i]) > d_treshold){	    
	    cpt_samples_packet++;
	    avg_rss = avg_rss + std::abs(in[i]);
	    // Packet complete
	    if (cpt_samples_packet == d_nbr_samples_packet){
		if (packet_number<1000) { packet_number +=1; }
        	else 			{ packet_number = 1; } 
		current_rss = avg_rss/(d_nbr_samples_packet);
		rss_max=*std::max_element(past_rss,past_rss+4); //GENESYS : use *std::max_element instead of *max_element
		if (current_rss > rss_max){ feedback_symbol = packet_number; }   
        	else			  { feedback_symbol = -packet_number; }
        	//printf("Feedback Symbol %d \n", feedback_symbol);
		//feedback_symbol_available = true;
		avg_rss=0; 
        	cpt_samples_packet = 0;
      		past_rss[3]=past_rss[2]; 
        	past_rss[2]=past_rss[1];
        	past_rss[1]=past_rss[0];
        	past_rss[0]=current_rss;
	    } // End of if(cpt_samples_packet == d_nbr_samples_packet)
	} // End of if(std::abs(in[i]) > d_treshold)
	else{
	    cpt_samples_packet = 0;
	    avg_rss = 0;
	} // End of else-statement of if(std::abs(in[i]) > d_treshold)
	
  } // End of for(int i = 0; i < noutput_items*d_decim_factor; i++)

  return noutput_items;
} // End of work()

  } /* namespace EnergyBeamforming */
} /* namespace gr */

