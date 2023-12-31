/**
   *************************************************** ****************************
   * @file my_math.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.9.25
   * @note This program is the underlying application MY_MATH
   *************************************************** ****************************
   */
#include "sysinc.h"



  /**
  * @brief Quick square root
  * @param[in] number number
  */
float math_rsqrt(float number)
{
     long i;
     float x2, y;
     const float threehalfs = 1.5F;

     x2 = number * 0.5F;
     y = number;
     i = * ( long * ) &y; // evil floating point bit level hacking(?????????hack)
     i = 0x5f3759df - ( i >> 1 ); // what the fuck?(??????????)
     y = * ( float * ) &i;
     y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration (???????)
     y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed(?????,????)

     return y;
}