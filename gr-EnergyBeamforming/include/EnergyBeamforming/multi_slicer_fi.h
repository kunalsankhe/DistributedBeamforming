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


#ifndef INCLUDED_ENERGYBEAMFORMING_MULTI_SLICER_FI_H
#define INCLUDED_ENERGYBEAMFORMING_MULTI_SLICER_FI_H

#include <EnergyBeamforming/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace EnergyBeamforming {

    /*!
     * \brief <+description of block+>
     * \ingroup EnergyBeamforming
     *
     */
    class ENERGYBEAMFORMING_API multi_slicer_fi : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<multi_slicer_fi> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of EnergyBeamforming::multi_slicer_fi.
       *
       * To avoid accidental use of raw pointers, EnergyBeamforming::multi_slicer_fi's
       * constructor is in a private implementation
       * class. EnergyBeamforming::multi_slicer_fi::make is the public interface for
       * creating new instances.
       */
      static sptr make(double upper_th, double lower_th);
    };

  } // namespace EnergyBeamforming
} // namespace gr

#endif /* INCLUDED_ENERGYBEAMFORMING_MULTI_SLICER_FI_H */

