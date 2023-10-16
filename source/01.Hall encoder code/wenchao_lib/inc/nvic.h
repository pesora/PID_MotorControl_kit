/**
   *************************************************** ****************************
   * @file nvic.h
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.8.21
   * @note This program is the underlying programming of NVIC in ARM 
   * (attached to the secondary encapsulation of the official underlying library)
   *************************************************** ****************************
   */

#ifndef __CH_LIB_NVIC_H__
#define __CH_LIB_NVIC_H__

#ifdef __cplusplus
  extern "C" {
#endif


#include "misc.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

void NVIC_init(void);

#endif
