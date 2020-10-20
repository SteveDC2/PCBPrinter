#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/rom.h"
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"
#include "Main.h"
#include "LCD.h"
#include "TSC.h"
#include "Helpers.h"
#include "SPI.h"

uint16_t TSC_X_RAW;
uint16_t TSC_Y_RAW;
uint16_t TSC_X;
uint16_t TSC_Y;
uint16_t TSCXOffset = 0x39; 
uint16_t TSCYOffset = 0x4c; 
float TSCXScale = 0.349999;
float TSCYScale = 0.260756;	

void TSC_WaitTouched()
{
	while(TOUCH_PRESSED) WaitFor(10);//Wait until screen not touched with some debounce time
	while(!TSC_UpdateCoordinates()) WaitFor(10);//Wait until screen touched with some debounce time
}

void TSC_WaitReleased()
{
	while(TSC_UpdateCoordinates()) WaitFor(10);
}

void TSC_WaitTouchedReleased()
{
	TSC_WaitTouched();
	TSC_WaitReleased();
}

uint16_t TSC_Read(uint8_t Channel)
{
	uint8_t Command;
	uint32_t Temp1;
	uint32_t Temp2;
	uint8_t Mode = 0;//0 = 12 bit, 1 = 8 bit
	uint8_t SingleDiff = 0;
	uint8_t PowerMode = 0;//Normal mode, interrupt is enabled
	
	Command = 0x80 | (Channel << 4) | (Mode << 3) | (SingleDiff << 2) | PowerMode;
	TSC_CS_ACTIVE;
	WaitFor(2);
	SPI_WR_Data8(Command);
#ifdef SOFTSPI	
	//Soft SPI does not push a value for the write phase of the SPI transaction
	Temp1 = SPI_WRRD_DATA8(0);
	Temp2 = SPI_WRRD_DATA8(0);
#else
	Temp1 = SPI_WRRD_Data8(0);//Ignore first returned value from hardware SPI
	Temp1 = SPI_WRRD_Data8(0);
	SSIDataGet(SSI0_BASE, &Temp2);
#endif
	//Not needed since the data get is blocking	while(SSIBusy(SSI0_BASE));//Wait until command is finished
	TSC_CS_INACTIVE;
	WaitFor(2);
	return ((Temp2 >> 3) | (Temp1 << 3));
}

uint8_t TSC_UpdateIfTouchedRaw()//Return 1 for touched, 0 for untouched
{
	uint16_t XSum;
	uint16_t YSum;
	uint8_t i;
	uint8_t Valid;
	int16_t X[8];
	int16_t Y[8];

	Valid = 0;
	XSum = 0;
	YSum = 0;
  //Only update if touched at start of sequence	
	if (TOUCH_PRESSED)
	{
		//Set the SPI slow for TSC
		SPI_SetSpeed(TSC_SPISpeed);
		Valid = 1;
		for (i = 0; i < 8; i++)
		{
			//Only note samples with touch asserted
			if (TOUCH_PRESSED)
			{
				X[i] = TSC_Read(1);
				Y[i] = TSC_Read(5);
				XSum += X[i];
				YSum += Y[i];
				if (!TOUCH_PRESSED)//Make sure still pressed, i.e. is the sample still valid? No, skip out
				{
					Valid = 0;
					i = 100;
				}
			}
			else
			{
				//Touch not active for one of the samples so skip all samples
				Valid = 0;
				i = 100;
			}
		}
	}
	//All valid samples? Yes so filter
	if (Valid == 1)
	{
		//For the time being just use the simple average
		TSC_X_RAW = 1023 - (XSum >> 3);
		TSC_Y_RAW = 1023 - (YSum >> 3);
		TSC_X = TSC_X_RAW;
		TSC_Y = TSC_Y_RAW;
	}
	//Set SPI back fast for LCD
	SPI_SetSpeed(40000000);
	return Valid;
}

