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

//GENESYS include files
#include <math.h> 
#include <stdio.h>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <gnuradio/fxpt.h>
#include <gsl/gsl_fft_complex.h>
//GENESYS 
#include <iostream>
#include <gnuradio/io_signature.h>
#include "lo_estimation_cf_impl.h"

namespace gr {
  namespace EnergyBeamforming {

    lo_estimation_cf::sptr
    lo_estimation_cf::make(double sample_rate, double treshold, int nbr_samples_packet, int N, int K)
    {
      return gnuradio::get_initial_sptr
        (new lo_estimation_cf_impl(sample_rate, treshold, nbr_samples_packet, N, K));
    }

    /*
     * The private constructor
     */
    static int ios[] = {sizeof (float), sizeof (float), sizeof (float), sizeof (uint64_t), sizeof (double)};
    static std::vector<int> iosig(ios, ios+sizeof(ios)/sizeof(int)); 
     
    lo_estimation_cf_impl::lo_estimation_cf_impl(double sample_rate, double treshold, int nbr_samples_packet, int N, int K)
      : gr::block("lo_estimation_cf",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::makev(1, 5, iosig)),
              d_sample_rate(sample_rate),
			  d_treshold(treshold),
			  d_nbr_samples_packet(nbr_samples_packet),
			  d_N(N),
			  d_K(K)
    {

	Ts 			= 1/d_sample_rate;
	cpt_samples 		= 0;
	cpt_samples_packet 	= 0;
	cpt_k_1 		= 0;
	cpt_k_2 		= 0;
	sign_cpt_k_1 		= 0;
	sign_cpt_k_2 		= 0;
	pi 			= 3.141592653589793;
	last_start_time_packet 	= 0;

	/* --------------- gnuradio packet format ----------------
	We correlate against acces code and header. For 2 samples per symbol, this means
	that we are correlating against samples[33:224] (d_K=192) of the transmitted packet. Note
	that if the payload is changed, the header will change as well, and we should then
	only correlate against the access code, in samples[33:160] (d_K=128) of the transmitted packet. 
 	-----------------------------------------------------------------------------
 	|  preamble  |access code|   header   |  payload   |   CRC32   |   endbyte  |
 	-----------------------------------------------------------------------------
 	<- 2 bytes -><- 8 bytes -><- 4 bytes-><- x bytes -><- 4 bytes -><- 1 byte  ->
 	*/
        tx_header[0] = gr_complex(-0.0000,1.0000); 
        tx_header[1] = gr_complex(-0.3788,0.9255); 
        tx_header[2] = gr_complex(0.0000,1.0000); 
        tx_header[3] = gr_complex(0.3788,0.9255); 
        tx_header[4] = gr_complex(0.0000,1.0000); 
        tx_header[5] = gr_complex(-0.3788,0.9255); 
        tx_header[6] = gr_complex(0.0000,1.0000); 
        tx_header[7] = gr_complex(0.3788,0.9255); 
        tx_header[8] = gr_complex(0.0000,1.0000); 
        tx_header[9] = gr_complex(-0.3788,0.9255); 
        tx_header[10] = gr_complex(0.0000,1.0000); 
        tx_header[11] = gr_complex(0.3785,0.9256); 
        tx_header[12] = gr_complex(-0.0259,0.9997); 
        tx_header[13] = gr_complex(-0.7071,0.7071); 
        tx_header[14] = gr_complex(-0.9997,0.0259); 
        tx_header[15] = gr_complex(-0.9257,-0.3782); 
        tx_header[16] = gr_complex(-0.9997,0.0259); 
        tx_header[17] = gr_complex(-0.7071,0.7071); 
        tx_header[18] = gr_complex(-0.0259,0.9997); 
        tx_header[19] = gr_complex(0.3782,0.9257); 
        tx_header[20] = gr_complex(-0.0259,0.9997); 
        tx_header[21] = gr_complex(-0.7071,0.7071); 
        tx_header[22] = gr_complex(-0.9997,0.0259); 
        tx_header[23] = gr_complex(-0.9256,-0.3785); 
        tx_header[24] = gr_complex(-1.0000,0.0000); 
        tx_header[25] = gr_complex(-0.9256,0.3785); 
        tx_header[26] = gr_complex(-0.9997,-0.0259); 
        tx_header[27] = gr_complex(-0.7069,-0.7073); 
        tx_header[28] = gr_complex(-0.0000,-1.0000); 
        tx_header[29] = gr_complex(0.7069,-0.7073); 
        tx_header[30] = gr_complex(0.9997,-0.0259); 
        tx_header[31] = gr_complex(0.9256,0.3785); 
        tx_header[32] = gr_complex(1.0000,-0.0000); 
        tx_header[33] = gr_complex(0.9256,-0.3785); 
        tx_header[34] = gr_complex(0.9997,0.0259); 
        tx_header[35] = gr_complex(0.7071,0.7071); 
        tx_header[36] = gr_complex(0.0259,0.9997); 
        tx_header[37] = gr_complex(-0.3785,0.9256); 
        tx_header[38] = gr_complex(0.0000,1.0000); 
        tx_header[39] = gr_complex(0.3788,0.9255); 
        tx_header[40] = gr_complex(-0.0000,1.0000); 
        tx_header[41] = gr_complex(-0.3785,0.9256); 
        tx_header[42] = gr_complex(0.0259,0.9997); 
        tx_header[43] = gr_complex(0.7071,0.7071); 
        tx_header[44] = gr_complex(0.9997,0.0259); 
        tx_header[45] = gr_complex(0.9256,-0.3785); 
        tx_header[46] = gr_complex(1.0000,0.0000); 
        tx_header[47] = gr_complex(0.9256,0.3785); 
        tx_header[48] = gr_complex(0.9997,-0.0259); 
        tx_header[49] = gr_complex(0.7071,-0.7071); 
        tx_header[50] = gr_complex(0.0259,-0.9997); 
        tx_header[51] = gr_complex(-0.3782,-0.9257); 
        tx_header[52] = gr_complex(0.0259,-0.9997); 
        tx_header[53] = gr_complex(0.7073,-0.7069); 
        tx_header[54] = gr_complex(1.0000,-0.0000); 
        tx_header[55] = gr_complex(0.7073,0.7069); 
        tx_header[56] = gr_complex(0.0259,0.9997); 
        tx_header[57] = gr_complex(-0.3782,0.9257); 
        tx_header[58] = gr_complex(0.0259,0.9997); 
        tx_header[59] = gr_complex(0.7073,0.7069); 
        tx_header[60] = gr_complex(1.0000,0.0000); 
        tx_header[61] = gr_complex(0.7073,-0.7069); 
        tx_header[62] = gr_complex(0.0259,-0.9997); 
        tx_header[63] = gr_complex(-0.3785,-0.9256); 
        tx_header[64] = gr_complex(0.0000,-1.0000); 
        tx_header[65] = gr_complex(0.3788,-0.9255); 
        tx_header[66] = gr_complex(0.0000,-1.0000); 
        tx_header[67] = gr_complex(-0.3785,-0.9256); 
        tx_header[68] = gr_complex(0.0259,-0.9997); 
        tx_header[69] = gr_complex(0.7073,-0.7069); 
        tx_header[70] = gr_complex(1.0000,-0.0000); 
        tx_header[71] = gr_complex(0.7071,0.7071); 
        tx_header[72] = gr_complex(0.0000,1.0000); 
        tx_header[73] = gr_complex(-0.7069,0.7073); 
        tx_header[74] = gr_complex(-0.9997,0.0259); 
        tx_header[75] = gr_complex(-0.9257,-0.3782); 
        tx_header[76] = gr_complex(-0.9997,0.0259); 
        tx_header[77] = gr_complex(-0.7071,0.7071); 
        tx_header[78] = gr_complex(-0.0259,0.9997); 
        tx_header[79] = gr_complex(0.3785,0.9256); 
        tx_header[80] = gr_complex(-0.0000,1.0000); 
        tx_header[81] = gr_complex(-0.3788,0.9255); 
        tx_header[82] = gr_complex(0.0000,1.0000); 
        tx_header[83] = gr_complex(0.3788,0.9255); 
        tx_header[84] = gr_complex(0.0000,1.0000); 
        tx_header[85] = gr_complex(-0.3785,0.9256); 
        tx_header[86] = gr_complex(0.0259,0.9997); 
        tx_header[87] = gr_complex(0.7073,0.7069); 
        tx_header[88] = gr_complex(1.0000,0.0000); 
        tx_header[89] = gr_complex(0.7073,-0.7069); 
        tx_header[90] = gr_complex(0.0259,-0.9997); 
        tx_header[91] = gr_complex(-0.3782,-0.9257); 
        tx_header[92] = gr_complex(0.0259,-0.9997); 
        tx_header[93] = gr_complex(0.7071,-0.7071); 
        tx_header[94] = gr_complex(0.9997,-0.0259); 
        tx_header[95] = gr_complex(0.9257,0.3782); 
        tx_header[96] = gr_complex(0.9997,-0.0259); 
        tx_header[97] = gr_complex(0.7069,-0.7073); 
        tx_header[98] = gr_complex(0.0000,-1.0000); 
        tx_header[99] = gr_complex(-0.7071,-0.7071); 
        tx_header[100] = gr_complex(-1.0000,-0.0000); 
        tx_header[101] = gr_complex(-0.7073,0.7069); 
        tx_header[102] = gr_complex(-0.0259,0.9997); 
        tx_header[103] = gr_complex(0.3785,0.9256); 
        tx_header[104] = gr_complex(-0.0000,1.0000); 
        tx_header[105] = gr_complex(-0.3785,0.9256); 
        tx_header[106] = gr_complex(0.0259,0.9997); 
        tx_header[107] = gr_complex(0.7073,0.7069); 
        tx_header[108] = gr_complex(1.0000,0.0000); 
        tx_header[109] = gr_complex(0.7071,-0.7071); 
        tx_header[110] = gr_complex(0.0000,-1.0000); 
        tx_header[111] = gr_complex(-0.7071,-0.7071); 
        tx_header[112] = gr_complex(-1.0000,-0.0000); 
        tx_header[113] = gr_complex(-0.7073,0.7069); 
        tx_header[114] = gr_complex(-0.0259,0.9997); 
        tx_header[115] = gr_complex(0.3782,0.9257); 
        tx_header[116] = gr_complex(-0.0259,0.9997); 
        tx_header[117] = gr_complex(-0.7073,0.7069); 
        tx_header[118] = gr_complex(-1.0000,0.0000); 
        tx_header[119] = gr_complex(-0.7071,-0.7071); 
        tx_header[120] = gr_complex(-0.0000,-1.0000); 
        tx_header[121] = gr_complex(0.7071,-0.7071); 
        tx_header[122] = gr_complex(1.0000,-0.0000); 
        tx_header[123] = gr_complex(0.7071,0.7071); 
        tx_header[124] = gr_complex(0.0000,1.0000); 
        tx_header[125] = gr_complex(-0.7069,0.7073); 
        tx_header[126] = gr_complex(-0.9997,0.0259); 
        tx_header[127] = gr_complex(-0.9257,-0.3782); 
        tx_header[128] = gr_complex(-0.9997,0.0259); 
        tx_header[129] = gr_complex(-0.7069,0.7073); 
        tx_header[130] = gr_complex(-0.0000,1.0000); 
        tx_header[131] = gr_complex(0.7071,0.7071); 
        tx_header[132] = gr_complex(1.0000,0.0000); 
        tx_header[133] = gr_complex(0.7071,-0.7071); 
        tx_header[134] = gr_complex(0.0000,-1.0000); 
        tx_header[135] = gr_complex(-0.7071,-0.7071); 
        tx_header[136] = gr_complex(-1.0000,-0.0000); 
        tx_header[137] = gr_complex(-0.7071,0.7071); 
        tx_header[138] = gr_complex(-0.0000,1.0000); 
        tx_header[139] = gr_complex(0.7071,0.7071); 
        tx_header[140] = gr_complex(1.0000,0.0000); 
        tx_header[141] = gr_complex(0.7071,-0.7071); 
        tx_header[142] = gr_complex(0.0000,-1.0000); 
        tx_header[143] = gr_complex(-0.7071,-0.7071); 
        tx_header[144] = gr_complex(-1.0000,-0.0000); 
        tx_header[145] = gr_complex(-0.7071,0.7071); 
        tx_header[146] = gr_complex(-0.0000,1.0000); 
        tx_header[147] = gr_complex(0.7069,0.7073); 
        tx_header[148] = gr_complex(0.9997,0.0259); 
        tx_header[149] = gr_complex(0.9256,-0.3785); 
        tx_header[150] = gr_complex(1.0000,0.0000); 
        tx_header[151] = gr_complex(0.9256,0.3785); 
        tx_header[152] = gr_complex(0.9997,-0.0259); 
        tx_header[153] = gr_complex(0.7069,-0.7073); 
        tx_header[154] = gr_complex(0.0000,-1.0000); 
        tx_header[155] = gr_complex(-0.7069,-0.7073); 
        tx_header[156] = gr_complex(-0.9997,-0.0259); 
        tx_header[157] = gr_complex(-0.9256,0.3785); 
        tx_header[158] = gr_complex(-1.0000,0.0000); 
        tx_header[159] = gr_complex(-0.9256,-0.3785); 
        tx_header[160] = gr_complex(-0.9997,0.0259); 
        tx_header[161] = gr_complex(-0.7069,0.7073); 
        tx_header[162] = gr_complex(-0.0000,1.0000); 
        tx_header[163] = gr_complex(0.7071,0.7071); 
        tx_header[164] = gr_complex(1.0000,0.0000); 
        tx_header[165] = gr_complex(0.7071,-0.7071); 
        tx_header[166] = gr_complex(0.0000,-1.0000); 
        tx_header[167] = gr_complex(-0.7071,-0.7071); 
        tx_header[168] = gr_complex(-1.0000,-0.0000); 
        tx_header[169] = gr_complex(-0.7071,0.7071); 
        tx_header[170] = gr_complex(-0.0000,1.0000); 
        tx_header[171] = gr_complex(0.7071,0.7071); 
        tx_header[172] = gr_complex(1.0000,0.0000); 
        tx_header[173] = gr_complex(0.7071,-0.7071); 
        tx_header[174] = gr_complex(0.0000,-1.0000); 
        tx_header[175] = gr_complex(-0.7071,-0.7071); 
        tx_header[176] = gr_complex(-1.0000,-0.0000); 
        tx_header[177] = gr_complex(-0.7071,0.7071); 
        tx_header[178] = gr_complex(0.0000,1.0000); 
        tx_header[179] = gr_complex(0.7069,0.7073); 
        tx_header[180] = gr_complex(0.9997,0.0259); 
        tx_header[181] = gr_complex(0.9256,-0.3785); 
        tx_header[182] = gr_complex(1.0000,-0.0000); 
        tx_header[183] = gr_complex(0.9256,0.3785); 
        tx_header[184] = gr_complex(0.9997,-0.0259); 
        tx_header[185] = gr_complex(0.7069,-0.7073); 
        tx_header[186] = gr_complex(0.0000,-1.0000); 
        tx_header[187] = gr_complex(-0.7069,-0.7073); 
        tx_header[188] = gr_complex(-0.9997,-0.0259); 
        tx_header[189] = gr_complex(-0.9256,0.3785); 
        tx_header[190] = gr_complex(-1.0000,-0.0000); 
        tx_header[191] = gr_complex(-0.9256,-0.3785); 
        

}

