/**
   *************************************************** ****************************
   * @file exti.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.8.22
   * @note This program is the underlying programming of EXTI in ARM (attached to the secondary encapsulation of the official underlying library)
   *************************************************** ****************************
   */


#include "sysinc.h"

static GPIO_CallBackType GPIO_CallBackTable[16] = {NULL};


  /**
  * @brief External interrupt fast initialization program, default interrupt enable and falling edge trigger
  * @code
  * // Initialize extiA, pin10, main priority preepri=2, response priority subpri=2, enable interrupt ITsta=ENABLE
  * EXTI_QuickInit(HW_EXTIA, EXTI_Pin_10, 2, 2);
  * @endcode
  * @param[in] instance module number
  * @arg HW_EXTIA A port
  * @arg HW_EXTIB B port
  * @arg HW_EXTIC C port
  * @arg HW_EXTID D port
  * @arg HW_EXTIE E port
  * @param[in] EXTI_Pin_x: x is the GPIO pin number, optional values 0,1,2....15
  * @param[in] preepri preemption priority, optional values 0,1,2,3
  * @param[in] subpri response priority, optional values 0,1,2,3
  */
void EXTI_QuickInit(int instance, int EXTI_Pin_x,int preepri, int subpri)
{
	int EXTIx_IRQn;

	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_QuickInit(instance, 0x01 << EXTI_Pin_x, GPIO_Mode_IPU);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE); //Enable multiplexing function clock

	GPIO_EXTILineConfig(instance, EXTI_Pin_x);

	EXTI_InitStructure.EXTI_Line = 0x01 << EXTI_Pin_x;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //Initialize the peripheral EXTI register according to the parameters specified in EXTI_InitStruct

	switch(EXTI_Pin_x)
	{
	case EXTI_Pin_0: EXTIx_IRQn = EXTI0_IRQn; break;
	case EXTI_Pin_1: EXTIx_IRQn = EXTI1_IRQn; break;
	case EXTI_Pin_2: EXTIx_IRQn = EXTI2_IRQn; break;
	case EXTI_Pin_3: EXTIx_IRQn = EXTI3_IRQn; break;
	case EXTI_Pin_4: EXTIx_IRQn = EXTI4_IRQn; break;
	case EXTI_Pin_5: EXTIx_IRQn = EXTI9_5_IRQn; break;
	case EXTI_Pin_6: EXTIx_IRQn = EXTI9_5_IRQn; break;
	case EXTI_Pin_7: EXTIx_IRQn = EXTI9_5_IRQn; break;
	case EXTI_Pin_8: EXTIx_IRQn = EXTI9_5_IRQn; break;
	case EXTI_Pin_9: EXTIx_IRQn = EXTI9_5_IRQn; break;
	case EXTI_Pin_10: EXTIx_IRQn = EXTI15_10_IRQn; break;
	case EXTI_Pin_11: EXTIx_IRQn = EXTI15_10_IRQn; break;
	case EXTI_Pin_12: EXTIx_IRQn = EXTI15_10_IRQn; break;
	case EXTI_Pin_13: EXTIx_IRQn = EXTI15_10_IRQn; break;
	case EXTI_Pin_14: EXTIx_IRQn = EXTI15_10_IRQn; break;
	case EXTI_Pin_15: EXTIx_IRQn = EXTI15_10_IRQn; break;
	default : break ;
	}

	NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn; //Enable the external interrupt channel where the button WK_UP is located
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preepri; //Preemption priority 2,
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpri; //Subpriority 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //Enable external interrupt channel
	NVIC_Init(&NVIC_InitStructure);
}




  /**
  * @brief interrupt enable or disable program
  * @code
  * // Initialize extiA, pin10, main priority preepri=2, response priority subpri=2, enable interrupt ITsta=ENABLE
  * EXTI_QuickInit(HW_EXTIA, EXTI_Pin_10, 2, 2);
  * @endcode
  * @param[in] instance module number
  * @arg HW_EXTIA A port
  * @arg HW_EXTIB B port
  * @arg HW_EXTIC C port
  * @arg HW_EXTID D port
  * @arg HW_EXTIE E port
  * @param[in] EXTI_Pin_x: x is the GPIO pin number, optional values 0,1,2....15
  * @param[in] ITsta enable/disable interrupt, optional values ENABLE, DISABLE
  */
