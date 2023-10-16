/**
   *************************************************** ****************************
   * @file uart.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.8.22
   * @note This program is the underlying programming of uart in ARM (attached to the secondary encapsulation of the official underlying library)
   *************************************************** ****************************
   */

#include "uart.h"
#include "gpio.h"
#include "nvic.h"

static UART_CallBackType UART_CallBackTable[4] = {NULL};


int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}
   
     return ch;
}


int GetKey(void)
{

     while (!(USART1->SR & USART_FLAG_RXNE));

     return ((int)(USART1->DR & 0x1FF));
}


  /**
  * @brief serial port quick initialization program
  * @code
  * Initialize UART2: 9600-0-8-1, main priority preepri=2, response priority subpri=2, enable interrupt ITsta=ENABLE
  * UART_QuickInit(HW_UART2, 9600, 2, 2, ENABLE);
  * @endcode
  * @param[in] instance module number
  * @arg HW_UART1 1 port
  * @arg HW_UART2 2 port
  * @arg HW_UART3 3 port
  * @param[in] bound baud rate: 9600,115200....
  * @param[in] preepri preemption priority, default group2, optional values 0,1,2,3
  * @param[in] subpri response priority, default group2, optional values 0,1,2,3
  * @param[in] ITsta enable/disable interrupt, optional values ENABLE, DISABLE
  * @retval UART initialization result, 1 initialization is correct, 0 initialization failed
  */
uint8_t UART_QuickInit(int instance, int bound, int preepri, int subpri, FunctionalState ITsta)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	switch(instance)
	{
		case HW_UART1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE); //Enable USART1, GPIOA clock
			USART_DeInit(USART1); //Reset serial port 1

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //USART1_TX PA.9
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //Multiplex push-pull output
			GPIO_Init(GPIOA, &GPIO_InitStructure); //Initialize PA9

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART1_RX PA.10
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Floating input
			GPIO_Init(GPIOA, &GPIO_InitStructure); //Initialize PA10

			NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=preepri;//Preemption priority 3
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpri; //Subpriority 3
			NVIC_InitStructure.NVIC_IRQChannelCmd = ITsta; //IRQ channel enable
			NVIC_Init(&NVIC_InitStructure); //Initialize the VIC register according to the specified parameters

			USART_InitStructure.USART_BaudRate = bound; //Generally set to 9600;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b; //Word length is 8-bit data format
			USART_InitStructure.USART_StopBits = USART_StopBits_1; //A stop bit
			USART_InitStructure.USART_Parity = USART_Parity_No; //No parity bit
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//No hardware data flow control
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Transceiver mode

			USART_Init(USART1, &USART_InitStructure); //Initialize the serial port
			USART_ITConfig(USART1, USART_IT_RXNE, ITsta); //Enable interrupts
			USART_Cmd(USART1, ENABLE); //Enable serial port

		case HW_UART2:
			RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOA, ENABLE); //Enable USART1, GPIOA clock
			USART_DeInit(USART2); //Reset serial port 1

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //USART1_TX PA.9
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //Multiplex push-pull output
			GPIO_Init(GPIOA, &GPIO_InitStructure); //Initialize PA9

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //USART1_RX PA.10
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Floating input
			GPIO_Init(GPIOA, &GPIO_InitStructure); //Initialize PA10

			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=preepri;//Preemption priority 3
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpri; //Subpriority 3
			NVIC_InitStructure.NVIC_IRQChannelCmd = ITsta; //IRQ channel enable
			NVIC_Init(&NVIC_InitStructure); //Initialize the VIC register according to the specified parameters

			USART_InitStructure.USART_BaudRate = bound; //Generally set to 9600;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;//The word length is 8-bit data format
			USART_InitStructure.USART_StopBits = USART_StopBits_1; //A stop bit
			USART_InitStructure.USART_Parity = USART_Parity_No; //No parity bit
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//No hardware data flow control
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Transceiver mode

			USART_Init(USART2, &USART_InitStructure); //Initialize the serial port
			USART_ITConfig(USART2, USART_IT_RXNE, ITsta);
			//Enable interrupts
			USART_Cmd(USART2, ENABLE); //Enable serial port //Initialize according to set parameters
			break;
			
		case HW_UART3:
			RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3|RCC_APB2Periph_GPIOB, ENABLE); //Enable USART1, GPIOA clock
			USART_DeInit(USART3); //Reset serial port 1

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //USART1_TX PA.9
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //Multiplex push-pull output
			GPIO_Init(GPIOB, &GPIO_InitStructure); //Initialize PA9

			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //USART1_RX PA.10
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Floating input
			GPIO_Init(GPIOB, &GPIO_InitStructure); //Initialize PA10

			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=preepri;//Preemption priority 3
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = subpri; //Subpriority 3
			NVIC_InitStructure.NVIC_IRQChannelCmd = ITsta; //IRQ channel enable
			NVIC_Init(&NVIC_InitStructure); //Initialize the VIC register according to the specified parameters

			USART_InitStructure.USART_BaudRate = bound; //Generally set to 9600;
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;//The word length is 8-bit data format
			USART_InitStructure.USART_StopBits = USART_StopBits_1; //A stop bit
			USART_InitStructure.USART_Parity = USART_Parity_No; //No parity bit
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//No hardware data flow control
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //Transceiver mode

			USART_Init(USART3, &USART_InitStructure); //Initialize the serial port
			USART_ITConfig(USART1, USART_IT_RXNE, ITsta); //Enable interrupts
			USART_Cmd(USART3, ENABLE); //Enable serial port //Initialize according to set parameters
			break;
        
		default:
			return 0;
	}
	return 1;
}

