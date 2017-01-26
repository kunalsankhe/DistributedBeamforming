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

#include <stdio.h>
#include <gnuradio/io_signature.h>
#include "bfweight2_cc_impl.h"

namespace gr {
  namespace EnergyBeamforming {

    bfweight2_cc::sptr
    bfweight2_cc::make()
    {
      return gnuradio::get_initial_sptr
        (new bfweight2_cc_impl());
    }

    /*
     * The private constructor
     */
    bfweight2_cc_impl::bfweight2_cc_impl()
      : gr::sync_block("bfweight2_cc",
              gr::io_signature::make(1, 2, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {}

    /*
     * Our virtual destructor.
     */
    bfweight2_cc_impl::~bfweight2_cc_impl()
    {
    }

    int
    bfweight2_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in1 = (const gr_complex *) input_items[0];
      const gr_complex *in2 = (const gr_complex *) input_items[1];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
		for (int i = 0; i < noutput_items; i++){
			
			//printf("TxPacket Output %f + i%f",in1[i].real(),in1[i].imag()); 		
			//printf("Complex gain %f + i%f",in2[i].real(),in2[i].imag()); 		
						out[i] = in2[i]*in1[i];	    
			}
	  
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace EnergyBeamforming */
} /* namespace gr */

