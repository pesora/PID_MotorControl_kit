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

#ifndef __CH_LIB_IIC_H__
#define __CH_LIB_IIC_H__

#ifdef __cplusplus
  extern "C" {
#endif


#include "sysinc.h"

//IO direction setting
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO operation function
#define IIC_SCL PBout(10) //SCL
#define IIC_SDA PBout(11) //SDA
#define READ_SDA PBin(11) //Input SDA

//All operating functions of IIC
void IIC_Init(void); //Initialize the IO port of IIC
void IIC_Start(void); //Send IIC start signal
void IIC_Stop(void); //Send IIC stop signal
void IIC_Send_Byte(u8 txd); //IIC sends a byte
u8 IIC_Read_Byte(unsigned char ack);//IIC reads a byte
u8 IIC_Wait_Ack(void); //IIC waits for ACK signal
void IIC_Ack(void); //IIC sends ACK signal
void IIC_NAck(void); //IIC does not send ACK signal

#endif
