/**
   *************************************************** ****************************
   * @file i2c.h
   * @author Zhang Wenchao
* @qq 269426626
   * @version V1.0
   * @date 2016.8.21
   * @note This program is the underlying programming of IIC in ARM
   *************************************************** ****************************
   */

#include "sysinc.h"


  /**
  * @brief I2C initialization
  */
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Push-pull output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); //PB10, PB11 output high
}

  /**
  * @brief generates IIC start signal
  */
void IIC_Start(void)
{
	SDA_OUT(); //sda line output
	IIC_SDA=1;
	IIC_SCL=1;
	SYSTICK_DelayUs(4);
	  IIC_SDA=0; //START:when CLK is high,DATA change form high to low
	SYSTICK_DelayUs(4);
	IIC_SCL=0; //Clamp the I2C bus and prepare to send or receive data
}

/**
  * @brief generates IIC stop signal
  */
void IIC_Stop(void)
{
	SDA_OUT(); //sda line output
	IIC_SCL=0;
	IIC_SDA=0; //STOP:when CLK is high DATA change form low to high
	SYSTICK_DelayUs(4);
	IIC_SCL=1;
	IIC_SDA=1; //Send I2C bus end signal
	SYSTICK_DelayUs(4);
}

/**
  * @brief Waiting for the response signal to arrive
  * @return Return value: 1, failure to receive response 0, success in receiving response
  */
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	
	SDA_IN(); //SDA is set as input
	IIC_SDA=1;
	SYSTICK_DelayUs(1);
	IIC_SCL=1;
	SYSTICK_DelayUs(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0; //clock output 0
	
	return 0;
}

/**
  * @brief Generate ACK response
  */
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	SYSTICK_DelayUs(2);
	IIC_SCL=1;
	SYSTICK_DelayUs(2);
	IIC_SCL=0;
}

/**
  * @brief does not generate ACK response
  */
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	SYSTICK_DelayUs(2);
	IIC_SCL=1;
	SYSTICK_DelayUs(2);
	IIC_SCL=0;
}

/**
  * @brief IIC sends a byte
  * @param[in] txd: the data to be sent
  * @date: Returns whether the slave has responded 1, if there is a response 0, if there is no response
  */
void IIC_Send_Byte(u8 txd)
{
    u8	t;
	
	SDA_OUT();
	IIC_SCL=0; //Pull the clock low to start data transmission
	for(t=0;t<8;t++)
	{
		//IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
			
		txd<<=1;
		SYSTICK_DelayUs(2);
		IIC_SCL=1;
		SYSTICK_DelayUs(2);
		IIC_SCL=0;
		SYSTICK_DelayUs(2);
	}
}

/**
  * @brief IIC reads a byte
  * @param[in] ack When ack=1, send ACK, ack=0, send nACK
  */
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	
	SDA_IN(); //SDA is set as input
	for(i=0;i<8;i++)
	{
		IIC_SCL=0;
		SYSTICK_DelayUs(2);
		IIC_SCL=1;
		receive<<=1;
		if(READ_SDA)receive++;
		SYSTICK_DelayUs(1);
	}
	if (!ack)
		IIC_NAck(); //Send nACK
	else
		IIC_Ack(); //Send ACK
	
	return receive;
}