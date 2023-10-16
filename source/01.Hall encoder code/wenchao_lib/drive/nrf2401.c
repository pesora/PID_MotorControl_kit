/**
   *************************************************** ****************************
   * @file nrf2401.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.8.22
   * @note This program is the underlying driver programming of nrf2401 in ARM
   *************************************************** ****************************
   */

#include "sysinc.h"


  /*Sending and receiving addresses*/
const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};


  /**
  * @brief NRF2401 initialization function
  * @code
  * //Initialize Spi1
  * NRF24L01_Init(Spi1);
  * @endcode
  * @param[in] Spix: Spi module selection, available values are Spi1, Spi2, Spi3
  */
void NRF24L01_Init(int Spix)
{
   	SPI_QuickInit(Spix, CPOL_LOW_CPHA_1, SPI_BaudRatePrescaler_16, SPI_NSS_Soft);
	NRF24L01_CE=0; //Enable 24L01
	NRF24L01_CSN=1; //SPI chip select cancel
	SPI_SetSpeed(SPIcs,SPI_BaudRatePrescaler_8);
	EXTI_QuickInit(HW_EXTIB, EXTI_Pin_6, 3, 3);

	GPIO_QuickInit(HW_GPIOB, GPIO_Pin_6, GPIO_Mode_IPD); //Initialize the hardware interface connected to the LED
	GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_4);
	
	if(NRF24L01_Check(SPIcs)==1)
	{
		printf(" no_check\n");
	}
	else
		printf("  check_0k\n");
	NRF24L01_RX_Mode(SPIcs);
}



  /**
  * @brief NRF2401 interrupt function
  * @code
  * Initialize extiA, pin10, main priority preepri=2, response priority subpri=2, enable interrupt ITsta=ENABLE
  * NRF24L01_ITinit(HW_EXTIA, EXTI_Pin_10, 2, 2);
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
void NRF24L01_ITinit(int instance, int EXTI_Pin_x,int preepri, int subpri)
{
	EXTI_QuickInit(instance, EXTI_Pin_x, preepri, subpri);
}



  /**
  * @brief NRF2401 configured in receive mode
  * @code
  * Configure SPI1 as the receiving module of NRF2401
  * NRF24L01_RX_Mode(SPI1);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  */
void NRF24L01_RX_Mode(SPI_TypeDef *SPIx)
{
	NRF24L01_Write_Reg(SPIcs, WRITE_REG_NRF+CONFIG, 0x08); //Configure the parameters of the basic working mode; PWR_UP, EN_CRC, 16BIT_CRC, receiving mode
	EXTI_IRQcmd(HW_EXTIB, EXTI_Pin_6,ENABLE);

	NRF24L01_CE=0;
	NRF24L01_Write_Buf(SPIx, WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //Write RX node address

	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+EN_AA,0x01); //Enable automatic response of channel 0
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+EN_RXADDR,0x01); //Enable the receive address of channel 0
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+RF_CH,40); //Set the RF communication frequency
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+RX_PW_P0,RX_PLOAD_WIDTH); //Select the effective data width of channel 0
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+RF_SETUP,0x0f); //Set TX transmission parameters, 0db gain, 2Mbps, low noise gain on
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+CONFIG, 0x0f); //Configure the parameters of the basic working mode; PWR_UP, EN_CRC, 16BIT_CRC, receiving mode
	NRF24L01_CE = 1; //CE is high, enter receiving mode
}


  /**
  * @brief NRF2401 configured in transmit mode
  * @code
  * Configure SPI1 as the receiving module of NRF2401
  * NRF24L01_TX_Mode(SPI1);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  */
