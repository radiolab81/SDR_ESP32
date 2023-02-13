#include "globals.h"

uint8_t global_agc_mode;
float global_agc_gain = 1.0;

uint8_t global_volume_setting = 75;

// shared mem buffer for fft -> displayd
int16_t global_cb_fft_spectrum[256] = {0};

// shared mem demodulator (AM,USB,LSB,CW ...) -> displayd
uint8_t global_current_demodulator = MODE_AM;

// shared mem LPF BW -> displayd
uint8_t global_current_filter_bw = LPF_6000;


