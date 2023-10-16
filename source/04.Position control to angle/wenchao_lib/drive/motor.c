#include "motor.h" 


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
