#ifndef _MSG_H_
#define _MSG_H_

#include "audio_element.h"

struct AMessage
{
    char ucMessage;
    //char ucData[255];

} xMessage;

struct ATransmitterdMessage
{
    char ucMessage;
    uint32_t new_freq;

} xTransmitterdMessage;

#define SET_FREQ 50
//QueueHandle_t xTransmitterdQueue;


#endif
