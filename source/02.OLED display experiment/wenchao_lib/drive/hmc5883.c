/**
   *************************************************** ****************************
   * @file hmc5883.c
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.10.1
   * @note This program is for application programming in ARM
   *************************************************** ****************************
   */
#include "sysinc.h"

  /**
  * @brief HMC5883 initialization
  */
void HMC5883_Init(void)
{
HMC5883_ByteWrite(HMC_REGB,0xE0);
HMC5883_ByteWrite(HMC_MODE,0x00);
}

  /**
  * @brief HMC5883 register write data
  * @param[in] REG_Address register address value: refer to the register address in the header file
  * @param[in] REG_data data to be written
  */
void HMC5883_ByteWrite(uint8_t REG_Address,uint8_t REG_data)
{
	IIC_Start();
	IIC_Send_Byte(HMC_SlaveAddress);
	IIC_Wait_Ack();

	IIC_Send_Byte(REG_Address);
	IIC_Wait_Ack();

	IIC_Send_Byte(REG_data);
	IIC_Wait_Ack();

	IIC_Stop();
}

  /**
  * @brief HMC5883 single register read data
  * @param[in] REG_Address register address value: refer to the register address in the header file
  * @return read data
  */
u8 HMC5883_ByteRead_Single(uint8_t REG_Address)
{
	u8 Date;
	IIC_Start();

	IIC_Send_Byte(HMC_SlaveAddress);
	IIC_Wait_Ack();

	IIC_Send_Byte(REG_Address);
	IIC_Wait_Ack();

	IIC_Start();
	IIC_Send_Byte(HMC_SlaveAddress+1);
	IIC_Wait_Ack();
	Date = IIC_Read_Byte(0);
	IIC_Stop();
	
	return Date;
}

  /**
  * @brief HMC5883 reads all data
  * @param[in] p: Data structure HMC_DATE pointer
  */
void HMC5883_GetDate(HMC_DATE *p)
{
	u16 Date[6];
	
	Date[0] = HMC5883_ByteRead_Single(HMC_XOUT_H);
	Date[1] = HMC5883_ByteRead_Single(HMC_XOUT_L);
	Date[2] = HMC5883_ByteRead_Single(HMC_ZOUT_H);
	Date[3] = HMC5883_ByteRead_Single(HMC_ZOUT_L);
	Date[4] = HMC5883_ByteRead_Single(HMC_YOUT_H);
	Date[5] = HMC5883_ByteRead_Single(HMC_YOUT_L);
	p->hmc_date_x = (Date[0]<<8) | (Date[1]&0x00ff);
	p->hmc_date_x = p->hmc_date_x - p->hmc_offset_x;

	p->hmc_date_z = (Date[2]<<8) | (Date[3]&0x00ff);
	   p->hmc_date_z = p->hmc_date_z - p->hmc_offset_z;

	p->hmc_date_y = (Date[4]<<8) | (Date[5]&0x00ff);
	p->hmc_date_y = p->hmc_date_y - p->hmc_offset_y;
}

  /**
  * @brief HMC5883 read deviation
  * @param[in] p: Data structure HMC_DATE pointer
  */
void HMC5883_offset(HMC_DATE *p)
{
	short tep_xh,tep_xl,tep_yh,tep_yl,tep_zh,tep_zl,i;

	tep_xh = 0;
	tep_xl = 0;
	tep_yh = 0;
	tep_yl = 0;
	tep_zh = 0;
	tep_zl = 0;

	for( i = 0; i<500; i++)
	{
		HMC5883_GetDate(p);
		if(p->hmc_date_x > tep_xh){ tep_xh = p->hmc_date_x;}
		if(p->hmc_date_x < tep_xl){ tep_xl = p->hmc_date_x;}
		if(p->hmc_date_y > tep_yh){ tep_yh = p->hmc_date_y;}
		if(p->hmc_date_y < tep_yl){ tep_yl = p->hmc_date_y;}
		if(p->hmc_date_z > tep_zh){ tep_zh = p->hmc_date_z;}
		if(p->hmc_date_z < tep_zl){ tep_zl = p->hmc_date_z;}

		SYSTICK_DelayMs(10);
	}
	p->hmc_offset_x = (tep_xh + tep_xl)/2;
	p->hmc_offset_y = (tep_yh + tep_yl)/2;
	p->hmc_offset_z = (tep_zh + tep_zl)/2;
}


  /**
  * @brief HMC5883 gets the declination angle when placed horizontally
  * @param[in] p: Data structure HMC_DATE pointer
  */
void HMC5883_GetLevel_Angle(HMC_DATE *p)
{
	HMC5883_GetDate(p);
	if(p->hmc_date_x == 0)
	{
		if(p->hmc_date_y > 0){p->hmc_level_angle = 0; }
		else if(p->hmc_date_y < 0){p->hmc_level_angle = -180; }
	}
	else if(p->hmc_date_x < 0)
	{
		p->hmc_level_angle =T_ANGLE*atan(((float)p->hmc_date_y)/((float)p->hmc_date_x)) + 90;
	}
	else
	{
		p->hmc_level_angle =T_ANGLE*atan(((float)p->hmc_date_y)/((float)p->hmc_date_x)) - 90;
	}
}