/**
  * @brief Serial port reading byte program
  * @code
  * // Read UART2, unit16_t ch;
  * UART_ReadByte(HW_UART2, &ch);
  * @endcode
  * @param[in] instance module number
  * @arg HW_UART1 1 port
  * @arg HW_UART2 2 port
  * @arg HW_UART3 3 port
  * @retval UART reading result, 1 read correctly, 0 read failed
  */
uint8_t UART_ReadByte(uint32_t instance, uint16_t *ch)
{
switch(instance)
{
case HW_UART1:
*ch = (uint16_t)(USART1->DR & (uint16_t)0x01FF);
break;
case HW_UART2:
*ch = (uint16_t)(USART2->DR & (uint16_t)0x01FF);
break;
case HW_UART3:
*ch = (uint16_t)(USART3->DR & (uint16_t)0x01FF);
break;
default:
return 0;
}
return 1;
}

/**
  * @brief Serial port writing byte program
  * @code
  * //Write UART2, unit16_t ch=ox7a;
  * UART_WriteByte(HW_UART2, ch);
  * @endcode
  * @param[in] instance module number
  * @arg HW_UART1 1 port
  * @arg HW_UART2 2 port
  * @arg HW_UART3 3 port
  */
void UART_WriteByte(uint32_t instance, uint16_t ch)
{
switch(instance)
{
case HW_UART1:
USART1->DR = (ch & (uint16_t)0x01FF);
break;
case HW_UART2:
         USART2->DR = (ch & (uint16_t)0x01FF);
break;
case HW_UART3:
USART3->DR = (ch & (uint16_t)0x01FF);
break;
}
}


void UART_CallbackInstall(uint32_t instance, UART_CallBackType AppCBFun)
{
     /* init module */
     if(AppCBFun != NULL)
     {
         UART_CallBackTable[instance] = AppCBFun;
     }
}
/**
  * @brief The system receives the interrupt function. Users do not need to use it. They only need to change the registered function at the beginning of the file.
  */
void USART1_IRQHandler(void)
{
uint16_t Res;
if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
{
UART_ReadByte(HW_UART1, &Res);
if(UART_CallBackTable[HW_UART1] != NULL)UART_CallBackTable[HW_UART1](Res);
}
}


/**
  * @brief The system receives the interrupt function. Users do not need to use it. They only need to change the registered function at the beginning of the file.
  */
void USART2_IRQHandler(void)
{
uint16_t Res;
if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
{
UART_ReadByte(HW_UART2, &Res);
if(UART_CallBackTable[HW_UART2] != NULL)UART_CallBackTable[HW_UART2](Res);
}
}


/**
  * @brief The system receives the interrupt function. Users do not need to use it. They only need to change the registered function at the beginning of the file.
  */
void USART3_IRQHandler(void)
{
uint16_t Res;
if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
{
   UART_ReadByte(HW_UART3, &Res);
if(UART_CallBackTable[HW_UART3] != NULL)UART_CallBackTable[HW_UART3](Res);
}
}
