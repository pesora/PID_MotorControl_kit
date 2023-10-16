/**
   *************************************************** ****************************
   * @file timer.h
   * @author Zhang Wenchao
* @qq 269426626
   * @version V1.0
   * @date 2016.8.22
   * @note This program is the underlying programming of ARM (attached to the secondary encapsulation of the official underlying library)
   *************************************************** ****************************
   */

#ifndef __CH_LIB_TIMER_H__
#define __CH_LIB_TIMER_H__

#ifdef __cplusplus
  extern "C" {
#endif


typedef struct
{
	int pwm1;
	int pwm2;
	int pwm3;
	int pwm4;
}PWM_DUTY;


#include "sysinc.h"

/* PWM port definition*/
#define PWM_CH1 1
#define PWM_CH2 2
#define PWM_CH3 3
#define PWM_CH4 4

/* TIMER port definition*/
#define HW_TIMER2 2
#define HW_TIMER3 3
#define HW_TIMER4 4

/* Input capture port definition*/
#define CAP_CH1 1
#define CAP_CH2 2
#define CAP_CH3 3
#define CAP_CH4 4

/* Timer interrupt callback function prototype */
typedef void (*TIMER_CallBackType)(void);


//timer2~timer5 API

u8 TIM_us_Init(TIM_TypeDef* TIMx, u32 Tim_us, u16 TIM_IT, u16 Subpri,FunctionalState NewState);
u8 TIM_ms_Init(TIM_TypeDef* TIMx, u16 Tim_ms, u16 TIM_IT, u16 Subpri,FunctionalState NewState);

void TIM_ITconfig(TIM_TypeDef* TIMx, u16 TIM_IT,u16 Subpri, FunctionalState NewState);
void TIM_cmd(TIM_TypeDef* TIMx, FunctionalState NewState);

//pwm(timer2~timer5)
void TIM_PWM_Init(TIM_TypeDef* TIMx, u8 PWM_CHx, u32 Frequency, FunctionalState NewState);
u8 TIM_SetCompare(TIM_TypeDef* TIMx, u8 PWM_CHx, u32 PWM_Duty);

u8 TIM_tim_Init(TIM_TypeDef* TIMx, u32 Tim_us, FunctionalState NewState);
void GPIO_PWM_Init(TIM_TypeDef* TIMx, u8 PWM_CHx);
void TIM_OCInit(TIM_TypeDef* TIMx, u8 PWM_CHx);

//Input capture function timer2~timer5
void TIM_Cap_Init(TIM_TypeDef* TIMx, u8 CAP_CHx, u16 arr, u16 psc, u16 Subpri, u8 N);

u8 TIM_Ctim_Init(TIM_TypeDef* TIMx, u16 arr,u16 psc, u16 Subpri, FunctionalState NewState);
u32 GPIO_Cap_Init(TIM_TypeDef* TIMx, u8 CAP_CHx);

void TIMER_CallbackInstall(u8 HW_TIMERx, TIMER_CallBackType AppCBFun);


#endif
