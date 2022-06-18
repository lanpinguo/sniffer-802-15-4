//////////////////////////////////////////////////////////////////////////////////	 
//Copyright(C) PureMicro
//All rights reserved
/**************************************************************************************************/		
 /* @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/	
#include "sys.h"

#ifndef _SPI_H_
#define _SPI_H_

#ifdef GD32F40_41xxx

u8 SPI_WriteByte(uint32_t SPIx,u8 Byte);
u16 SPI_WriteWord(uint32_t SPIx, u16 val);
void SPI1_Init(void);
void SPI2_Init(void);
void SPI_SetSpeed(uint32_t SPIx,u8 SpeedSet);
u8 SPI_ReadByte(uint32_t SPIx);
u8 SPI_WriteByte(uint32_t SPIx,u8 Byte);

#else
 
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);
u16 SPI_WriteWord(SPI_TypeDef* SPIx, u16 val);
void SPI1_Init(void);
void SPI2_Init(void);
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet);
u8 SPI_ReadByte(SPI_TypeDef* SPIx);

#endif



#endif
