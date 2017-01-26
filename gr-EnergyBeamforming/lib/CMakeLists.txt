# Copyright 2011,2012,2016 Free Software Foundation, Inc.
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
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

list(APPEND EnergyBeamforming_sources
    lo_estimation_cf_impl.cc
    ekf_ff_impl.cc
    tx_packet_source_impl.cc
    bfweight_cc_impl.cc    
    correct_gmsk_cc_impl.cc
    randphpert2_f_impl.cc
    randphpert_f_impl.cc
    one_bit_feedb2_cf_impl.cc
    multi_slicer_fi_impl.cc
    randphpert3_f_impl.cc
    randphpert4_f_impl.cc
    bfweight2_cc_impl.cc
)

set(EnergyBeamforming_sources "${EnergyBeamforming_sources}" PARENT_SCOPE)
if(NOT EnergyBeamforming_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT EnergyBeamforming_sources)

add_library(gnuradio-EnergyBeamforming SHARED ${EnergyBeamforming_sources})
target_link_libraries(gnuradio-EnergyBeamforming ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-EnergyBeamforming PROPERTIES DEFINE_SYMBOL "gnuradio_EnergyBeamforming_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-EnergyBeamforming PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-EnergyBeamforming RUNTIME_COMPONENT "EnergyBeamforming_runtime" DEVEL_COMPONENT "EnergyBeamforming_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_EnergyBeamforming_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_EnergyBeamforming.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_EnergyBeamforming.cc  
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_bfweight2_cc.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_randphpert4_f.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_randphpert3_f.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_multi_slicer_fi.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_one_bit_feedb2_cf.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_randphpert2_f.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_correct_gmsk_cc.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_randphpert_f.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_bfweight_cc.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_tx_packet_source.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_ekf_ff.cc
)

add_executable(test-EnergyBeamforming ${test_EnergyBeamforming_sources})

target_link_libraries(
  test-EnergyBeamforming
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-EnergyBeamforming
)

GR_ADD_TEST(test_EnergyBeamforming test-EnergyBeamforming)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")

