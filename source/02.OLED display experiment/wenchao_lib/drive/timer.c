/**
   *************************************************** ****************************
   * @file timer.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.8.22
   * @note This program is the underlying programming of ARM (attached to the secondary encapsulation of the official underlying library)
   *************************************************** ****************************
   */


#include "sysinc.h"

//Define interrupt callback function
static TIMER_CallBackType TIMER_CallBackTable[6] = {NULL};



  /**
  * @brief Microsecond timer initialization
  * @code
  * // Initialize timer 2, 100us, timer update interrupt TIM_IT_Update, response priority 2, start the timer
  * TIM_us_Init(TIM2, 100, TIM_IT_Update,2, ENABLE);
  * @endcode
  * @param[in] TIMx selects the timer module value: TIM2, TIM3, TIM4
  * @param[in] Tim_us
  * @param[in] TIM_IT common interrupt type
  * @arg TIM_IT_Update timer update interrupt
  * @arg TIM_IT_CC1 input capture interrupt-channel one
  * @arg TIM_IT_CC2 input capture interrupt-channel two
  * @arg TIM_IT_CC3 input capture interrupt-channel three
  * @arg TIM_IT_CC4 input capture interrupt-channel four
  * @param[in] Subpri preemption priority value: 0,1,2,3
  * @param[in] NewState enables/disables the timer, optional values ENABLE, DISABLE
  */
u8 TIM_us_Init(TIM_TypeDef* TIMx, u32 Tim_us, u16 TIM_IT, u16 Subpri,FunctionalState NewState)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	u16 arr;
	u16 psc;
	IRQn_Type tim_irq;
	u32 num;
	
	num = Tim_us*72;
	psc = num/20000;
	if(psc <= 1)
	{
		psc = 0;
		arr = num;
	}
	else if((psc > 1) &&(psc <= 10000))
	{
		psc--;
		arr = num/psc;
	}
	else if(psc > 10000)
	{
		return 0;
	}

	if(TIMx == TIM2)
	{
		tim_irq = TIM2_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	}
	else if(TIMx == TIM3)
	{
		tim_irq = TIM3_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	else if(TIMx == TIM4)
	{
		tim_irq = TIM4_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	}
	
	TIM_TimeBaseStructure.TIM_Period = arr; //Set the value of the active auto-reload register period for loading in the next update event
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //Set the prescaler value used as the TIMx clock frequency divisor
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //Set clock division
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM up count mode
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //Initialize the time base unit of TIMx according to the specified parameters
 
	TIM_ITConfig(TIMx,TIM_IT,NewState); //Enable the specified TIM3 interrupt and allow update interrupts
 
	NVIC_InitStructure.NVIC_IRQChannel = tim_irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //Preempt priority level 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Subpri;
	NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
	NVIC_Init(&NVIC_InitStructure); //Initialize the NVIC register
	TIM_Cmd(TIMx, NewState); //Enable TIMx
	
	return 1;
}

  /**
  * @brief millisecond timer initialization
  * @code
  * // Initialize timer 2, 100ms, timer update interrupt TIM_IT_Update, response priority 2, start the timer
  * TIM_ms_Init(TIM2, 100, TIM_IT_Update,2, ENABLE);
  * @endcode
  * @param[in] TIMx selects the timer module value: TIM2, TIM3, TIM4
  * @param[in] Tim_ms
  * @param[in] TIM_IT common interrupt type
  * @arg TIM_IT_Update timer update interrupt
  * @arg TIM_IT_CC1 input capture interrupt-channel one
  * @arg TIM_IT_CC2 input capture interrupt-channel two
  * @arg TIM_IT_CC3 input capture interrupt-channel three
  * @arg TIM_IT_CC4 input capture interrupt-channel four
  * @param[in] Subpri preemption priority value: 0,1,2,3
  * @param[in] NewState enables/disables the timer, optional values ENABLE, DISABLE
  */
