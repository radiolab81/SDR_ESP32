#ifndef _AGC_AUDIOELEMENT_H_
#define _AGC_AUDIOELEMENT_H_

#include "audio_pipeline.h"
#include "audio_element.h"

esp_err_t _open_agc(audio_element_handle_t);
int _process_agc(audio_element_handle_t, char*, int);

#endif
