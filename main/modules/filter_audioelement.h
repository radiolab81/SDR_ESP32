#ifndef _FILTER_AUDIOELEMENT_H_
#define _FILTER_AUDIOELEMENT_H_

#include "audio_pipeline.h"
#include "audio_element.h"

esp_err_t _open_filter(audio_element_handle_t);
int _process_filter(audio_element_handle_t, char*, int);

#endif