uint8_t TSC_UpdateCoordinates()
{
//	//Set the SPI slow for TSC
//	SPISetSpeed(2500000);//2.5 MHz for TSC 
//	TSC_X = LCD_W - (LCD_W * ((TSC_Read(1) + TSC_Read(1) + TSC_Read(1) + TSC_Read(1))) >> 12);
//	TSC_Y = LCD_H - (LCD_H * ((TSC_Read(5) + TSC_Read(5) + TSC_Read(5) + TSC_Read(5))) >> 12);
	if (TSC_UpdateIfTouchedRaw())
	{
		TSC_X = (TSC_X_RAW - TSCXOffset) * TSCXScale;
		TSC_Y = (TSC_Y_RAW - TSCYOffset) * TSCYScale;
		return 1;
	}
	else
	{
		return 0;
	}
//	//Set SPI back fast for LCD
//	SPISetSpeed(40000000);
}

void TSC_Calibrate()
{
	uint8_t Done = 0;
	uint16_t XTL;
	uint16_t XTR;
	uint16_t XBL;
	uint16_t XBR;
	uint16_t YTL;
	uint16_t YTR;
	uint16_t YBL;
	uint16_t YBR;
	uint16_t Left;
	uint16_t Right;
	uint16_t Top;
	uint16_t Bottom;
	uint16_t DeltaX;
	uint16_t DeltaY;
	
	BACK_COLOR = BLACK;
	LCD_Clear(BLACK);
	LCD_DrawCross(20, 20, WHITE);
	LCD_DrawCross(20, 220, WHITE);
	LCD_DrawCross(300, 20, WHITE);
	LCD_DrawCross(300, 220, WHITE);
	LCD_SetFont(1);
	LCD_FontString(50,100,(uint8_t*)"Touch each cross", 0);
	
	TSC_WaitReleased();
	
	while((Done != 0xf) || (TOUCH_PRESSED))//Wait until all 4 corners done and touch released on last corner
	{
		//TSC_WaitTouched();
		if (TSC_UpdateIfTouchedRaw())
		{
			LCD_ShowNum(50,200,TSC_X_RAW,4);
			LCD_ShowNum(50,220,TSC_Y_RAW,4);
			if ((TSC_X_RAW < 512) && (TSC_Y_RAW < 512))
			{
				//Top left
				Done |= 1;
				XTL = TSC_X_RAW;
				YTL = TSC_Y_RAW;
				LCD_DrawCross(20, 20, GREEN);
			}
			else if ((TSC_X_RAW >=  512) && (TSC_Y_RAW < 512))
			{
				//Top right
				Done |= 2;
				XTR = TSC_X_RAW;
				YTR = TSC_Y_RAW;
				LCD_DrawCross(300, 20, GREEN);
			}
			else if ((TSC_X_RAW < 512) && (TSC_Y_RAW >= 512))
			{
				//Bottom left
				Done |= 4;
				XBL = TSC_X_RAW;
				YBL = TSC_Y_RAW;
				LCD_DrawCross(20, 220, GREEN);
			}
			else if ((TSC_X_RAW >= 512) && (TSC_Y_RAW >= 512))
			{
				//Bottom right
				Done |= 8;
				XBR = TSC_X_RAW;
				YBR = TSC_Y_RAW;
				LCD_DrawCross(300, 220, GREEN);
			}
		}
	}
	//Now average top, bottom, left and right measurements
	Left = (XTL + XBL) >> 1;
	Right = (XTR + XBR) >> 1;
	Top = (YTL + YTR) >> 1;
	Bottom = (YBL + YBR) >> 1;
	//Calculate offsets and scale factor
	DeltaX = Right - Left;
	DeltaY = Bottom - Top;
	TSCXScale = (float)280.0 / (float)DeltaX;
	TSCYScale = (float)200.0 / (float)DeltaY;
	TSCXOffset = 20 / TSCXScale;
	TSCYOffset = 20 / TSCYScale;
	
//	while(GPIOPinRead(GPIO_PORTC_BASE, SW_C) != 0)
	{
		if (TSC_UpdateCoordinates())
		{
			LCD_DrawPoint(TSC_X, TSC_Y);
			LCD_ShowNum(50,200,TSC_X,4);
			LCD_ShowNum(50,220,TSC_Y,4);
		}
	}
}