    /*
     * Our virtual destructor.
     */
    lo_estimation_cf_impl::~lo_estimation_cf_impl()
    {
    }

    void
    lo_estimation_cf_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    lo_estimation_cf_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
	  float *out1 = (float *)output_items[0];
	  float *out2 = (float *)output_items[1];
	  float *out3 = (float *)output_items[2];
	  uint64_t *out4 = (uint64_t *)output_items[3];
	  double *out5 = (double *)output_items[4];

		// Grab all "rx_time" tags in this work call
	  const uint64_t samp0_count = this->nitems_read(0);
	  std::vector<gr::tag_t> rx_time_tags; //GENESYS gr_tag_t --> gr::tag_t 
	  get_tags_in_range(rx_time_tags, 0, samp0_count, samp0_count + noutput_items, pmt::string_to_symbol("rx_time"));
	  // Print all tags
	  BOOST_FOREACH(const gr::tag_t &rx_time_tag, rx_time_tags){
		const uint64_t offset = rx_time_tag.offset;
		const pmt::pmt_t &value = rx_time_tag.value; 
		secs = pmt::to_uint64(tuple_ref(value, 0)); //GENESYS pmt::pmt_to_uint64 --> pmt::to_uint64
		fracs = pmt::to_double(tuple_ref(value, 1)); //GENESYS pmt_tuple_ref --> tuple_ref
		offs  = offset;
		std::cout << std::endl
			<< boost::format("Time Stamp -- Full seconds %u, Frac seconds %f, Abs sample offset %u")
				% pmt::to_uint64(tuple_ref(value, 0))
				% pmt::to_double(tuple_ref(value, 1))
				% offset
				<< std::endl;
		
	  } // End BOOST_FOREACH(const gr_tag_t &rx_time_tag, rx_time_tags)
	  