void NRF24L01_TX_Mode(SPI_TypeDef *SPIx)
{
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+CONFIG, 0x08); //Configure the parameters of the basic working mode; PWR_UP, EN_CRC, 16BIT_CRC, receiving mode
	EXTI_IRQcmd(HW_EXTIB, EXTI_Pin_6,DISABLE);

	NRF24L01_CE=0;
	NRF24L01_Write_Buf(SPIx, WRITE_REG_NRF+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH); //Write TX node address
	NRF24L01_Write_Buf(SPIx, WRITE_REG_NRF+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //Set the TX node address, mainly to enable ACK

	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+EN_AA,0x01); //Enable automatic response of channel 0
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+EN_RXADDR,0x01); //Enable the receive address of channel 0
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+SETUP_RETR,0x1a); //Set automatic retransmission interval: 500us + 86us; Maximum number of automatic retransmissions: 10 times
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+RF_CH,40); //Set the RF channel to 40
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+RF_SETUP,0x0f); //Set TX transmission parameters, 0db gain, 2Mbps, low noise gain on
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+CONFIG,0x0e); //Configure the parameters of the basic working mode; PWR_UP, EN_CRC, 16BIT_CRC, receive mode, enable all interrupts
	NRF24L01_CE=1;//CE is high, start sending after 10us
}


  /**
  * @brief NRF2401 write data
  * @code
  * Configure SPI1, write the register address to 0x01, the array a[12], the length is 12
  * NRF24L01_Write_Buf(SPI1,0x01, a, 12);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  * @param[in] reg: register address
  * @param[in] pBuf: data address
  * @param[in] len: data length
  * @return return value: read status value
  */
u8 NRF24L01_Write_Buf(SPI_TypeDef *SPIx,u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;
	
	NRF24L01_CSN = 0; //Enable SPI transmission
	status = SPI_ReadWriteByte(SPIx,reg); //Send the register value (position) and read the status value
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI_ReadWriteByte(SPIx,*pBuf++); //Write data
	NRF24L01_CSN = 1;
	
	return status;          																													//返回读到的状态值
}

  /**
  * @brief NRF2401 read data
  * @code
  * Configure SPI1, read register address is 0x01, array a[12], length is 12
  * NRF24L01_Read_Buf(SPI1,0x01, a, 12);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  * @param[in] reg: register address
  * @param[in] pBuf: data address
  * @param[in] len: data length
  * @return return value: read status value
  */
u8 NRF24L01_Read_Buf(SPI_TypeDef *SPIx,u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;
	NRF24L01_CSN = 0; //Enable SPI transmission
	status=SPI_ReadWriteByte(SPIx, reg); //Send the register value (position) and read the status value
	for(u8_ctr=0;u8_ctr<len;u8_ctr++) pBuf[u8_ctr]=SPI_ReadWriteByte(SPIx, 0XFF); //Read data
	NRF24L01_CSN=1; //Close SPI transmission
	return status; //Return the read status value
}


  /**
  * @brief NRF2401 read register
  * @code
  * Configure SPI1, read register address is 0x01
  * NRF24L01_Read_Reg(SPI1, 0x01);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  * @param[in] reg: register address
  * @return return value: read register value
  */
u8 NRF24L01_Read_Reg(SPI_TypeDef *SPIx, u8 reg)
{
	u8 reg_val;
	
	NRF24L01_CSN = 0; //Enable SPI transmission
	SPI_ReadWriteByte(SPIx, reg); //Send register number
	reg_val=SPI_ReadWriteByte(SPIx, 0XFF); //Read register contents
	NRF24L01_CSN = 1; //Disable SPI transmission
	
	return(reg_val);
}

  /**
  * @brief NRF2401 write register
  * @code
  * Configure SPI1 and write the register address to 0x01
  * NRF24L01_Write_Reg(SPI1, 0x01);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  * @param[in] reg: register address
  * @return return value: read status value
  */
