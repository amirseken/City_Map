
#include "math_helpers.h"

//New cosine function to speed up find distance function
//Reference: W. J. Cody, W. Waite, Software Manual for the Elementary Functions. Prentice-Hall, 1980
double cos_core (double x){
  double x8, x4, x2;
  x2 = x * x;
  x4 = x2 * x2;
  x8 = x4 * x4;
  /* evaluate polynomial using Estrin's scheme */
  return (-2.7236370439787708e-7 * x2 + 2.4799852696610628e-5) * x8 +
         (-1.3888885054799695e-3 * x2 + 4.1666666636943683e-2) * x4 +
         (-4.9999999999963024e-1 * x2 + 1.0000000000000000e+0);
}
//new absolute value function to support find closest function
double new_abs (double x){
    if(x < 0)
        return -x;
    else
        return x;
}
