#include "dc_block.h"

/*
https://www.dsprelated.com/freebooks/filters/DC_Blocker.html
Here, x denotes the current input sample, and y denotes the current output sample.
The variables xm1 and ym1 hold once-delayed input and output samples, respectively (and are typically initialized to zero). 
In this implementation, the pole is fixed at $ R=0.995, which corresponds to an adaptation time-constant of approximately $ 1/(1-R) = 200$ samples.
A smaller $ R$ value allows faster tracking of ``wandering dc levels'', but at the cost of greater low-frequency attenuation. */

float dc_block(float x)  {
  static float xm1, ym1;
  float y = x - xm1 + 0.99 * ym1;
  xm1 = x;
  ym1 = y;
  return y;
}
