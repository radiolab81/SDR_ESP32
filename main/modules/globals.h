#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdint.h>

#define MODE_AM  0
#define MODE_USB 1
#define MODE_LSB 2
#define MODE_CW  3

#define LPF_500  0
#define LPF_3200 1
#define LPF_4500 2
#define LPF_6000 3

#define USE_INTERNAL_AUDIODAC

// override by menuconfig
#ifdef CONFIG_USE_INTERNAL_AUDIODAC
  #define USE_INTERNAL_AUDIODAC
#endif

#define AGC_OFF 0 
#define AGC_ON  1
extern uint8_t global_agc_mode;
extern float global_agc_gain;

extern uint8_t global_volume_setting;

// shared mem buffer for fft -> displayd
extern int16_t global_cb_fft_spectrum[256];

// shared mem demodulator (AM,USB,LSB,CW ...) -> displayd
extern uint8_t global_current_demodulator;

// shared mem LPF BW -> displayd
extern uint8_t global_current_filter_bw;


#endif
