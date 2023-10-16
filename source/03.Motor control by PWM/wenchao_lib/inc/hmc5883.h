/**
   *************************************************** ****************************
   * @file hmc5883.h
   * @author Zhang Wenchao
* @qq 269426626
   * @version V1.0
   * @date 2016.10.1
   * @note This program is for application programming in ARM
   *************************************************** ****************************
   */
#ifndef __CH_HMC5883_H__
#define __CH_HMC5883_H__

#ifdef __cplusplus
  extern "C" {
#endif


typedef struct
{
     short hmc_date_x;
     short hmc_date_z;
     short hmc_date_y;

	short hmc_offset_x;
	short hmc_offset_y;
	short hmc_offset_z;

	float hmc_level_lastangle;
	float hmc_level_angle; //North is zero degrees, east of north is positive, and west is negative. In addition: this angle is the horizontal placement angle

	float hmc_nlevel_x;
	float hmc_nlevel_y;
	float hmc_nlevel_angle;
} HMC_DATE;

#include "sysinc.h"


#define HMC_SlaveAddress 0x3c

/*HMC5883 register address*/
#define HMC_REGA 0X00 	//Configuration register A: default value 0x10, normal measurement mode, no need to change it
#define HMC_REGB 0X01 	//Configuration register B: Main configuration gain, no need to change it
#define HMC_MODE 0X02 	//Mode register: generally needs to be manually configured to 0X00, continuous measurement mode
#define HMC_XOUT_H 0x03 //All data must be read before the data will be updated
#define HMC_XOUT_L 0x04
#define HMC_ZOUT_H 0x05
#define HMC_ZOUT_L 0x06
#define HMC_YOUT_H 0x07
#define HMC_YOUT_L 0x08
#define HMC_STATUS 0x09 //Status register: 7~2 bits: reserve 1 bit: data latch bit, cleared when all six data are read, otherwise the latch is set
#define HMC_IDENA 0x0A	 //0 bit: data ready bit, set when all data is updated and ready
#define HMC_IDENB 0x0B
#define HMC_IDENC 0x0C

//API
void HMC5883_Init(void);
void HMC5883_offset(HMC_DATE *p);
void HMC5883_ByteWrite(uint8_t REG_Address,uint8_t REG_data);
u8 HMC5883_ByteRead_Single(uint8_t REG_Address);
void HMC5883_GetDate(HMC_DATE *p);
void HMC5883_GetLevel_Angle(HMC_DATE *p);

#endif
