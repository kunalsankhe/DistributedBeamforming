# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Install public header files
########################################################################
install(FILES
    api.h
    lo_estimation_cf.h
    ekf_ff.h
    tx_packet_source.h
    bfweight_cc.h
    randphpert_f.h
    correct_gmsk_cc.h
    randphpert2_f.h
    one_bit_feedb2_cf.h
    multi_slicer_fi.h
    randphpert3_f.h
    randphpert4_f.h
    bfweight2_cc.h DESTINATION include/EnergyBeamforming
)