u8 TIM_ms_Init(TIM_TypeDef* TIMx, u16 Tim_ms, u16 TIM_IT,u16 Subpri, FunctionalState NewState)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	u16 arr;
	u16 psc;
	u32 num;
	
	num = Tim_ms*72000;
	IRQn_Type tim_irq;
	psc = num/30000;
	if(psc <= 1)
	{
		psc = 0;
		arr = num;
	}
	else if((psc > 1) &&(psc <= 40000))
	{
		psc--;
		arr = num/psc;
	}
	else if(psc > 20000)
	{
		printf("Tim_ms is too much");
		return 0;
	}

	if(TIMx == TIM2)
	{
		tim_irq = TIM2_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	}
	else if(TIMx == TIM3)
	{
		tim_irq = TIM3_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	else if(TIMx == TIM4)
	{
		tim_irq = TIM4_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	}


	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
 
	TIM_ITConfig(TIMx,TIM_IT,NewState);

	  NVIC_InitStructure.NVIC_IRQChannel = tim_irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Subpri;
	NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
	NVIC_Init(&NVIC_InitStructure);

	TIM_ITConfig(TIMx,TIM_IT,NewState);
	TIM_Cmd(TIMx, NewState);
	return 1;
}

  /**
  * @brief timer interrupt setting function
  * @code
  * // Initialize timer 2, timer update interrupt TIM_IT_Update, response priority 2, enable timer interrupt
  * TIM_ITconfig(TIM2,TIM_IT_Update,2, ENABLE);
  * @endcode
  * @param[in] TIMx selects the timer module value: TIM2, TIM3, TIM4
  * @param[in] TIM_IT common interrupt type
  * @arg TIM_IT_Update timer update interrupt
  * @arg TIM_IT_CC1 input capture interrupt-channel one
  * @arg TIM_IT_CC2 input capture interrupt-channel two
  * @arg TIM_IT_CC3 input capture interrupt-channel three
  * @arg TIM_IT_CC4 input capture interrupt-channel four
  * @param[in] Subpri preemption priority value: 0,1,2,3
  * @param[in] NewState enables/disables the timer, optional values ENABLE, DISABLE
  */
void TIM_ITconfig(TIM_TypeDef* TIMx, u16 TIM_IT,u16 Subpri, FunctionalState NewState)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	IRQn_Type tim_irq;

	TIM_ITConfig(TIMx,TIM_IT,NewState);
	if(TIMx == TIM2)
	{
		tim_irq = TIM2_IRQn;
	}
	else if(TIMx == TIM3)
	{
		tim_irq = TIM3_IRQn;
	}
	else if(TIMx == TIM4)
	{
		tim_irq = TIM4_IRQn;
	}
	NVIC_InitStructure.NVIC_IRQChannel = tim_irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Subpri;
	NVIC_InitStructure.NVIC_IRQChannelCmd = NewState;
	NVIC_Init(&NVIC_InitStructure);
}

  /**
  * @brief PWM initialization
  * @code
  * // Initialize timer 2, channel 2, frequency 1000, start the timer
  * TIM_PWM_Init(TIM2, PWM_CH2, 1000, ENABLE);
  * @endcode
  * @param[in] TIMx selects the timer module value: TIM2, TIM3, TIM4
  * @param[in] PWM_CHx PWM channel selection value: PWM_CH1, PWM_CH2, PWM_CH3, PWM_CH4
  * @param[in] Frequency PWM frequency
  * @param[in] NewState enables/disables the timer, optional values ENABLE, DISABLE
  */
void TIM_PWM_Init(TIM_TypeDef* TIMx, u8 PWM_CHx, u32 Frequency, FunctionalState NewState)
{
u32 tim;
	u32 tep;
	
	tim = 72000000/Frequency;
	tep = 72000000%Frequency;
	if(tep >= (Frequency/2))
	{
		tim++;
	}

	GPIO_PWM_Init(TIMx, PWM_CHx);
	TIM_tim_Init(TIMx, tim, NewState);
	TIM_OCInit(TIMx, PWM_CHx);

	TIM_Cmd(TIMx, NewState);
}

  /**
  * @brief PWM setting duty cycle function
  * @code
  * // Initialize timer 2, channel 2, duty cycle 20%
  * TIM_SetCompare(TIM2, PWM_CH2, 2000);
  * @endcode
  * @param[in] TIMx selects the timer module value: TIM2, TIM3, TIM4
  * @param[in] PWM_CHx PWM channel selection value: PWM_CH1, PWM_CH2, PWM_CH3, PWM_CH4
  * @param[in] PWM_Duty duty cycle value: from 0 to 10000, representing from 0% to 100%
  */