	  for (int i = 0; i < noutput_items; i++){  
      	cpt_samples++;
	
	if (cpt_samples < 10000){ 
	    // Ignoring the first 10000 incoming samples, the USRP usually needs some time to adjust its AGCs
	    out1[i] = 0;
	    out2[i] = 0;
	    out3[i] = 0;
	    out4[i] = 0;
	    out5[i] = 0;} // End if(cpt_samples < 10000)
      	else{ 
	    out1[i] = 0;
	    out2[i] = 0;
	    out3[i] = 0;
	    out4[i] = 0;
	    out5[i] = 0;

	    /*if ( fmod(cpt_samples,10000)==0 ){
		fracs_pkt = fracs + (this->nitems_written(0)+i)/d_sample_rate - offs/d_sample_rate;
            	double intpart; //normalize
            	fracs_pkt = std::modf(fracs_pkt, &intpart);
            	secs_pkt  = secs + uint64_t(intpart);
		pps_secs = secs_pkt;
                pps_fracs = fracs_pkt;
		//std::cout <<  boost::format("[Rx packet] Full seconds %u, Frac seconds %f")% pps_secs% pps_fracs << std::endl;
		out4[i] = pps_secs;
	    	out5[i] = pps_fracs; }*/
		

	    if (std::abs(in[i]) > d_treshold){
		cpt_samples_packet++;
		if (cpt_samples_packet == 1){
		    // Start recording packet
		    cpt_k_1 = -1; sign_cpt_k_1 = -1;
		    cpt_k_2 = -1; sign_cpt_k_2 = -1;
		    rx_packet[cpt_samples_packet-1] = in[i];
		    start_time_recording_packet = this->nitems_written(0)+i;
		} // End if(cpt_samples_packet == 1)
		else if (cpt_samples_packet < d_nbr_samples_packet){
		    // Recording packet
		    rx_packet[cpt_samples_packet-1] = in[i];
		    if (cpt_samples_packet%2 == 0){
			if (cpt_k_1 < d_N){ 
			    cpt_k_1++; sign_cpt_k_1 = (-1)*sign_cpt_k_1;
		    	    pilotSignal_1 = sign_cpt_k_1 * (in[i]) * (in[i]);
		    	    data_fft_1[2*cpt_k_1]   = double(pilotSignal_1.real());
		    	    data_fft_1[2*cpt_k_1+1] = double(pilotSignal_1.imag()); }
		    } // End if(cpt_samples_packet%2 == 0)
		    else if (cpt_samples_packet%2 == 1){
			if (cpt_k_2 < d_N){ 
			    cpt_k_2++; sign_cpt_k_2 = (-1)*sign_cpt_k_2;
		    	    pilotSignal_2 = sign_cpt_k_2 * (in[i]) * (in[i]);
		    	    data_fft_2[2*cpt_k_2]   = double(pilotSignal_2.real());
		    	    data_fft_2[2*cpt_k_2+1] = double(pilotSignal_2.imag()); }
		    } // End else if(cpt_samples_packet%2 == 1)
		} // End else if(cpt_samples_packet < d_nbr_samples_packet)
		else if (cpt_samples_packet == d_nbr_samples_packet){
		    // Stop recording packet, performing LO frequency, phase and time estimation
		    
		    // Make LO frequency offset estimation ...
		    // ---------------------------------------
		    eff_Ts = 2*Ts;
		    // ... on first set of data
		    gsl_fft_complex_radix2_forward (data_fft_1, 1, d_N);
		    for (int k=0; k<d_N; k++){ data_abs_1[k] = sqrt( data_fft_1[2*k]*data_fft_1[2*k] + data_fft_1[2*k+1]*data_fft_1[2*k+1] );}
		    for (int k=0; k<d_N/2; k++){ pilotSpectrum_1[k] = data_abs_1[k+d_N/2];}
		    for (int k=d_N/2; k<d_N; k++){ pilotSpectrum_1[k] = data_abs_1[k-d_N/2];}	
		    max_val_1 = 0; max_ind_1 = 0;
		    for (int k=0; k<d_N; k++){ 
		    	if (pilotSpectrum_1[k]>max_val_1){max_val_1 = pilotSpectrum_1[k]; max_ind_1 = k;}}
		    // ... on second set of data
		    gsl_fft_complex_radix2_forward (data_fft_2, 1, d_N);
		    for (int k=0; k<d_N; k++){ data_abs_2[k] = sqrt( data_fft_2[2*k]*data_fft_2[2*k] + data_fft_2[2*k+1]*data_fft_2[2*k+1] );}
		    for (int k=0; k<d_N/2; k++){ pilotSpectrum_2[k] = data_abs_2[k+d_N/2];}
		    for (int k=d_N/2; k<d_N; k++){ pilotSpectrum_2[k] = data_abs_2[k-d_N/2];}	
		    max_val_2 = 0; max_ind_2 = 0;
		    for (int k=0; k<d_N; k++){ 
		    	if (pilotSpectrum_2[k]>max_val_2){max_val_2 = pilotSpectrum_2[k]; max_ind_2 = k;}}
		    // Determining if first of second set of data has to be used
		    if (max_val_1 >= max_val_2){
		        max_val = max_val_1; max_ind = max_ind_1;
		        Y_before = pilotSpectrum_1[max_ind_1-1];
		        Y_after = pilotSpectrum_1[max_ind_1+1]; }
		    else{
		    	max_val = max_val_2;
		    	max_ind = max_ind_2;
		    	Y_before = pilotSpectrum_2[max_ind_2-1];
		    	Y_after = pilotSpectrum_2[max_ind_2+1]; }
		    // Computing LO frequency offset estimate
		    if (Y_after>Y_before){IPlus = 1;} else{IPlus = 0;}
		    max_ind_final = double(max_ind) + IPlus*(Y_after/(Y_after+max_val)) - (1-IPlus)*(Y_before/(Y_before+max_val));
		    freq_estimate_double_lo = (double(max_ind_final)-0-double(d_N)/2)/double(d_N) / eff_Ts;
		    freq_estimate   = freq_estimate_double_lo/2;	
		    //printf("LO frequency estimate = %f  \n", freq_estimate );

		    // Make LO phase offset and timing estimation
		    // ------------------------------------------ 
		    // Perform correlation with transmitted packet header
		    for (int n=0; n < d_nbr_samples_packet-d_K; n++){
			corr_output[n] = gr_complex(0,0);
			for (int k=0; k<d_K; k++){ 
		    	    phase_coeff    = 2*pi*freq_estimate*(k*Ts);
		    	    corr_output[n]  = corr_output[n] + rx_packet[k+n]*(std::conj(tx_header[k]*(gr_complex(std::cos(phase_coeff),std::sin(phase_coeff))))); }
		    } // End for(int n=0; n < d_nbr_samples_packet-d_K; n++)
		    // Find maximum correlator output
		    corr_output_max = corr_output[0];
		    index_max  	= 0;
		    for (int n=1; n<d_nbr_samples_packet-d_K; n++){
			if ( std::abs(corr_output[n])>std::abs(corr_output_max) ){
			    corr_output_max = corr_output[n];
			    index_max  	= n;}
		    }
		    phase_estimate = std::arg(corr_output_max);
		    //printf("LO phase estimate = %f  \n", phase_estimate );

		    // Timing estimation
		    // -----------------
		    start_time_packet = start_time_recording_packet + index_max;
		    delay_estimate    = (start_time_packet - last_start_time_packet)/d_sample_rate;
		    //printf("Delay estimate = %d samples\n", start_time_packet - last_start_time_packet );
		    last_start_time_packet = start_time_packet;
		    //printf("Delay estimate = %f  \n", delay_estimate );
			
		    // Time-stamp calculation
            	    fracs_pkt = fracs + start_time_packet/d_sample_rate - offs/d_sample_rate;
            	    double intpart; //normalize
            	    fracs_pkt = std::modf(fracs_pkt, &intpart);
            	    secs_pkt  = secs + uint64_t(intpart);
		    pps_secs = secs_pkt;
                    pps_fracs = fracs_pkt;
		    //std::cout <<  boost::format("[Rx packet] Full seconds %u, Frac seconds %f")% pps_secs% pps_fracs << std::endl;

		    out1[i] = freq_estimate;
	    	    out2[i] = phase_estimate;
	    	    out3[i] = delay_estimate;
		    out4[i] = pps_secs;
	    	    out5[i] = pps_fracs;
		    cpt_samples_packet = 0;

		} // End else if(cpt_samples_packet == d_nbr_samples_packet)
	    } // End if(std::abs(in[i]) > d_treshold)
	    else{ 
		cpt_samples_packet = 0;

	    } // End else-statement of if(std::abs(in[i]) > d_treshold)

	} // End else-statement of if(cpt_samples < 10000)
  } // End for(int i = 0; i < noutput_items; i++)

  // Tell runtime system how many input items we consumed on
  // each input stream.

  consume_each (noutput_items);

  // Tell runtime system how many output items we produced.
  return noutput_items;
}

  } /* namespace EnergyBeamforming */
} /* namespace gr */

