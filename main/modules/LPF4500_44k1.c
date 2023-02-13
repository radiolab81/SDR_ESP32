#include "LPF4500_44k1.h"
/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: Low Pass
Filter model: Butterworth
Filter order: 4
Sampling Frequency: 44 KHz
Cut Frequency: 4.500000 KHz
Coefficents Quantization: 16-bit

Z domain Zeros
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000

Z domain Poles
z = 0.516181 + j -0.147423
z = 0.516181 + j 0.147423
z = 0.652146 + j -0.449661
z = 0.652146 + j 0.449661
***************************************************************/
#define Ntap 16

#define DCgain 65536


int16_t FIRCoef_4500[Ntap] = { 
        -1364,
        -1738,
        -1233,
          734,
         4472,
         9575,
        14418,
        16479,
        14418,
         9575,
         4472,
          734,
        -1233,
        -1738,
        -1364,
         -673
    };

int16_t fir_I_4500(int16_t NewSample) {


    static int16_t x[Ntap]; //input samples
    int32_t y=0;            //output sample
    int n;

    //shift the old samples
    for(n=Ntap-1; n>0; n--)
       x[n] = x[n-1];

    //Calculate the new output
    x[0] = NewSample;
    for(n=0; n<Ntap; n++)
        y += FIRCoef_4500[n] * x[n];
    
    return y / DCgain;
}


int16_t fir_Q_4500(int16_t NewSample) {


    static int16_t x[Ntap]; //input samples
    int32_t y=0;            //output sample
    int n;

    //shift the old samples
    for(n=Ntap-1; n>0; n--)
       x[n] = x[n-1];

    //Calculate the new output
    x[0] = NewSample;
    for(n=0; n<Ntap; n++)
        y += FIRCoef_4500[n] * x[n];
    
    return y / DCgain;
}