u8 TIM_SetCompare(TIM_TypeDef* TIMx, u8 PWM_CHx, u32 PWM_Duty)
{
	if(PWM_Duty > 10000)
	{
		return 0;
	}
	u32 num;
	num = TIMx->ARR;
	PWM_Duty = PWM_Duty*(num+1)/10000;

	switch(PWM_CHx)
	{
		case PWM_CH1: TIM_SetCompare1(TIMx,PWM_Duty); break;
		case PWM_CH2: TIM_SetCompare2(TIMx,PWM_Duty); break;
		case PWM_CH3: TIM_SetCompare3(TIMx,PWM_Duty); break;
		case PWM_CH4: TIM_SetCompare4(TIMx,PWM_Duty); break;
		default: break;
	}
	return 1;
}

  /**
  * @brief Input capture initialization
  * @code
  * // Initialize timer 2, channel 2, load count value 0xffff, prescaler 0, response priority 2, digital filter 0
  * TIM_Cap_Init(TIM2, CAP_CH2, 0xffff,0, 2, 0);
  * @endcode
  * @param[in] TIMx selects the timer module value: TIM2, TIM3, TIM4
  * @param[in] CAP_CHx PWM channel selection value: CAP_CH1, CAP_CH2, CAP_CH3, CAP_CH4
  * @param[in] arr preinstalled value Value: 0~0xffff
  * @param[in] psc prescaler value Value: 0~0xffff
  * @param[in] subpri response priority, value: 0,1,2,3
  * @param[in] N filter value Value: 0~0x0f
  */
void TIM_Cap_Init(TIM_TypeDef* TIMx, u8 CAP_CHx, u16 arr, u16 psc, u16 Subpri, u8 N)
{
	u16 TIM_Channel_x;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	TIM_Channel_x = GPIO_Cap_Init(TIMx, CAP_CHx);
	TIM_Ctim_Init(TIMx, arr, psc, Subpri, ENABLE);
	
	switch(CAP_CHx)
	{
		case CAP_CH1: TIM_ITConfig(TIMx,TIM_IT_Update|TIM_IT_CC1,ENABLE);break;
		case CAP_CH2: TIM_ITConfig(TIMx,TIM_IT_Update|TIM_IT_CC2,ENABLE);break;
		case CAP_CH3: TIM_ITConfig(TIMx,TIM_IT_Update|TIM_IT_CC3,ENABLE);break;
		case CAP_CH4: TIM_ITConfig(TIMx,TIM_IT_Update|TIM_IT_CC4,ENABLE);break;
		default: break; 
	}
	TIM_ITConfig(TIMx,TIM_IT_Update|((TIM_Channel_x>>16) & 0xff),ENABLE);//Allow update interrupts and allow CC1IE to capture interrupts
	TIM_ICInitStructure.TIM_Channel = (TIM_Channel_x & 0xff); //CC1S=01 selects the input terminal IC1 and maps it to TI1
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //rising edge capture
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //Mapped to TI1
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //Configure input frequency division, no frequency division
	TIM_ICInitStructure.TIM_ICFilter = N; //IC1F configuration input filter
	TIM_ICInit(TIMx, &TIM_ICInitStructure);
	TIM_Cmd(TIMx,ENABLE );
}

  /**
  * @brief interrupt callback function
  * @code
  * // Initialize timer 2, user function app(){};
  * TIMER_CallbackInstall(HW_TIMER2,app);
  * @endcode
  * @param[in] HW_TIMERx select timer module value: HW_TIMER2, HW_TIMER3, HW_TIMER4
  * @param[in] AppCBFun user function address
  */

void TIMER_CallbackInstall(u8 HW_TIMERx, TIMER_CallBackType AppCBFun)
{
	if(AppCBFun != NULL)
	{
		TIMER_CallBackTable[HW_TIMERx] = AppCBFun;
	}
}

  /**
  * @brief timer enable function
  * @code
  * // Initialize timer 2 and enable timer interrupt
  * TIM_cmd(TIM2,ENABLE);
  * @endcode
  * @param[in] TIMx selects the timer module value: TIM2, TIM3, TIM4
  * @param[in] NewState enables/disables the timer, optional values ENABLE, DISABLE
  */
void TIM_cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
	TIM_Cmd(TIMx, NewState);
}

  /**
  * @brief The timer is an internal function used by PWM, and the user does not need to call it
  */
