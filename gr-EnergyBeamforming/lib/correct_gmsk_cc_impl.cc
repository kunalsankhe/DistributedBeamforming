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

#include <math.h>
#include <stdio.h>
#include <gnuradio/fxpt.h>
#include <gnuradio/io_signature.h>
#include "correct_gmsk_cc_impl.h"

namespace gr {
  namespace EnergyBeamforming {

    correct_gmsk_cc::sptr
    correct_gmsk_cc::make(double nbr_samples_packet, double sampling_freq)
    {
      return gnuradio::get_initial_sptr
        (new correct_gmsk_cc_impl(nbr_samples_packet, sampling_freq));
    }

    /*
     * The private constructor
     */
    correct_gmsk_cc_impl::correct_gmsk_cc_impl(double nbr_samples_packet, double sampling_freq)
      : gr::block("correct_gmsk_cc",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
              d_nbr_samples_packet(nbr_samples_packet), 
              d_sampling_freq(sampling_freq)		
{
	cpt_samples 		= 0;
	phase_shift 		= 0;
	
	
}

    /*
     * Our virtual destructor.
     */
    correct_gmsk_cc_impl::~correct_gmsk_cc_impl()
    {
    }

    void
    correct_gmsk_cc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items; 
    }

    int
    correct_gmsk_cc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *)output_items[0];

      // Do <+signal processing+>
      
      for (int i = 0; i < noutput_items; i++){ 
		cpt_samples++;
	
      	if (cpt_samples == 1){
	    phase_shift = std::arg(in[i]);
	    //printf("Phase first symbol packet = %f\n", phase_shift *180/3.1416);

	    /////////////////////// Testing write "sob" tag
	    /*const size_t item_index = i;
	    const uint64_t offset = this->nitems_written(0) + item_index;
	    pmt::pmt_t key = pmt::pmt_string_to_symbol("tx_sob");
	    pmt::pmt_t value = pmt::PMT_T;
	    this->add_item_tag(0,offset,key,value);*/
	    ///////////////////////


      	}
      	else if (cpt_samples == d_nbr_samples_packet){
	    cpt_samples = 0;
	    //printf("End of packet ! \n");


	    /////////////////////// Testing write "eob" tag
	    /*const size_t item_index = i;
	    const uint64_t offset = this->nitems_written(0) + item_index;
	    pmt::pmt_t key = pmt::pmt_string_to_symbol("tx_eob");
	    pmt::pmt_t value = pmt::PMT_T;
	    this->add_item_tag(0,offset,key,value);*/
	    ///////////////////////

      	}

      	out[i] = in[i] * std::conj( gr_complex(std::cos(phase_shift),std::sin(phase_shift)) );	
	//if (cpt_samples < 10){printf("out[%d] = %f + j%f\n",cpt_samples, std::real(out[i]), std::imag(out[i]));}
		}

      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace EnergyBeamforming */
} /* namespace gr */

