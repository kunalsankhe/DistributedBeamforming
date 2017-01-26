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


#ifndef INCLUDED_ENERGYBEAMFORMING_RANDPHPERT4_F_H
#define INCLUDED_ENERGYBEAMFORMING_RANDPHPERT4_F_H

#include <EnergyBeamforming/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace EnergyBeamforming {

    /*!
     * \brief <+description of block+>
     * \ingroup EnergyBeamforming
     *
     */
    class ENERGYBEAMFORMING_API randphpert4_f : virtual public gr::sync_block
    {
		
	 private:
	 //gr_complex d_complex_gain;
	 	
     public:
      typedef boost::shared_ptr<randphpert4_f> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of EnergyBeamforming::randphpert4_f.
       *
       * To avoid accidental use of raw pointers, EnergyBeamforming::randphpert4_f's
       * constructor is in a private implementation
       * class. EnergyBeamforming::randphpert4_f::make is the public interface for
       * creating new instances.
       */
      static sptr make(double rand_pert_size, int bf_flag);
      
      virtual gr_complex poll_complex_gain() const=0; 
      
      virtual int bf_flag () const=0;
      
    };

  } // namespace EnergyBeamforming
} // namespace gr

#endif /* INCLUDED_ENERGYBEAMFORMING_RANDPHPERT4_F_H */