u8 TIM_tim_Init(TIM_TypeDef* TIMx, u32 Tim_us, FunctionalState NewState)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	u16 arr;
	u16 psc;
	u32 num;
	num = Tim_us;
	psc = num/20000;
	
	if(psc <= 1)
	{
		psc = 0;
		arr = --num;
	}
	else if((psc > 1) &&(psc <= 10000))
	{
		arr = num/psc - 1;
		psc--;
	}
	else if(psc > 10000)
	{
		printf("Tim_us is too much");
		return 0;
	}

	if(TIMx == TIM2)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	}
	else if(TIMx == TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
		else if(TIMx == TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	}

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
 
	TIM_Cmd(TIMx, NewState);
	
	return 1;
}

  /**
  * @brief GPIO is used by PWM to use internal functions, users do not need to call
  */
void GPIO_PWM_Init(TIM_TypeDef* TIMx, u8 PWM_CHx)
{
	if(TIMx == TIM2)
	{ 
		switch(PWM_CHx)
		{
		case PWM_CH1: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_0, GPIO_Mode_AF_PP); break;
		case PWM_CH2: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_1, GPIO_Mode_AF_PP); break;
		case PWM_CH3: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_2, GPIO_Mode_AF_PP); break;
		case PWM_CH4: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_3, GPIO_Mode_AF_PP); break;
		default : break; 
		}
	}
	else if(TIMx == TIM3)
	{ 
		switch(PWM_CHx)
		{
		case PWM_CH1: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_6, GPIO_Mode_AF_PP); break;
		case PWM_CH2: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_7, GPIO_Mode_AF_PP); break;
		case PWM_CH3: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_0, GPIO_Mode_AF_PP); break;
		case PWM_CH4: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_1, GPIO_Mode_AF_PP); break;
		default : break; 
		}
	}
	else if(TIMx == TIM4)
	{ 
		switch(PWM_CHx)
		{case PWM_CH1: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_6, GPIO_Mode_AF_PP); break;
		case PWM_CH2: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_7, GPIO_Mode_AF_PP); break;
		case PWM_CH3: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_8, GPIO_Mode_AF_PP); break;
		case PWM_CH4: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_9, GPIO_Mode_AF_PP); break;
		default : break; 
		}
	}
	
}

  /**
  * @brief GPIO is used by CAP to use internal functions, users do not need to call
  */
u32 GPIO_Cap_Init(TIM_TypeDef* TIMx, u8 CAP_CHx)
{
	u16 TIM_Channel_x;
	u32 IT;

	if(TIMx == TIM2)
	{ 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //Enable TIM5 clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //Enable GPIOA clock
		switch(CAP_CHx)
		{
			case CAP_CH1: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_0, GPIO_Mode_IPD);GPIO_ResetBits(GPIOA,GPIO_Pin_0);
				TIM_Channel_x = TIM_Channel_1;IT= TIM_IT_CC1;break;
			case CAP_CH2: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_1, GPIO_Mode_IPD);GPIO_ResetBits(GPIOA,GPIO_Pin_1);
				TIM_Channel_x = TIM_Channel_2; IT= TIM_IT_CC2;break;
			case CAP_CH3: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_2, GPIO_Mode_IPD);GPIO_ResetBits(GPIOA,GPIO_Pin_2);
				TIM_Channel_x = TIM_Channel_3; IT= TIM_IT_CC3;break;
			case CAP_CH4: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_3, GPIO_Mode_IPD);GPIO_ResetBits(GPIOA,GPIO_Pin_3);
				TIM_Channel_x = TIM_Channel_4; IT= TIM_IT_CC4;break;
			default : break; 
		}
	}
	else if(TIMx == TIM3)
	{ 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //Enable TIM5 clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //Enable GPIOA clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //Enable GPIOA clock
		switch(CAP_CHx)
		{
			case CAP_CH1: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_6,GPIO_Mode_IPD); GPIO_ResetBits(GPIOA,GPIO_Pin_6);
				TIM_Channel_x = TIM_Channel_1; IT= TIM_IT_CC1;break;
			case CAP_CH2: GPIO_QuickInit(HW_GPIOA, GPIO_Pin_7,GPIO_Mode_IPD); GPIO_ResetBits(GPIOA,GPIO_Pin_7);
				TIM_Channel_x = TIM_Channel_2; IT= TIM_IT_CC2;break;
			case CAP_CH3: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_0,GPIO_Mode_IPD); GPIO_ResetBits(GPIOB,GPIO_Pin_0);
				TIM_Channel_x = TIM_Channel_3; IT= TIM_IT_CC3;break;
			case CAP_CH4: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_1,GPIO_Mode_IPD); GPIO_ResetBits(GPIOB,GPIO_Pin_1);
				TIM_Channel_x = TIM_Channel_4; IT= TIM_IT_CC4;break;
			default : break; 
		}
	}
	else if(TIMx == TIM4)
	{ 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //Enable TIM5 clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //Enable GPIOA clock
		switch(CAP_CHx)
		{
			case CAP_CH1: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_6,GPIO_Mode_IPD); GPIO_ResetBits(GPIOB,GPIO_Pin_6);
				TIM_Channel_x = TIM_Channel_1; IT= TIM_IT_CC1;break;
			case CAP_CH2: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_7,GPIO_Mode_IPD); GPIO_ResetBits(GPIOB,GPIO_Pin_7);
				TIM_Channel_x = TIM_Channel_2; IT= TIM_IT_CC2;break;
			case CAP_CH3: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_8,GPIO_Mode_IPD); GPIO_ResetBits(GPIOB,GPIO_Pin_8);
				TIM_Channel_x = TIM_Channel_3;IT= TIM_IT_CC3; break;
			case CAP_CH4: GPIO_QuickInit(HW_GPIOB, GPIO_Pin_9,GPIO_Mode_IPD); GPIO_ResetBits(GPIOB,GPIO_Pin_9);	
				TIM_Channel_x = TIM_Channel_4;IT= TIM_IT_CC4; break;
			default : break; 
		}
	}
	return ((IT<<16) + TIM_Channel_x);
}

  /**
  * @brief The timer is an internal function used by PWM, and the user does not need to call it
  */
