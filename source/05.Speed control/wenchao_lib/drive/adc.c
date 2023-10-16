/**
   *************************************************** ****************************
   * @file adc.c
   * @author Zhang Wenchao
* @qq 269426626
   * @version V1.0
   * @date 2016.9.10
   * @note This program is the underlying programming of ADC in ARM
   *************************************************** ****************************
   */

#include "sysinc.h"


  /**
  * @brief ADC initialization function
  * @code
  * // Initialize ADC channel 4
  * ADC_init(ADC_CH4);
  * @endcode
  * @param[in] ADC_CHx ADC channel number: Value: ADC_CH0~ADC_CH15
  */
void ADC_init(u8 ADC_CHx)
{
	switch(ADC_CHx)
	{
		case ADC_CH0: RCC->APB2ENR|=1<<2; GPIOA->CRL&=0XFFFFFFF0; break;
		case ADC_CH1: RCC->APB2ENR|=1<<2; GPIOA->CRL&=0XFFFFFF0F; break;
		case ADC_CH2: RCC->APB2ENR|=1<<2; GPIOA->CRL&=0XFFFFF0FF; break;
		case ADC_CH3: RCC->APB2ENR|=1<<2; GPIOA->CRL&=0XFFFF0FFF; break;
		case ADC_CH4: RCC->APB2ENR|=1<<2; GPIOA->CRL&=0XFFF0FFFF; break;
		case ADC_CH5: RCC->APB2ENR|=1<<2; GPIOA->CRL&=0XFF0FFFFF; break;
		case ADC_CH6: RCC->APB2ENR|=1<<2; GPIOA->CRL&=0XF0FFFFFF; break;
		case ADC_CH7: RCC->APB2ENR|=1<<2; GPIOA->CRL&=0X0FFFFFFF; break;
		case ADC_CH8: RCC->APB2ENR|=1<<3; GPIOB->CRL&=0XFFFFFFF0; break;
		case ADC_CH9: RCC->APB2ENR|=1<<3; GPIOB->CRL&=0XFFFFFF0F; break;
		case ADC_CH10:RCC->APB2ENR|=1<<4; GPIOC->CRL&=0XFFFFFFF0; break;
		case ADC_CH11:RCC->APB2ENR|=1<<4; GPIOC->CRL&=0XFFFFFF0F; break;
		case ADC_CH12:RCC->APB2ENR|=1<<4; GPIOC->CRL&=0XFFFFF0FF; break;
		case ADC_CH13:RCC->APB2ENR|=1<<4; GPIOC->CRL&=0XFFFF0FFF; break;
		case ADC_CH14:RCC->APB2ENR|=1<<4; GPIOC->CRL&=0XFFF0FFFF; break;
		case ADC_CH15:RCC->APB2ENR|=1<<4; GPIOC->CRL&=0XFF0FFFFF; break;
	}
	
	//Channel 10/11 settings
	RCC->APB2ENR|=1<<9; //ADC1 clock enable
	RCC->APB2RSTR|=1<<9; //ADC1 reset
	RCC->APB2RSTR&=~(1<<9); //Reset ends
	RCC->CFGR&=~(3<<14); //Clear the frequency division factor to zero
	//SYSCLK/DIV2=12M ADC clock is set to 12M, and the maximum ADC clock cannot exceed 14M!
	//Otherwise, the ADC accuracy will decrease!
	RCC->CFGR|=2<<14;
	ADC1->CR1&=0XF0FFFF; //Clear the working mode
	ADC1->CR1|= 0<<16;
	ADC1->CR1|= ~(1<<8);
	ADC1->CR2|= 1<<20;
	ADC1->CR2&= ~(1<<1);
	ADC1->CR2|= 7<<17;
	ADC1->CR2&=~(1<<11);
	ADC1->SMPR2&=~(7<<3);
	ADC1->SQR1&=~(0XF<<20);
	ADC1->SQR1|=0<<20;
	ADC1->CR2|=1<<0;
	ADC1->CR2|=1<<3;
	while(ADC1->CR2&(1<<3));
	ADC1->CR2|=1<<2;
	while(ADC1->CR2&(1<<2));
}

  /**
  * @brief Get the ADC conversion value function
  * @code
  * // Get the ADC channel 4 value
  * Get_Adc(ADC_CH4);
  * @endcode
  * @param[in] ADC_CHx ADC channel number: Value: ADC_CH0~ADC_CH15
  */
u16 Get_Adc(u8 ADC_CHx)
{
	//Set conversion sequence
	ADC1->SQR3&=0XFFFFFFE0; //Rule sequence 1 channel ch
	ADC1->SQR3|=ADC_CHx;
	ADC1->CR2|=1<<22; //Start the rule conversion channel
	while(!(ADC1->SR&1<<1)); //Wait for the conversion to end
	
	return ADC1->DR; //return adc value
}

  /**
  * @brief Get the ADC conversion average function
  * @code
  * // Get the ADC channel 4 value, the number is 20
  * Get_Adc_Average(ADC_CH4, 20);
  * @endcode
  * @param[in] ADC_CHx ADC channel number: Value: ADC_CH0~ADC_CH15
  * @param[in] times average times
  */
u16 Get_Adc_Average(u8 ADC_CHx,u8 times)
{
	u32 temp_val=0;
	u8 t;
	
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ADC_CHx);
		//SYSTICK_DelayMs(5);
	}
	
	return temp_val/times;
}