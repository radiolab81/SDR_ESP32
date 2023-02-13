#include "LPF500_44k1.h"
/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: Low Pass
Filter model: Butterworth
Filter order: 3
Sampling Frequency: 44 KHz
Cut Frequency: 0.500000 KHz
Coefficents Quantization: 16-bit

Z domain Zeros
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000

Z domain Poles
z = 0.931184 + j -0.000000
z = 0.963185 + j -0.059523
z = 0.963185 + j 0.059523
***************************************************************/
#define Ntap 32

#define DCgain 524288

int16_t FIRCoef_500[Ntap] = { 
        11151,
        11929,
        12718,
        13516,
        14317,
        15116,
        15908,
        16685,
        17440,
        18162,
        18841,
        19463,
        20011,
        20461,
        20777,
        20895,
        20777,
        20461,
        20011,
        19463,
        18841,
        18162,
        17440,
        16685,
        15908,
        15116,
        14317,
        13516,
        12718,
        11929,
        11151,
        10389
    };

int16_t fir_I_500(int16_t NewSample) {
   
    static int16_t x[Ntap]; //input samples
    int32_t y=0;            //output sample
    int n;

    //shift the old samples
    for(n=Ntap-1; n>0; n--)
       x[n] = x[n-1];

    //Calculate the new output
    x[0] = NewSample;
    for(n=0; n<Ntap; n++)
        y += FIRCoef_500[n] * x[n];
    
    return y / DCgain;
}

int16_t fir_Q_500(int16_t NewSample) {
   
    static int16_t x[Ntap]; //input samples
    int32_t y=0;            //output sample
    int n;

    //shift the old samples
    for(n=Ntap-1; n>0; n--)
       x[n] = x[n-1];

    //Calculate the new output
    x[0] = NewSample;
    for(n=0; n<Ntap; n++)
        y += FIRCoef_500[n] * x[n];
    
    return y / DCgain;
}
