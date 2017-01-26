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

#ifndef INCLUDED_ENERGYBEAMFORMING_BFWEIGHT_CC_IMPL_H
#define INCLUDED_ENERGYBEAMFORMING_BFWEIGHT_CC_IMPL_H

#include <EnergyBeamforming/bfweight_cc.h>

namespace gr {
  namespace EnergyBeamforming {

    class bfweight_cc_impl : public bfweight_cc
    {
     private:
     gr_complex d_bf_weight;
      // Nothing to declare in this block.

     public:
      bfweight_cc_impl(gr_complex bf_weight);
      ~bfweight_cc_impl();

	//accessors
	  gr_complex bf_weight () const {		  
		  return d_bf_weight; }
	 
	  //manipulators
	  virtual void set_bf_weight (gr_complex bf_weight);

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace EnergyBeamforming
} // namespace gr

#endif /* INCLUDED_ENERGYBEAMFORMING_BFWEIGHT_CC_IMPL_H */