void EXTI_IRQcmd(int instance, int EXTI_Pin_x,FunctionalState ITsta)
{
	int EXTIx_IRQn;
	NVIC_InitTypeDef NVIC_InitStructure;
	switch(EXTI_Pin_x)
	{
	case EXTI_Pin_0: EXTIx_IRQn = EXTI0_IRQn; break;
	case EXTI_Pin_1: EXTIx_IRQn = EXTI1_IRQn; break;
	case EXTI_Pin_2: EXTIx_IRQn = EXTI2_IRQn; break;
	case EXTI_Pin_3: EXTIx_IRQn = EXTI3_IRQn; break;
	case EXTI_Pin_4: EXTIx_IRQn = EXTI4_IRQn; break;
	case EXTI_Pin_5: EXTIx_IRQn = EXTI9_5_IRQn; break;
	case EXTI_Pin_6: EXTIx_IRQn = EXTI9_5_IRQn; break;
	case EXTI_Pin_7: EXTIx_IRQn = EXTI9_5_IRQn; break;
	case EXTI_Pin_8: EXTIx_IRQn = EXTI9_5_IRQn; break;
	case EXTI_Pin_9: EXTIx_IRQn = EXTI9_5_IRQn; break;
	case EXTI_Pin_10: EXTIx_IRQn = EXTI15_10_IRQn; break;
	case EXTI_Pin_11: EXTIx_IRQn = EXTI15_10_IRQn; break;
	case EXTI_Pin_12: EXTIx_IRQn = EXTI15_10_IRQn; break;
	case EXTI_Pin_13: EXTIx_IRQn = EXTI15_10_IRQn; break;
	case EXTI_Pin_14: EXTIx_IRQn = EXTI15_10_IRQn; break;
	case EXTI_Pin_15: EXTIx_IRQn = EXTI15_10_IRQn; break;
	default : break ;
	}
	NVIC_InitStructure.NVIC_IRQChannel = EXTIx_IRQn; //Enable the external interrupt channel where the button WK_UP is located
	NVIC_InitStructure.NVIC_IRQChannelCmd = ITsta; //Enable external interrupt channel
	NVIC_Init(&NVIC_InitStructure);
}




  /**
  * @brief interrupt callback function
  * @param[in] EXTI_Pin_x: x is the GPIO pin number, optional values 0,1,2....15
  * @param[in] AppCBFun function address
  */
void EXTI_CallbackInstall(int EXTI_Pin_x, GPIO_CallBackType AppCBFun)
{
     /* init module */
     if(AppCBFun != NULL)
     {
         GPIO_CallBackTable[EXTI_Pin_x] = AppCBFun;
     }
}
/**
  * @brief System interrupt function, users do not need to use it, they only need to change the registered function at the beginning of the file.
  */
void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
	  	if(GPIO_CallBackTable[0] !=NULL)GPIO_CallBackTable[0]();
		EXTI_ClearITPendingBit(EXTI_Line0);  
	}
}
 

void EXTI1_IRQHandler(void)
{	

	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		if(GPIO_CallBackTable[1] !=NULL) {GPIO_CallBackTable[1]();}
		EXTI_ClearITPendingBit(EXTI_Line1);  
	}
}

void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2) == SET)
	{
	  	if(GPIO_CallBackTable[2] !=NULL) GPIO_CallBackTable[2]();
		EXTI_ClearITPendingBit(EXTI_Line2);  
	}
}

void EXTI3_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
	 	if(GPIO_CallBackTable[3] !=NULL) GPIO_CallBackTable[3]();
		EXTI_ClearITPendingBit(EXTI_Line3);  
	}
}

void EXTI4_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line4) == SET)
	{
	  	if(GPIO_CallBackTable[4] !=NULL)GPIO_CallBackTable[4]();
		EXTI_ClearITPendingBit(EXTI_Line4); 
	}
}
 

void EXTI9_5_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line5) == SET)
	{
	  	if(GPIO_CallBackTable[5] !=NULL)GPIO_CallBackTable[5]();
		EXTI_ClearITPendingBit(EXTI_Line5);  
	}
	else if(EXTI_GetITStatus(EXTI_Line6) == SET)
	{
	  	if(GPIO_CallBackTable[6] !=NULL)GPIO_CallBackTable[6]();
		EXTI_ClearITPendingBit(EXTI_Line6); 
	}
	else if(EXTI_GetITStatus(EXTI_Line7) == SET)
	{
	  	if(GPIO_CallBackTable[7] !=NULL)GPIO_CallBackTable[7]();
		EXTI_ClearITPendingBit(EXTI_Line7);  
	}
	else if(EXTI_GetITStatus(EXTI_Line8) == SET)
	{
	  	if(GPIO_CallBackTable[8] !=NULL)GPIO_CallBackTable[8]();
		EXTI_ClearITPendingBit(EXTI_Line8);   
	}
	else if(EXTI_GetITStatus(EXTI_Line9) == SET)
	{
	  	if(GPIO_CallBackTable[9] !=NULL)GPIO_CallBackTable[9]();
		EXTI_ClearITPendingBit(EXTI_Line9);  
	}
}


void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10) == SET)
	{
	  	if(GPIO_CallBackTable[10] !=NULL)GPIO_CallBackTable[10]();
		EXTI_ClearITPendingBit(EXTI_Line10);   
	}
	else if(EXTI_GetITStatus(EXTI_Line11) == SET)
	{
	  	if(GPIO_CallBackTable[11] !=NULL)GPIO_CallBackTable[11]();
		EXTI_ClearITPendingBit(EXTI_Line11);  
	}
	else if(EXTI_GetITStatus(EXTI_Line12) == SET)
	{
	  	if(GPIO_CallBackTable[12] !=NULL)GPIO_CallBackTable[12]();
		EXTI_ClearITPendingBit(EXTI_Line12);   
	}
	else if(EXTI_GetITStatus(EXTI_Line13) == SET)
	{
	  	if(GPIO_CallBackTable[13] !=NULL)GPIO_CallBackTable[13]();
		EXTI_ClearITPendingBit(EXTI_Line13);   
	}
	else if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
	  	if(GPIO_CallBackTable[14] !=NULL)GPIO_CallBackTable[14]();
		EXTI_ClearITPendingBit(EXTI_Line14);  
	}
	else if(EXTI_GetITStatus(EXTI_Line15) == SET)
	{
	  	if(GPIO_CallBackTable[15] !=NULL)GPIO_CallBackTable[15]();
		EXTI_ClearITPendingBit(EXTI_Line15);  
	}
}
