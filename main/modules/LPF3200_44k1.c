#include "LPF3200_44k1.h"
/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: Low Pass
Filter model: Butterworth
Filter order: 6
Sampling Frequency: 44 KHz
Cut Frequency: 3.200000 KHz
Coefficents Quantization: 16-bit

Z domain Zeros
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000

Z domain Poles
z = 0.629946 + j -0.079953
z = 0.629946 + j 0.079953
z = 0.684689 + j -0.237417
z = 0.684689 + j 0.237417
z = 0.806006 + j -0.381782
z = 0.806006 + j 0.381782
***************************************************************/
#define Ntap 16

#define DCgain 131072
int16_t FIRCoef_3200[Ntap] = { 
       -1619,
          497,
         3673,
         7671,
        12003,
        15967,
        18779,
        19800,
        18779,
        15967,
        12003,
         7671,
         3673,
          497,
        -1619,
        -2671
    };

int16_t fir_I_3200(int16_t NewSample) {
    static int16_t x[Ntap]; //input samples
    int32_t y=0;            //output sample
    int n;

    //shift the old samples
    for(n=Ntap-1; n>0; n--)
       x[n] = x[n-1];

    //Calculate the new output
    x[0] = NewSample;
    for(n=0; n<Ntap; n++)
        y += FIRCoef_3200[n] * x[n];
    
    return y / DCgain;
}


/**************************************************************
WinFilter version 0.8
http://www.winfilter.20m.com
akundert@hotmail.com

Filter type: Low Pass
Filter model: Butterworth
Filter order: 2
Sampling Frequency: 44 KHz
Cut Frequency: 3.200000 KHz
Coefficents Quantization: 16-bit

Z domain Zeros
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000

Z domain Poles
z = 0.684689 + j -0.237417
z = 0.684689 + j 0.237417
***************************************************************/

int16_t fir_Q_3200(int16_t NewSample) {
    static int16_t x[Ntap]; //input samples
    int32_t y=0;            //output sample
    int n;

    //shift the old samples
    for(n=Ntap-1; n>0; n--)
       x[n] = x[n-1];

    //Calculate the new output
    x[0] = NewSample;
    for(n=0; n<Ntap; n++)
        y += FIRCoef_3200[n] * x[n];
    
    return y / DCgain;
}
