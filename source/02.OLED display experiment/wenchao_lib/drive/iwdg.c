/**
   *************************************************** ****************************
   * @file iwdg.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.8.21
   * @note This program is the underlying programming of IWDG in ARM (attached to the official underlying library package)
   *************************************************** ****************************
   */
#include "iwdg.h"

/**
  * @brief Initialize independent watchdog
  * @code
  * //Watchdog 100ms
  * WDOG_QuickInit(100);
  * @endcode
  * @param[in] timeInMs range 10¡ª¡ª10000Ms
  * @param[in] Time calculation (approximately): Tout=((4*2^prer)*rlr)/40 (ms).
  * @retval None
  */
void IWDG_QuickInit(int timeInMs)
{
	int rlr;
	
	rlr = (int)(timeInMs *10/32);
	IWDG->KR = IWDG_WriteAccess_Enable; //Enable write operations to registers IWDG_PR and IWDG_RLR
	IWDG->PR = IWDG_Prescaler_32; 		//Set the IWDG prescaler value: Set the IWDG prescaler value to 64
	IWDG->RLR = rlr;
	IWDG->KR = ((uint16_t)0xAAAA); 		//Reload the IWDG counter according to the value of the IWDG reload register
}

/**
  * @brief Feed the independent watchdog
  * @retval None
  */
void IWDG_Feed(void)
{
	IWDG->KR = ((uint16_t)0xAAAA); //Reload the IWDG counter according to the value of the IWDG reload register
}


/**
  * @brief enables independent watchdog
  * @retval None
  */
void IWDG_ENABLE(void)
{
	IWDG->KR = ((uint16_t)0xCCCC); //Enable IWDG
}