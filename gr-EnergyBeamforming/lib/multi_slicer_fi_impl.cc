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
#include "multi_slicer_fi_impl.h"

namespace gr {
  namespace EnergyBeamforming {

    multi_slicer_fi::sptr
    multi_slicer_fi::make(double upper_th, double lower_th)
    {
      return gnuradio::get_initial_sptr
        (new multi_slicer_fi_impl(upper_th, lower_th));
    }

    /*
     * The private constructor
     */
    multi_slicer_fi_impl::multi_slicer_fi_impl(double upper_th, double lower_th)
      : gr::sync_block("multi_slicer_fi",
              gr::io_signature::make(1, 1, sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(int8_t))),
              d_upper_th(upper_th),
              d_lower_th(lower_th)
    {}

    /*
     * Our virtual destructor.
     */
    multi_slicer_fi_impl::~multi_slicer_fi_impl()
    {
    }

    int
    multi_slicer_fi_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const float *in = (const float *) input_items[0];
      int8_t *out = (int8_t *) output_items[0];

      // Do <+signal processing+>
      
       for (int i = 0; i < noutput_items; i++){
		   
		   if (in[i]>d_upper_th)
				{out[i]=1;}		   
		   else if (in[i]<d_lower_th)
				{out[i]=-1;}
		   else
				{out[i]= 0;}	
		
		   
	   }
      

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace EnergyBeamforming */
} /* namespace gr */

