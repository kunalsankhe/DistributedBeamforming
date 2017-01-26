#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2016 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import EnergyBeamforming_swig as EnergyBeamforming

class qa_tx_packet_source (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        src_freq = (900, 900, 900, 900, 900)
        src_phase = (0, 0, 0, 0, 0)
        src_seconds = (1, 1, 1, 1, 1)
        src_frac = (0.01, 0.01, 0.01, 0.01, 0.01)        
        expected_result = (9, 16, 30.25, 4, 9)
        src1 = blocks.vector_source_f (src_freq)
        src2 = blocks.vector_source_f (src_phase)
        src3 = blocks.vector_source_f (src_seconds)
        src4 = blocks.vector_source_f (src_frac)
        sqr = EnergyBeamforming.tx_packet_source(200000,0.04,1500,964000000, 892000000)
        dst = blocks.vector_sink_f ()
        self.tb.connect (src1, sqr)
        self.tb.connect (src2, sqr)
        self.tb.connect (src3, sqr)
        self.tb.connect (src4, sqr)           
        self.tb.connect (sqr, dst)
        self.tb.run ()        
        result_data = dst.data ()
        self.assertFloatTuplesAlmostEqual (expected_result, result_data, 6)
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_tx_packet_source, "qa_tx_packet_source.xml")
