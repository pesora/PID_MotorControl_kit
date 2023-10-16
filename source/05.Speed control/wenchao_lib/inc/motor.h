/**
  ******************************************************************************
  * @file    adc.h
  * @author  张文超 
	* @qq      269426626
  * @version V1.0
  * @date    2020.02.28
  * @note    此程序蜹B6612驱动程序
  ******************************************************************************
  */
#ifndef __CH_LIB_TB6612_H__
#define __CH_LIB_TB6612_H__

#include "sysinc.h"

void motor_init(void);
void set_motor_pwm(int M_PWM);

#endif
