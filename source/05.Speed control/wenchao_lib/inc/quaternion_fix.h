/**
   *************************************************** ****************************
   * @file quaternion_fix.h
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.9.27
   * @note This program is for ARM application programming
   *************************************************** ****************************
   */
#ifndef __CH_LIB_QUATERNION_FIX_H__
#define __CH_LIB_QUATERNION_FIX_H__

#ifdef __cplusplus
  extern "C" {
#endif


#include "sysinc.h"


#define Kp 100.0f
#define Ki 0.002f
#define halfT 0.005f //Half of the period

//API
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

#endif
