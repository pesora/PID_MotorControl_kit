/**
   *************************************************** *************************************************** *********
   * @file nrf2401.h
   * @author Zhang Wenchao
   * @qq 269426626
   * @version V1.0
   * @date 2016.09.03
   * @note This program is the underlying programming of nrf2401 in ARM 
   * (this program corrects the BUG circulating on the Internet that the mode cannot be switched without power failure)
   *************************************************** *************************************************** *********
   */

#ifndef __CH_LIB_NRF2401_H__
#define __CH_LIB_NRF2401_H__

#ifdef __cplusplus
  extern "C" {
#endif


#include "sysinc.h"

//24L01 operating line
#define SPIcs SPI2
#define Spics Spi2
#define NRF24L01_CE 	SPI2_CE 	//24L01 chip select signal
#define NRF24L01_CSN 	SPI2_NSS 	//SPI chip select signal
#define NRF24L01_IRQ 	PBin(6) 	//IRQ host data input

//NRF24L01 register operation command
#define READ_REG_NRF 	0x00 //Read the configuration register, the lower 5 bits are the register address
#define WRITE_REG_NRF 	0x20 //Write configuration register, the lower 5 bits are the register address
#define RD_RX_PLOAD 	0x61 //Read RX valid data, 1~32 bytes
#define WR_TX_PLOAD 	0xA0 //Write TX valid data, 1~32 bytes
#define FLUSH_TX 		0xE1 //Clear the TX FIFO register. Used in transmit mode
#define FLUSH_RX 		0xE2 //Clear the RX FIFO register. Used in receive mode
#define REUSE_TX_PL		0xE3 //Reuse the previous packet of data, CE is high, and data packets are sent continuously.
#define NOP 			0xFF //No operation, can be used to read the status register
//SPI(NRF24L01) register address
#define CONFIG 			0x00 //Configuration register address; bit0: 1 receiving mode, 0 transmitting mode; bit1: electrical selection; bit2: CRC mode; bit3: CRC enable;
                             //bit4: interrupt MAX_RT (interrupt when reaching the maximum number of retransmissions) is enabled; bit5: interrupt TX_DS is enabled; bit6: interrupt RX_DR is enabled
#define EN_AA 			0x01 //Enable automatic response function bit0~5, corresponding to channel 0~5
#define EN_RXADDR 		0x02 //Receive address allowed, bit0~5, corresponding to channel 0~5
#define SETUP_AW 		0x03 //Set address width (all data channels): bit1, 0:00, 3 bytes; 01, 4 bytes; 02, 5 bytes;
#define SETUP_RETR 		0x04 //Establish automatic retransmission; bit3:0, automatic retransmission counter; bit7:4, automatic retransmission delay 250*x+86us
#define RF_CH 			0x05 //RF channel, bit6:0, working channel frequency;
#define RF_SETUP 		0x06 //RF register; bit3: transmission rate (0:1Mbps, 1:2Mbps); bit2:1, transmit power; bit0: low noise amplifier gain
#define STATUS 			0x07 //Status register; bit0: TX FIFO full flag; bit3:1, receive data channel number (maximum: 6); bit4, reach the maximum number of retransmissions
                             //bit5: data sending completion interrupt; bit6: receiving data interrupt;
#define MAX_TX 			0x10 //Interrupt when reaching the maximum number of sending times
#define TX_OK 			0x20 //TX transmission completion interrupt
#define RX_OK 			0x40 //Data received interrupt

#define OBSERVE_TX 		0x08 //Send detection register, bit7:4, packet loss counter; bit3:0, retransmission counter
#define CD 				0x09 //Carrier detection register, bit0, carrier detection;
#define RX_ADDR_P0		 0x0A //Data channel 0 receiving address, maximum length 5 bytes, low byte first
#define RX_ADDR_P1 		0x0B //Data channel 1 receiving address, maximum length 5 bytes, low byte first
#define RX_ADDR_P2 		0x0C //Data channel 2 receiving address, the lowest byte can be set, the high byte must be equal to RX_ADDR_P1[39:8];
#define RX_ADDR_P3 		0x0D //Data channel 3 receiving address, the lowest byte can be set, the high byte must be equal to RX_ADDR_P1[39:8];
#define RX_ADDR_P4 		0x0E //Data channel 4 receiving address, the lowest byte can be set, the high byte must be equal to RX_ADDR_P1[39:8];
#define RX_ADDR_P5 		0x0F //Data channel 5 receiving address, the lowest byte can be set, the high byte must be equal to RX_ADDR_P1[39:8];
#define TX_ADDR 		0x10 //Sending address (low byte first), in ShockBurstTM mode, RX_ADDR_P0 is equal to this address
#define RX_PW_P0 		0x11 //Valid data width of receive data channel 0 (1~32 bytes), setting it to 0 is illegal
#define RX_PW_P1 		0x12 //Valid data width of receive data channel 1 (1~32 bytes), setting it to 0 is illegal
#define RX_PW_P2 		0x13 //Valid data width of receive data channel 2 (1~32 bytes), setting it to 0 is illegal
#define RX_PW_P3 		0x14 //Valid data width of receive data channel 3 (1~32 bytes), setting it to 0 is illegal
#define RX_PW_P4 		0x15 //effective data width of receive data channel 4 (1~32 bytes), setting it to 0 is illegal
#define RX_PW_P5 		0x16 //Valid data width of receive data channel 5 (1~32 bytes), setting it to 0 is illegal
#define NRF_FIFO_STATUS 0x17 //FIFO status register; bit0, RX FIFO register empty flag; bit1, RX FIFO full flag; bit2, 3, reserved
                             //bit4, TX FIFO empty flag; bit5, TX FIFO full flag; bit6, 1, send the previous data packet in a loop. 0, no loop;

//24L01 sending and receiving data width definition
#define TX_ADR_WIDTH 	5 	//5-byte address width
#define RX_ADR_WIDTH 	5 	//5-byte address width
#define TX_PLOAD_WIDTH 	32 	//32-byte user data width
#define RX_PLOAD_WIDTH 	32 	//32-byte user data width


//API
void NRF24L01_Init(int Spix);
void NRF24L01_ITinit(int instance, int EXTI_Pin_x,int preepri, int subpri);
void NRF24L01_RX_Mode(SPI_TypeDef *SPIx);
void NRF24L01_TX_Mode(SPI_TypeDef *SPIx);
u8 NRF24L01_Write_Buf(SPI_TypeDef *SPIx,u8 reg, u8 *pBuf, u8 len);
u8 NRF24L01_Read_Buf(SPI_TypeDef *SPIx,u8 reg, u8 *pBuf, u8 len);
u8 NRF24L01_Read_Reg(SPI_TypeDef *SPIx, u8 reg);
u8 NRF24L01_Write_Reg(SPI_TypeDef *SPIx, u8 reg, u8 value);
u8 NRF24L01_Check(SPI_TypeDef *SPIx);
u8 NRF24L01_TxPacket(SPI_TypeDef *SPIx, u8 *txbuf);
u8 NRF24L01_RxPacket(SPI_TypeDef *SPIx, u8 *rxbuf);

#endif
