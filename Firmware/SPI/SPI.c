#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "Main.h"
#include "Helpers.h"
#include "SPI.h"
#include "LCD.h"

inline uint8_t SPI_WRRD_Data8(char da)
{
	uint32_t Temp;
#ifdef SOFTSPI
  LCD_DATA;
  Temp = SoftSPI(da); 
#else
	SSIDataPut(SSI0_BASE, da);
//	while(SSIBusy(SSI0_BASE));//Make sure previous send finished
	SSIDataGet(SSI0_BASE, &Temp);
#endif
	return Temp;
}  

void SPI_RXFlush(void)
{
	uint32_t pui32DataRx;
	
	while(SSIDataGetNonBlocking(SSI0_BASE, &pui32DataRx));
}

inline void SPI_WR_Data8(char da)
{
#ifdef SOFTSPI
  LCD_DATA;
  SoftSPI(da); 
#else
	SSIDataPut(SSI0_BASE, da);
#endif
}  

inline void SPI_WR_Data(int da)
{
#ifdef SOFTSPI
  LCD_DATA;
  SoftSPI(da>>8);
  SoftSPI(da);
#else
//	while(SSIBusy(SSI0_BASE));//Make sure previous command finished
//  LCD_DATA;
  SSIDataPut(SSI0_BASE, da>>8);
//	while(SSIBusy(SSI0_BASE));//Make sure previous command finished
  SSIDataPut(SSI0_BASE, da);
#endif
}	  

inline void SPI_WR_Reg(char da)
{
#ifdef SOFTSPI
	  LCD_COMMAND;
	  LCD_CS_ACTIVE;
    SoftSPI(da);    
#else
	while(SSIBusy(SSI0_BASE));//Make sure previous command finished
	LCD_COMMAND;
	LCD_CS_ACTIVE;
  SSIDataPut(SSI0_BASE, da);
	while(SSIBusy(SSI0_BASE));//Wait until command is finished
	//Set data/command mode back to data to allow optimized data sequence
	LCD_DATA;
#endif
}

void SPI_SetSpeed(uint32_t Speed)
{
#ifdef SOFTSPI
#else	
  SSIDisable(SSI0_BASE);
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, Speed, 8);
  SSIEnable(SSI0_BASE);
	//Flush the receive buffer of garbage
	while(SSIDataGetNonBlocking(SSI0_BASE, (uint32_t*)&MiscBuffer[0])) {}
#endif
}

uint8_t SoftSPI(int da)
{
	uint8_t Temp = 0;
	//Clock for command/data
//	LCD_CLK_HIGH;LCD_CLK_LOW;
	if (((da >> 7)  & 1)== 1){LCD_DATA_HIGH;}  else{ LCD_DATA_LOW;}
	LCD_CLK_HIGH;
	Temp = (Temp << 1) | ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) >> 4) & 0x01);
	LCD_CLK_LOW;
	if (((da >> 6)  & 1)== 1){LCD_DATA_HIGH;}  else{ LCD_DATA_LOW;}
	LCD_CLK_HIGH;
	Temp = (Temp << 1) | ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) >> 4) & 0x01);
	LCD_CLK_LOW;
	if (((da >> 5)  & 1)== 1){LCD_DATA_HIGH;}  else{ LCD_DATA_LOW;}
	LCD_CLK_HIGH;
	Temp = (Temp << 1) | ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) >> 4) & 0x01);
	LCD_CLK_LOW;
	if (((da >> 4)  & 1)== 1){LCD_DATA_HIGH;}  else{ LCD_DATA_LOW;}
	LCD_CLK_HIGH;
	Temp = (Temp << 1) | ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) >> 4) & 0x01);
	LCD_CLK_LOW;
	if (((da >> 3)  & 1)== 1){LCD_DATA_HIGH;}  else{ LCD_DATA_LOW;}
	LCD_CLK_HIGH;
	Temp = (Temp << 1) | ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) >> 4) & 0x01);
	LCD_CLK_LOW;
	if (((da >> 2)  & 1)== 1){LCD_DATA_HIGH;}  else{ LCD_DATA_LOW;}
	LCD_CLK_HIGH;
	Temp = (Temp << 1) | ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) >> 4) & 0x01);
	LCD_CLK_LOW;
	if (((da >> 1)  & 1)== 1){LCD_DATA_HIGH;}  else{ LCD_DATA_LOW;}
	LCD_CLK_HIGH;
	Temp = (Temp << 1) | ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) >> 4) & 0x01);
	LCD_CLK_LOW;
	if (((da >> 0)  & 1)== 1){LCD_DATA_HIGH;}  else{ LCD_DATA_LOW;}
	LCD_CLK_HIGH;
	Temp = (Temp << 1) | ((GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4) >> 4) & 0x01);
	LCD_CLK_LOW;
	return Temp;
}
