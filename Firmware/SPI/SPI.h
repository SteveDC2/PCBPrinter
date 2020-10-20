#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>

uint8_t SPI_WRRD_Data8(char da);
void SPI_WR_Data8(char da);
void SPI_WR_Data(int da);
void SPI_WR_Reg(char da);	 
void SPI_SetSpeed(uint32_t Speed);
uint8_t SoftSPI(int da);
void SPI_RXFlush(void);

#endif
