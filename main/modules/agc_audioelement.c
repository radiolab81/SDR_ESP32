#include "agc_audioelement.h"

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
 
static const char *TAG = "AGC_AUDIOELEMENT";


esp_err_t _open_agc(audio_element_handle_t self) {
    ESP_LOGI(TAG, "open");
    return ESP_OK;
}
 

int _process_agc(audio_element_handle_t self, char *buf, int len) {
    int rsize = audio_element_input(self, buf, len);

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

        // agc
        lSample = lSample * global_agc_gain; 
        rSample = rSample * global_agc_gain; 
        
        // convert samples to byte
        //lSample = __bswap_16 (lSample);
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

    rsize = audio_element_output(self, buf, rsize);
    return rsize;
}
