# Install script for directory: /home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/EnergyBeamforming" TYPE FILE FILES
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/api.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/lo_estimation_cf.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/ekf_ff.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/tx_packet_source.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/bfweight_cc.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/randphpert_f.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/correct_gmsk_cc.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/randphpert2_f.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/one_bit_feedb2_cf.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/multi_slicer_fi.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/randphpert3_f.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/randphpert4_f.h"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/include/EnergyBeamforming/bfweight2_cc.h"
    )
endif()

