/**
   *************************************************** ****************************
   * @file spi.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.09.03
   * @note This program is the underlying programming of spi in ARM (attached to the secondary encapsulation of the official underlying library)
   *************************************************** ****************************
   */

#include "sysinc.h"




  /**
  * @brief SPI fast initialization program
  * @code
  * // Initialize Spi1, the idle level is high, the second edge is low acquisition signal, the baud rate prescaler value is 64, and the NSS signal is managed by software
  * SPI_QuickInit(Spi1, CPOL_HIGH_CPHA_0, SPI_BaudRatePrescaler_64, SPI_NSS_Soft);
  * @endcode
  * @param[in] frameFormat module number
  * @arg CPOL_HIGH_CPHA_1 polarity high, phase low
  * @arg CPOL_HIGH_CPHA_2 high polarity, high phase
  * @arg CPOL_LOW_CPHA_1 polarity low, phase low
  * @arg CPOL_LOW_CPHA_2 polarity low, phase high
  * @param[in] SPI_BaudRatePrescaler_x: baud rate prescaler value, the optional values of x are 2,4,8,16,32....256
  * @param[in] SPI_NSS_mode: NSS signal management mode
  * @arg SPI_NSS_Soft software management
  * @arg SPI_NSS_Hard hardware management
  * @return Return 0: initialization error 1: initialization correct
  */
uint32_t SPI_QuickInit(uint32_t Spix, int frameFormat, uint32_t SPI_BaudRatePrescaler_x, int SPI_NSS_mode)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //Set the SPI one-way or two-way data mode: SPI is set to two-line, two-way full-duplex
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //Set SPI working mode: set to master SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //Set the data size of SPI: SPI sends and receives 8-bit frame structure
	SPI_InitStructure.SPI_NSS = SPI_NSS_mode; //Whether the NSS signal is managed by hardware (NSS pin) or software (using SSI bit): the internal NSS signal is controlled by the SSI bit
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_x; //Define the baud rate prescaler value: baud rate prescaler value
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //Specify whether the data transmission starts from the MSB bit or the LSB bit: the data transmission starts from the MSB bit
	SPI_InitStructure.SPI_CRCPolynomial = 7; //Polynomial for CRC value calculation

	switch(frameFormat)
	{
	case CPOL_HIGH_CPHA_1:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		break;
	case CPOL_HIGH_CPHA_2:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		break;
	case CPOL_LOW_CPHA_1:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
		break;
	case CPOL_LOW_CPHA_2:
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
		break;
	default : break ;
	}

	switch(Spix)
	{
	case Spi1:
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE );
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE );
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_3 | GPIO_Pin_7 | GPIO_Pin_13);

		SPI_Init(SPI1, &SPI_InitStructure);
		SPI_Cmd(SPI1, ENABLE);
		break;
	
	case Spi2:
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2, ENABLE );
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_4;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

		GPIO_SetBits(GPIOB,GPIO_Pin_4 | GPIO_Pin_12| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);

		SPI_Init(SPI2, &SPI_InitStructure);
		SPI_Cmd(SPI2, ENABLE);
		break;
		
	case Spi3:
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );
		RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3 , ENABLE );
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOB,GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
		GPIO_SetBits(GPIOA,GPIO_Pin_14 | GPIO_Pin_15);

		SPI_Init(SPI3, &SPI_InitStructure);
		SPI_Cmd(SPI3, ENABLE);
		SPI_ReadWriteByte(SPI3, 0xff); //Start transmission
		break;
		
	default :
		return 0;
	}
	return 1;
}

/**
  * @brief SPI set transmission speed
  * @code
  * // Initialize SPI1, the baud rate prescaler value is 64
  * SPI_SetSpeed(SPI1,SPI_BaudRatePrescaler_64);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  * @param[in] SPI_BaudRatePrescaler: baud rate prescaler value, the optional value of x is 2,4,8,16,32...256
  */
void SPI_SetSpeed(SPI_TypeDef *SPIx,u8 SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPIx->CR1&=0XFFC7;
	SPIx->CR1|=SPI_BaudRatePrescaler;
	SPI_Cmd(SPIx,ENABLE);
}


  /**
  * @brief SPI reads and writes one byte
  * @code
  * // Initialize SPI1, write a=0x13;
  * SPI_ReadWriteByte(SPI1,a);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  * @param[in] TxData: Bytes to be written
  * @return return value: read bytes
  */
u8 SPI_ReadWriteByte(SPI_TypeDef *SPIx,u8 TxData)
{
	u8 retry=0;
	
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //Check whether the specified SPI flag bit is set: send buffer empty flag bit
	{
		retry++;
		if(retry>200)return 0;
	}
	SPI_I2S_SendData(SPIx, TxData); //Send a data through the peripheral SPIx
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET)//Check whether the specified SPI flag bit is set: accept the cache non-empty flag bit
	{
		retry++;
		if(retry>200)return 0;
	}
	return SPI_I2S_ReceiveData(SPIx); //Return the most recently received data via SPIx
}
