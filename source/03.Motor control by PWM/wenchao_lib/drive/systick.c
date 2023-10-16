/**
   *************************************************** ****************************
   * @file systick.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.8.22
   * @note This program is the underlying programming of SysTick in ARM (attached to the secondary encapsulation of the official underlying library)
   *************************************************** ****************************
   */
#include "systick.h"

static u8 fac_us=0;//us delay multiplier
static u16 fac_ms=0;//ms delay multiplier


  /**
  * @brief initialize SysTick
  * @code
  * //SysTick sets the interrupt period to 10000us (10ms)
  * SYSTICK_Init(10000);
  * @endcode
  * @param[in] timeInUs time, in microseconds
  * @retval None
  */
void SYSTICK_Init(int timeInUs)
{
	/* Set clock source = core clock */
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //Select external clock HCLK/8
	fac_us=SystemCoreClock/8000000; //1/8 of the system clock fac_us /= 1000000;
	fac_ms = fac_us * 1000;
	/* every 100ms in an int */
	SysTick->LOAD = fac_us * timeInUs;
}
/**
  * @brief After configuring SysTick, you can apply the function DelayMs DelayUs
  * @code
  * SYSTICK_DelayInit();
  * SYSTICK_DelayMs(20);
  * @endcode
  * @retval None
  */
void SYSTICK_DelayInit(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); //Select external clock HCLK/8
	fac_us=SystemCoreClock/8000000; //1/8 of the system clock
	fac_ms=(u16)fac_us*1000; 		//Under non-ucos, represents the number of systick clocks required for each ms
	SYSTICK_Cmd(true);
	SYSTICK_ITConfig(false);
}

  /**
  * @brief Enable or disable SysTick
  * @code
  *   						//Enable
  * SYSTICK_Cmd(true);
  * @endcode
  * @param[in] NewState
  * @arg true enable
  * @arg false disabled
  * @retval None
  */
void SYSTICK_Cmd(bool NewState)
{
	(true == NewState)?(SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk):(SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk);
}

/**
  * @brief Turn on SysTick interrupt
  * @code
  *      				//example
  * SYSTICK_ITConfig(true);
  * @endcode
  * @param[in] NewState state
  * @arg true enable
  * @arg false disabled
  * @retval None
  */
void SYSTICK_ITConfig(bool NewState)
{
	(true == NewState)?(SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk):(SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk);
}

/**
  * @brief Subtle level delay
  * @code
  * //Delay 100us
  * SYSTICK_DelayUs(100);
  * @endcode
  * @retval None
  */
void SYSTICK_DelayUs(int nus)
{
	u32 temp;
	SysTick->LOAD=nus*fac_us; 	//Time loading
	SysTick->VAL=0x00; 			//Clear the counter
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //Start the countdown
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16))); 			//Wait for time to arrive
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; 	//Close the counter
	SysTick->VAL =0X00; 						//Clear the counter
}


/**
  * @brief millisecond delay
  * @code
  * //Delay 100ms
  * SYSTICK_DelayMs(100);
  * @endcode
  * @retval None
  */
void SYSTICK_DelayMs(int ms)
{
	uint32_t temp;
	
	uint32_t i;
	SysTick->LOAD = fac_ms;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	for(i = 0; i < ms; i++)
	{
		SysTick->VAL = 0;
		do
		{
			temp = SysTick->CTRL;
		}
		while((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));
	}
}


/**
  * @brief Get the current value of System tick timer
  * @retval returns the current value of System tick timer
  */
int SYSTICK_GetVal(void)
{
	return SysTick->VAL;
}
