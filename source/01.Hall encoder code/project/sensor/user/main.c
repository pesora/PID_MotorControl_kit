#include "sysinc.h"


#define EXTITRIGGER_TYPE EXTI_Trigger_Rising

int k=0;

//Get the encoder value
void encoder_exti(void)
{
	if(PCin(15) == 1){
		k++;
	}else{
		k--;;
	}
	printf("%d \n", k);
}

int main(void)
{
	NVIC_init();
	SYSTICK_DelayInit(); //Delay function initialization
	GPIO_QuickInit(HW_GPIOC, GPIO_Pin_13, GPIO_Mode_Out_PP);//Initialize the hardware interface connected to the LED
	UART_QuickInit(HW_UART1, 115200, 2, 2, ENABLE);

	EXTI_QuickInit(HW_EXTIA, EXTI_Pin_0, 3, 3); //motor phase A
	EXTI_CallbackInstall(EXTI_Pin_0, encoder_exti);

	GPIO_QuickInit(HW_GPIOC, GPIO_Pin_15, GPIO_Mode_IPU); //motor phase B

	 while(1)
	{
		PCout(13) = !PCout(13);
		SYSTICK_DelayMs(500);
	}
}
