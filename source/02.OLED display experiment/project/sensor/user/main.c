#include "sysinc.h"

int i = 0;

void timer2()
{	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){  //检查TIM3更新中断发生与否
		  PAout(1)= !PAout(1);
		  PCout(13)= !PCout(13);
			OLED_P6x8Str_Num(0,2," @ADC1:",'V',i++);
	  	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志 
	}
}

int main(void){
	NVIC_init();
	SYSTICK_DelayInit();	    	 //延时函数初始化	  	  	
	 
	GPIO_QuickInit(HW_GPIOA, GPIO_Pin_1, GPIO_Mode_Out_PP);//初始化与LED连接的硬件接口
	GPIO_QuickInit(HW_GPIOC, GPIO_Pin_13, GPIO_Mode_Out_PP);//初始化与LED连接的硬件接口
	
	IIC_Init();
	OLED_Init();
	for(int i = 1; i<=4; i++){
			OLED_P16x16Ch(i*16+16,2,i);
	}
	SYSTICK_DelayMs(1800);
	OLED_CLS();
	OLED_P8x16Str(0,0," TMP:");
	OLED_P6x8Str_Num(0,2," @ADC1:",'V',i++);
  OLED_P6x8Str_Num(0,3," @ADC1:",'V',24);
	OLED_P8x16Str(0,4," I:"); OLED_P8x16Num(32, 4, -15.26, 2);
	OLED_P8x16Str(0,6," D:"); OLED_P8x16Num(32, 6, 123.78, 2);
	
	TIM_ms_Init(TIM2, 1000, TIM_IT_Update,2, ENABLE);	
 	TIMER_CallbackInstall(HW_TIMER2, timer2);	 
	
	while(1);
}
 