u8 NRF24L01_Write_Reg(SPI_TypeDef *SPIx, u8 reg, u8 value)
{
	u8 status;
	
	NRF24L01_CSN=0; //Enable SPI transmission
	status =SPI_ReadWriteByte(SPIx, reg); //Send register number
	SPI_ReadWriteByte(SPIx, value); //Write the value of the register
	NRF24L01_CSN=1; //Disable SPI transmission
	
	return(status); //return status value
}

  /**
  * @brief NRF2401 check if available
  * @code
  * Configure SPI1
  * NRF24L01_Check(SPI1);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  * @return Return 0: Initialization is correct 1: Initialization error
  */
u8 NRF24L01_Check(SPI_TypeDef *SPIx)
	{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	
	u8	i;
	
	SPI_SetSpeed(SPIx, SPI_BaudRatePrescaler_8); //spi speed is 9Mhz (the maximum SPI clock of 24L01 is 10Mhz)
	NRF24L01_Write_Buf(SPIx, WRITE_REG_NRF+TX_ADDR,buf,5); //Write 5 bytes of address.
	NRF24L01_Read_Buf(SPIx, TX_ADDR,buf,5); //Read the written address
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;
	if(i!=5)return 1; //Detect 24L01 error
	
	return 0; //24L01 detected
}


  /**
  * @brief NRF2401 sends data packet
  * @code
  * Configure SPI1 and write data to adress[2];
  * NRF24L01_TxPacket(SPI1, adress);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  * @param[in] txbuf: data address
  * @return Return value: MAX_TX: Maximum number of retransmissions reached TX_OK: Transmission completed 0xff: Transmission failed due to other reasons
  */
u8 NRF24L01_TxPacket(SPI_TypeDef *SPIx, u8 *txbuf)
{
	NRF24L01_TX_Mode(SPIx);
	
	u8	sta;
	
	SPI_SetSpeed(SPIx, SPI_BaudRatePrescaler_8); //spi speed is 9Mhz (the maximum SPI clock of 24L01 is 10Mhz)
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(SPIx, WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH); //Write data to TX BUF 32 bytes
	NRF24L01_CE=1;//Start sending

	while(NRF24L01_IRQ != 0); //Wait for transmission to complete
	sta=NRF24L01_Read_Reg(SPIx, STATUS); //Read the value of the status register

	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+STATUS,sta); //Clear the TX_DS or MAX_RT interrupt flag
	NRF24L01_CE=0;
	if(sta&MAX_TX)
	{
		NRF24L01_Write_Reg(SPIx, FLUSH_TX,0xff); //Clear TX FIFO register
		NRF24L01_RX_Mode(SPIcs);
		
		return MAX_TX;
	}
	if(sta&TX_OK)
	{
		NRF24L01_RX_Mode(SPIcs);
		
		return TX_OK;
	}
	NRF24L01_RX_Mode(SPIcs);

	return 0xff;
}

  /**
  * @brief NRF2401 receives data packets
  * @code
  * Configure SPI1 and read data as adress[2];
  * NRF24L01_RxPacket(SPI1, adress);
  * @endcode
  * @param[in] SPIx: SPI module selection, optional values are SPI1, SPI2, SPI3
  * @param[in] txbuf: data address
  * @return Return value: 0: Data received 1: No data received
  */
u8 NRF24L01_RxPacket(SPI_TypeDef *SPIx, u8 *rxbuf)
{
	u8	sta;
	
	SPI_SetSpeed(SPIx, SPI_BaudRatePrescaler_8); //spi speed is 9Mhz (the maximum SPI clock of 24L01 is 10Mhz)
	sta=NRF24L01_Read_Reg(SPIx, STATUS); //Read the value of the status register
	NRF24L01_Write_Reg(SPIx, WRITE_REG_NRF+STATUS,sta); //Clear the TX_DS or MAX_RT interrupt flag
	if(sta&RX_OK) //Data received
	{
		NRF24L01_Read_Buf(SPIx, RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH); //Read data
		NRF24L01_Write_Reg(SPIx, FLUSH_RX,0xff); //Clear the RX FIFO register
		
		return 0;
	}
	return 1; //No data received
}