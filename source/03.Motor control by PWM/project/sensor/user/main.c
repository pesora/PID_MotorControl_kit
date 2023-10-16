#include "sysinc.h"


void motor_init(){
	
	GPIO_QuickInit(HW_GPIOA, GPIO_Pin_4, GPIO_Mode_Out_PP);   //AIN1
	GPIO_QuickInit(HW_GPIOA, GPIO_Pin_5, GPIO_Mode_Out_PP);   //AIN2
	
	PAout(4) = 0; PAout(5) = 0;
	
	TIM_PWM_Init(TIM2, PWM_CH2, 10000, ENABLE);
	TIM_SetCompare(TIM2, PWM_CH2, 0);
}

/**************************************************************************
函数功能：绝对值函数
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}

void set_motor_pwm(int M_PWM){
	if(M_PWM > 0){
		PAout(4) = 1; PAout(5) = 0;
	}else{
		PAout(4) = 0; PAout(5) = 1;
	}
	TIM_SetCompare( TIM2, PWM_CH2, myabs(M_PWM) );
}

 int main(void)
 {
	NVIC_init();
	SYSTICK_DelayInit();	    	 //延时函数初始化	  	  	
	GPIO_QuickInit(HW_GPIOC, GPIO_Pin_13, GPIO_Mode_Out_PP);//初始化与LED连接的硬件接口
	UART_QuickInit(HW_UART1, 115200, 2, 2, ENABLE);
	
	motor_init();
	
  while(1)
	{		
			PCout(13)= !PCout(13);
			SYSTICK_DelayMs(2000);
			set_motor_pwm(5000);
			
			PCout(13)= !PCout(13);
			SYSTICK_DelayMs(2000);
			set_motor_pwm(0);
			
			PCout(13)= !PCout(13);
			SYSTICK_DelayMs(2000);
			set_motor_pwm(-5000);
			
			PCout(13)= !PCout(13);
			SYSTICK_DelayMs(2000);
			set_motor_pwm(0);
	}
}
 

