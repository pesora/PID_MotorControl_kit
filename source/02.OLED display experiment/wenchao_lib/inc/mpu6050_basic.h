/**
   *************************************************** ****************************
   * @file mpu6050_basic.h
   * @author Zhang Wenchao
* @qq 269426626
   * @version V1.0
   * @date 2016.9.11
   * @note This program is the underlying programming of ARM
   *************************************************** ****************************
   */
#ifndef __CH_LIB_MPU6050_BASIC_H__
#define __CH_LIB_MPU6050_BASIC_H__

#ifdef __cplusplus
  extern "C" {
#endif


typedef struct
{
     short acc[3]; //x,y,z
     short gyr[3];
     short tmp;


     short acc_offset[3]; //zero offset
     short gyr_offset[3];
     short tmp_offset;

     float acc_gain[3]; //proportion
     float gyr_gain[3];
     float tmp_gain;

	float angle_acc_x;
	float angle_acc_y;
	float angle_acc_z;

	float w_gyr_x;
	float w_gyr_y;
	float w_gyr_z;
} ACGY;

//typedef struct ACGY MPU_DATE;


#include "sysinc.h"



//register
#define SMPLRT_DIV 0x19 	//Gyro sampling rate, typical value: 0x07 (125Hz)
#define CONFIG_ 0x1A 		//Low pass filter frequency, typical value: 0x06 (5Hz)
#define GYRO_CONFIG 0x1B 	//Gyroscope self-test and measurement range, typical value: 0x18 (no self-test, 2000deg/s)
#define ACCEL_CONFIG 0x1C	//Accelerometer self-test, measurement range and high-pass filter frequency, typical value: 0x01 (no self-test, 2G, 5Hz)
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define PWR_MGMT_1 0x6B 	//Power management, typical value: 0x00 (normally enabled)
#define WHO_AM_I 0x75 		//IIC address register (default value 0x68, read-only)
#define SlaveAddress 0xD0 	//Address byte data when IIC is written


//API
u8 MPU6050_Check(void);
void MPU6050_ByteWrite(uint8_t REG_Address,uint8_t REG_data);
uint8_t MPU6050_ByteRead(uint8_t REG_Address);

void MPU6050_Init(void);

void MPU6050_Offset(ACGY *ag);
void MPU6050_GetData(ACGY *ag);
void MPU6050_Acc_Angle(ACGY *ag);
void MPU6050_Gyr_W(ACGY *ag);

#endif
