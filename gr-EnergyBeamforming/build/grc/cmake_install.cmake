# Install script for directory: /home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/gnuradio/grc/blocks" TYPE FILE FILES
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_lo_estimation_cf.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_ekf_ff.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_tx_packet_source.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_bfweight_cc.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_randphpert_f.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_correct_gmsk_cc.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_randphpert2_f.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_one_bit_feedb2_cf.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_multi_slicer_fi.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_randphpert3_f.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_randphpert4_f.xml"
    "/home/genesys/Documents/Monday-Demo/gr-EnergyBeamforming/grc/EnergyBeamforming_bfweight2_cc.xml"
    )
endif()

