/*
 * Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * This class gathers together all the test cases for the gr-filter
 * directory into a single test suite.  As you create new test cases,
 * add them here.
 */

#include "qa_EnergyBeamforming.h"
#include "qa_ekf_ff.h"
#include "qa_tx_packet_source.h"
#include "qa_bfweight_cc.h"
#include "qa_randphpert_f.h"
#include "qa_correct_gmsk_cc.h"
#include "qa_randphpert2_f.h"
#include "qa_one_bit_feedb2_cf.h"
#include "qa_multi_slicer_fi.h"
#include "qa_randphpert3_f.h"
#include "qa_randphpert4_f.h"
#include "qa_bfweight2_cc.h"

CppUnit::TestSuite *
qa_EnergyBeamforming::suite()
{
  CppUnit::TestSuite *s = new CppUnit::TestSuite("EnergyBeamforming");
  s->addTest(gr::EnergyBeamforming::qa_ekf_ff::suite());
  s->addTest(gr::EnergyBeamforming::qa_tx_packet_source::suite());
  s->addTest(gr::EnergyBeamforming::qa_bfweight_cc::suite());
  s->addTest(gr::EnergyBeamforming::qa_randphpert_f::suite());
  s->addTest(gr::EnergyBeamforming::qa_correct_gmsk_cc::suite()); 
  s->addTest(gr::EnergyBeamforming::qa_randphpert2_f::suite());
  s->addTest(gr::EnergyBeamforming::qa_one_bit_feedb2_cf::suite());
  s->addTest(gr::EnergyBeamforming::qa_multi_slicer_fi::suite());
  s->addTest(gr::EnergyBeamforming::qa_randphpert3_f::suite());
  s->addTest(gr::EnergyBeamforming::qa_randphpert4_f::suite());
  s->addTest(gr::EnergyBeamforming::qa_bfweight2_cc::suite());

  return s;
}
