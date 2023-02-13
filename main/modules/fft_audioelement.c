#include "fft_audioelement.h"

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
 
static const char *TAG = "FFT_AUDIOELEMENT";

#define N_SAMPLES 256 // Amount of real input samples
int N = N_SAMPLES;

// FFT Input array
__attribute__((aligned(16)))
int16_t x[N_SAMPLES*2];

// Pointers to result arrays
int16_t* y_cf = &x[0];

__attribute__((aligned(16)))
int16_t sum_y[N_SAMPLES/2];


esp_err_t _open_fft(audio_element_handle_t self) {
    ESP_LOGI(TAG, "open");

    esp_err_t ret;
    ret = dsps_fft2r_init_sc16(NULL, N>>1);
    if (ret  != ESP_OK)
    {
        ESP_LOGE(TAG, "Not possible to initialize FFT2R. Error = %i", ret);
        return ESP_FAIL;
    }

    return ESP_OK;
}
 
int _process_fft(audio_element_handle_t self, char *buf, int len) {
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
   
    
        // create complex-vector for fft_calculations
        x[index*2 + 0] = lSample;    
        x[index*2 + 1] = 0; 

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

    // FFT Radix-2
    dsps_fft2r_sc16_ansi(x, N);
    // Bit reverse 
    dsps_bit_rev_sc16_ansi(x, N);
    // Convert one complex vector with length N/2 to one real spectrum vector with length N/2
    dsps_cplx2real_sc16_ansi(x, N);

    for (int i = 0 ; i < N/2 ; i++) {
        y_cf[i] = 10 * log10f((y_cf[i * 2 + 0] * y_cf[i * 2 + 0] + y_cf[i * 2 + 1] * y_cf[i * 2 + 1])/N);
        sum_y[i] = fmax(y_cf[i],0);
    }

    // cpy fft to shared mem for displayd
    memcpy(&global_cb_fft_spectrum,&sum_y,N);
    // dsps_view(y1_cf, N/2, 64, 10,  -60, 40, '|');



    rsize = audio_element_output(self, buf, rsize);
    return rsize;
}
