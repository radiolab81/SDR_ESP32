#include <string.h>
#include "board.h"
#include "globals.h"
#include "esp_log.h"
#include "audio_pipeline.h"
#include "audio_element.h"
#include "i2s_stream.h"

#include "esp_dsp.h"
#include <math.h>

#include <byteswap.h>
 
static const char *TAG = "DEMODULATOR_AUDIOELEMENT";

// fast dc remover
float dc_avg, last_dc_avg, avg_diff, dc_removal_level = 0;

// fast agc
#define FASTAGC_MAX_GAIN 50
#define AGC_SAMPLES 256
float agc_val, peak_input, last_peak, target_gain, last_gain;


esp_err_t _open_demodulator(audio_element_handle_t self) {
    ESP_LOGI(TAG, "open");
    return ESP_OK;
}
 
int _process_demodulator(audio_element_handle_t self, char *buf, int len) {
    int rsize = audio_element_input(self, buf, len);

    // dc block
    dc_avg = 0; 
    
    // fast agc
    peak_input = 0;

    uint16_t index = 0;
    for (int i =0; i<rsize; i+=4) {
        index++;
        // convert bytes to samples
        // uint16_t lSample = ((((uint16_t) buf[i]) << 8) & 0xFF00) + buf[i+1];
        // uint16_t rSample = ((((uint16_t) buf[i+2]) << 8) & 0xFF00) + buf[i+3]; 
        // lSample = __bswap_16 (lSample); // (lSample>>8) | (lSample<<8);
        // rSample = __bswap_16 (rSample); // (rSample>>8) | (rSample<<8);
        // or just : 
        uint16_t lSample = ((((uint16_t) buf[i+1]) << 8) & 0xFF00) + buf[i];
        uint16_t rSample = ((((uint16_t) buf[i+3]) << 8) & 0xFF00) + buf[i+2]; 
    
        // we have now the filtered I/Q, demodulate the signal 

        // [0.0:65536.0] <- [0:65536]
        float I = lSample;
        float Q = rSample;
                 
        // rm DC [0:65536]->[-32768;32768]       
        I = I - 32768;
  	Q = Q - 32768;
            
      	// scale [-1;1]
        I = I / 32768.0;
	Q = Q / 32768.0;	 

        // fast agc
        if (global_agc_mode == AGC_ON) {
          agc_val=fabs(I);
          if(agc_val>peak_input) peak_input=agc_val;
          global_agc_gain=last_gain*(1.0-((float)index/AGC_SAMPLES))+target_gain*((float)index/AGC_SAMPLES);
                  
          //I = I * gain; -> is done in agc_audioelement
          //Q = Q * gain; -> is done in agc_audioelement
        }

        switch(global_current_demodulator) {
           case MODE_AM: // AM_DEMODULATOR     
   
                // after demodulation -> [0;1.4142 {sqrt(2)]]
                I = sqrtf((I * I) + (Q * Q));
 	        //Q = I; // right = left Audio Channel;	

                // I/Q-dc correction [-sqrt(2)/2;sqrt(2)/2]
                //I = I - 0.70710678118;
                //Q = Q - 0.70710678118;
                
                // fast I/Q-dc block
                dc_avg += I; 
                dc_removal_level = last_dc_avg + avg_diff * ((float)index/256);
		I = (I - dc_removal_level/2);
		// Q = (Q - dc_removal_level/2); // right = left Audio Channel;
           break;

          
           case MODE_USB:
           case MODE_LSB:
                //  take just the real part and that is the demodulated audio
                I = I;
                //Q = I; // right = left Audio Channel;	
	   break;

           case MODE_CW: // just like SSB but with cw_filter
                global_current_filter_bw = LPF_500;
                //  take just the real part and that is the demodulated audio
                I = I;
                //Q = I; // right = left Audio Channel;	
          
	   break;

        }
 
        // add DC -> max[0;2]       
        I = I + 1;
	//Q = Q + 1; // right = left Audio Channel;	
 
        // ->[0;65536]  
        I = I * 32768;
	//Q = Q * 32768; // right = left Audio Channel;

        lSample = I;  
        rSample = lSample; //rSample = Q; // right = left Audio Channel;  

        // convert samples to byte
        // lSample = __bswap_16 (lSample);
        //rSample = __bswap_16 (rSample);
        //
        //buf[i] = (char) (((lSample & 0xFF00) >> 8) & 0xFF);
        //buf[i+1] = (char) (lSample & 0xFF);
	//
        //buf[i+2] = (char) (((rSample & 0xFF00) >> 8) & 0xFF);
        //buf[i+3] = (char) (rSample & 0xFF);
	// or just:
        buf[i+1] = (char) (((lSample & 0xFF00) >> 8) & 0xFF);
        buf[i] = (char) (lSample & 0xFF);
        buf[i+3] = (char) (((rSample & 0xFF00) >> 8) & 0xFF);
        buf[i+2] = (char) (rSample & 0xFF);
    }
 
    // fast I/Q-dc block
    dc_avg = dc_avg / index; 
    avg_diff = dc_avg - last_dc_avg;
    last_dc_avg = dc_avg;

    // fast agc
    if (global_agc_mode == AGC_ON) {
      target_gain=1.0/peak_input;
      if(target_gain>FASTAGC_MAX_GAIN) target_gain=FASTAGC_MAX_GAIN;
      last_gain = target_gain;
    }

    rsize = audio_element_output(self, buf, rsize);
    return rsize;
}

