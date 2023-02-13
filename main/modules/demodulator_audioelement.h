#ifndef _DEMODULATOR_AUDIOELEMENT_H_
#define _DEMODULATOR_AUDIOELEMENT_H_

#include "audio_pipeline.h"
#include "audio_element.h"

esp_err_t _open_demodulator(audio_element_handle_t);
int _process_demodulator(audio_element_handle_t, char*, int);

#endif
