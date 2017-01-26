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
#include "bfweight_cc_impl.h"

namespace gr {
  namespace EnergyBeamforming {

    bfweight_cc::sptr
    bfweight_cc::make(gr_complex bf_weight)
    {
      return gnuradio::get_initial_sptr
        (new bfweight_cc_impl(bf_weight));
    }

    /*
     * The private constructor
     */
    bfweight_cc_impl::bfweight_cc_impl(gr_complex bf_weight)
      : gr::sync_block("bfweight_cc",
              gr::io_signature::make(1,1, sizeof(gr_complex)),
              gr::io_signature::make(1,1, sizeof(gr_complex))),
              d_bf_weight(bf_weight)
    {}

    /*
     * Our virtual destructor.
     */
    bfweight_cc_impl::~bfweight_cc_impl()
    {
    }

    int
    bfweight_cc_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
	  gr_complex *out = (gr_complex *) output_items[0];

	  for (int i = 0; i < noutput_items; i++){
		//if(i % 200==0)
			//{
			  //printf("Complex gain %f + i%f",d_bf_weight.real(),d_bf_weight.imag()); 		
		    //}
		out[i] = d_bf_weight*in[i];
	  }
	//printf("Apply BF Weight: Complex gain %f + i%f",d_bf_weight.real(),d_bf_weight.imag());
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }
    
    void
    bfweight_cc_impl::set_bf_weight (gr_complex bf_weight)
	{
	  //printf("Apply BF Weight: Complex gain %f + i%f",d_bf_weight.real(),d_bf_weight.imag());
	  d_bf_weight = bf_weight;
	}
    

  } /* namespace EnergyBeamforming */
} /* namespace gr */

