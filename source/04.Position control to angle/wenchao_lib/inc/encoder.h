/**
   ***************************************************** *****************************
   * @file adc.h
   * @author Zhang Wenchao
* @qq 269426626
   * @version V1.0
   * @date 2020.02.28
   * @note This program AB encoder library file
   ***************************************************** *****************************
   */
#ifndef __CH_LIB_AB_ENCODER_H__
#define __CH_LIB_AB_ENCODER_H__

#include "sysinc.h"


#define EXTITRIGGER_TYPE EXTI_Trigger_Rising

int encoder_cnt=0;

int get_encoder_val(){
	return encoder_cnt;
}

//Get the encoder value
void encoder_exti(void)
{
	if(PCin(15) == 1){
		encoder_cnt++;
	}else{
		encoder_cnt--;;
	}
}

void encoder_init(){
	EXTI_QuickInit(HW_EXTIA, EXTI_Pin_0, 3, 3); //motor phase A
	EXTI_CallbackInstall(EXTI_Pin_0, encoder_exti);

	GPIO_QuickInit(HW_GPIOC, GPIO_Pin_15, GPIO_Mode_IPU); //motor phase B
}

#endif
