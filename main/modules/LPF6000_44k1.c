#include "LPF6000_44k1.h"
/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: Low Pass
Filter model: Raised Cosine
Roll Off Factor: 0.700000
Sampling Frequency: 44 KHz
Cut Frequency: 6.000000 KHz
Coefficents Quantization: 16-bit
***************************************************************/
#define Ntap 16
#define DCgain 65536

int16_t FIRCoef_6000[Ntap] = { 
          -61,
         -663,
        -1435,
         -857,
         2668,
         8944,
        15233,
        17891,
        15233,
         8944,
         2668,
         -857,
        -1435,
         -663,
          -61,
          -13
    };

int16_t fir_I_6000(int16_t NewSample) {


    static int16_t x[Ntap]; //input samples
    int32_t y=0;            //output sample
    int n;

    //shift the old samples
    for(n=Ntap-1; n>0; n--)
       x[n] = x[n-1];

    //Calculate the new output
    x[0] = NewSample;
    for(n=0; n<Ntap; n++)
        y += FIRCoef_6000[n] * x[n];
    
    return y / DCgain;
}


int16_t fir_Q_6000(int16_t NewSample) {


    static int16_t x[Ntap]; //input samples
    int32_t y=0;            //output sample
    int n;

    //shift the old samples
    for(n=Ntap-1; n>0; n--)
       x[n] = x[n-1];

    //Calculate the new output
    x[0] = NewSample;
    for(n=0; n<Ntap; n++)
        y += FIRCoef_6000[n] * x[n];
    
    return y / DCgain;
}