void TIM_OCInit(TIM_TypeDef* TIMx, u8 PWM_CHx)
{
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	switch(PWM_CHx)
	{
	case PWM_CH1: TIM_OC1Init(TIMx, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
		break;
	case PWM_CH2: TIM_OC2Init(TIMx, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);
		break;
	case PWM_CH3: TIM_OC3Init(TIMx, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);
		break;
	case PWM_CH4: TIM_OC4Init(TIMx, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);
		break;
	default: break;
	}
}

  /**
  * @brief internal function, users do not need to call it
  */
u8 TIM_Ctim_Init(TIM_TypeDef* TIMx, u16 arr,u16 psc, u16 Subpri, FunctionalState NewState)
{
	IRQn_Type tim_irq;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if(TIMx == TIM2)
	{
		tim_irq = TIM2_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	}
	else if(TIMx == TIM3)
	{
		tim_irq = TIM3_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	else if(TIMx == TIM4)
	{
		tim_irq = TIM4_IRQn;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	}
	TIM_TimeBaseStructure.TIM_Period = arr; //Set the counter automatic reload value
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //Prescaler
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //Set clock division: TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM up counting mode
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure); //Initialize the time base unit of TIMx according to the parameters specified in TIM_TimeBaseInitStruct
  
	NVIC_InitStructure.NVIC_IRQChannel = tim_irq;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //Preempt priority level 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Subpri;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ channel is enabled
	NVIC_Init(&NVIC_InitStructure); //Initialize the peripheral NVIC register according to the parameters specified in NVIC_InitStruct


	TIM_Cmd(TIMx, NewState);
	return 0;
}

  /**
  * @brief internal function, users do not need to call it
  */
void TIM2_IRQHandler(void) //TIM3 interrupt
{
	if(TIMER_CallBackTable[HW_TIMER2] != NULL)
	{
		TIMER_CallBackTable[HW_TIMER2]();
	}
	else
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //Clear the interrupt flag bit
	}
}

  /**
  * @brief internal function, users do not need to call it
  */
void TIM3_IRQHandler(void) //TIM3 interrupt
{
	if(TIMER_CallBackTable[HW_TIMER3] != NULL)
	{
		TIMER_CallBackTable[HW_TIMER3]();
	}
	else
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //Clear the interrupt flag bit
	}
}

  /**
  * @brief internal function, users do not need to call it
  */
void TIM4_IRQHandler(void) //TIM3 interrupt
{
	if(TIMER_CallBackTable[HW_TIMER4] != NULL)
	{
		TIMER_CallBackTable[HW_TIMER4]();
	}
	else
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update); //Clear the interrupt flag bit
	}
}
