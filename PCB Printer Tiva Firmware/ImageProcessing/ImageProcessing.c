#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "usblib/usblib.h"
#include "usblib/usbcdc.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdcdc.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "usb_serial_structs.h"
#include "inc/hw_uart.h"
#include "inc/hw_memmap.h"
#include "driverlib/uart.h"
#include "driverlib/gpio.h"
#include "main.h"
#include "Helpers.h"
#include "CommandProcessor.h"
#include "USBSerial.h"
#include "Stepper.h"
#include "ImageProcessing.h"

int32_t BMPWidth;
int32_t BMPHeight;
uint16_t BMPBPP;
int8_t BMPFlip;
uint8_t ImagePolarity;
uint8_t TestMode = 1;
uint16_t YPrintDPI;
uint8_t TestBacklashCorrection = 0;
uint8_t MoveWhenFinished = 0;

uint16_t WidthInSteps;
int16_t ImageIndexDirection = 0;
uint16_t ImageIndex;
uint8_t MonoIndex = 0;
uint8_t UniDirection = 0;
uint8_t LaserDisable = 0;
uint8_t PrintingActive = 0;
float ImageFlipOffset = 0.0;//Distance in inches
int32_t BackLashCorrection = 0;
uint8_t MirrorEnable = 0;
uint8_t FlipEnable = 0;
uint8_t HomeBeforePrint = 1;
uint8_t LCDLineBuffer[320 * 2];
uint32_t TotalBytesRead = 0;
uint32_t TotalBMPImageSize = 0;

float XLocator = 0.4;//Location of PCB start. Should really be at least ?? to allow acceleration. Should also align with alignment bars

uint16_t PrintBufferWidth;

void ImageProc_GetBMPHeader(void)
{
	uint32_t BMPFileSize;
	uint16_t BMPUnused;
	uint16_t BMPPlanes;
	uint32_t BMPDataOffset;
	uint32_t BMPHeaderSize;
	uint32_t BMPCompression;
	uint32_t BMPImageSize;
	uint32_t BMPHResolution;
	uint32_t BMPVResolution;
	uint32_t BMPPalette;
	uint32_t BMPImportant;

	uint32_t SkipCount;
	uint32_t Count;
	uint16_t Error = 0;

//	//Get the header data... Allow up to 30 seconds to start
//	Error = USB_Serial_GetBytes(2, (uint8_t*)&BMPHeaderID, 30000);
	//Once first data received then timeout can be a lot smaller
	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPFileSize, USBTimeout) << 1;
	Error |= USB_Serial_GetBytes(2, (uint8_t*)&BMPUnused, USBTimeout) << 2;
	Error |= USB_Serial_GetBytes(2, (uint8_t*)&BMPUnused, USBTimeout) << 3;
	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPDataOffset, USBTimeout) << 4;
	//Get Windows header info
	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPHeaderSize, USBTimeout) << 5;
	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPWidth, USBTimeout) << 6;
	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPHeight, USBTimeout) << 7;
	Error |= USB_Serial_GetBytes(2, (uint8_t*)&BMPPlanes, USBTimeout) << 8;
	Error |= USB_Serial_GetBytes(2, (uint8_t*)&BMPBPP, USBTimeout) << 9;

	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPCompression, USBTimeout) << 10;
	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPImageSize, USBTimeout) << 11;
	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPHResolution, USBTimeout) << 12;
	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPVResolution, USBTimeout) << 13;
	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPPalette, USBTimeout) << 14;
	Error |= USB_Serial_GetBytes(4, (uint8_t*)&BMPImportant, USBTimeout) << 15;

	TotalBMPImageSize = BMPImageSize;

	if (BMPHeight < 0)
	{
		BMPHeight = -BMPHeight;//Top down instead of bottom up, so need to flip left right when imaging
		BMPFlip = -1;
	}
	else
		BMPFlip = 1;

	//Check if flipping the image for double sided exposure
	if (FlipEnable == 1)
		BMPFlip = -BMPFlip;
	//Check if image mirrored for bottom side exposure
	if (MirrorEnable == 0)
		BMPFlip = -BMPFlip;

//	POINT_COLOR = WHITE;
//	sprintf((char*)MiscBuffer, "Header = %c%c\n", BMPHeaderID[0], BMPHeaderID[1]);
//	LCD_Print((uint8_t*)MiscBuffer, 0);
	sprintf((char*)MiscBuffer, " File size = %d\n", BMPFileSize);
	LCD_Print((uint8_t*)MiscBuffer, 0);
	sprintf((char*)MiscBuffer, " Image size = %d\n", BMPImageSize);
	LCD_Print((uint8_t*)MiscBuffer, 0);
	sprintf((char*)MiscBuffer, " Width = %d\n", BMPWidth);
	LCD_Print((uint8_t*)MiscBuffer, 0);
	sprintf((char*)MiscBuffer, " Height = %d\n", BMPHeight);
	LCD_Print((uint8_t*)MiscBuffer, 0);
	sprintf((char*)MiscBuffer, " BPP = %d\n", BMPBPP);
	LCD_Print((uint8_t*)MiscBuffer, 0);
//	sprintf((char*)MiscBuffer, " HResolution = %d\n", BMPHResolution);
//	LCD_Print((uint8_t*)MiscBuffer, 0);
//	sprintf((char*)MiscBuffer, " VResolution = %d\n", BMPVResolution);
//	LCD_Print((uint8_t*)MiscBuffer, 0);

	SkipCount = BMPDataOffset - 54;
	for (Count = 1; Count <= SkipCount; Count++)
		Error |= USB_Serial_GetBytes(1, (uint8_t*)&BMPUnused, 3);

	sprintf((char*)MiscBuffer, " Skip count = %d\n", SkipCount);
	LCD_Print((uint8_t*)MiscBuffer, 0);
}

uint8_t GetPixelFromUSB()
{
	uint8_t Blue;
	uint8_t Green;
	uint8_t Red;
	uint8_t Alpha;
	uint8_t Result;
	static uint8_t Mono = 0;

	if (BMPBPP == 1)
	{
		if (MonoIndex == 0)
		{
			Error |= USB_Serial_GetBytes(1, (uint8_t*)&Mono, USBTimeout);
			TotalBytesRead = TotalBytesRead + 1;
		}
		if ((Mono & (128 >> MonoIndex)) != 0)
			Result = 0;
		else
			Result = 1;
		MonoIndex = (MonoIndex + 1) & 7;
	}
	else if (BMPBPP == 24)
	{
		Error |= USB_Serial_GetBytes(1, (uint8_t*)&Blue, USBTimeout);
		Error |= USB_Serial_GetBytes(1, (uint8_t*)&Green, USBTimeout);
		Error |= USB_Serial_GetBytes(1, (uint8_t*)&Red, USBTimeout);
		TotalBytesRead = TotalBytesRead + 3;
		if (Green < 128)
			Result = 1;
		else
			Result = 0;
	}
	else 	if (BMPBPP == 32)
	{
		Error |= USB_Serial_GetBytes(1, (uint8_t*)&Blue, USBTimeout);
		Error |= USB_Serial_GetBytes(1, (uint8_t*)&Green, USBTimeout);
		Error |= USB_Serial_GetBytes(1, (uint8_t*)&Red, USBTimeout);
		Error |= USB_Serial_GetBytes(1, (uint8_t*)&Alpha, USBTimeout);
		TotalBytesRead = TotalBytesRead + 4;
		if (Green < 128)
			Result = 1;
		else
			Result = 0;
	}
	//1 = Laser on i.e. no copper
	return Result ^ ImagePolarity;
}

void ImageProc_ScanBMPLine(void)
{
	uint16_t BytePosition;
	uint8_t BitPosition;
	uint8_t Pixel;
	float PrintXTrack;
	float PrintXIncrement;
	float LCDXTrack;
	float LCDXIncrement;
	int16_t LCDXPixel;
	uint16_t PixelsRead;
	uint8_t FlushCount;

	//Retrieve a line of data and store expanded in the line buffer
	WidthInSteps = (uint16_t) (BMPWidth * XTicksPerInch / ImageDPI);
	PrintXTrack = 0.0;
	PrintXIncrement = (float)BMPWidth / WidthInSteps;
	LCDXTrack = 0.0;
	LCDXIncrement = (float)LCD_W / WidthInSteps;

	if (BMPFlip == 1)//If image is top bottom then flip display in X
		LCDXPixel = 0;
	else
		LCDXPixel = (320*2) - 2;//right edge of image - 1 pixel (i.e. - 1 word)

	PixelsRead = 0;
	MonoIndex = 0;

	for (PrintBufferWidth = 0; PrintBufferWidth < WidthInSteps; PrintBufferWidth++)
	{
		if (PrintBufferWidth == 252)
			PixelsRead = PixelsRead;

		PrintXTrack = PrintXTrack - PrintXIncrement;
		//Fill the print buffer as necessary
		while ((PrintXTrack <= 0.0) && (PixelsRead < BMPWidth))
		{
			PrintXTrack = PrintXTrack + 1.0;
			Pixel = GetPixelFromUSB();
			PixelsRead++;
		}


		if (Error != 0)//If error then drop out of the loop
			PrintBufferWidth = WidthInSteps + 1;

		BitPosition = (PrintBufferWidth & 0x7);
		BytePosition = (PrintBufferWidth >> 3);
		if (Pixel != 0)
			LineBuffer[BytePosition] |= (1 << BitPosition);
		else
			LineBuffer[BytePosition] &= ~(1 << BitPosition);

		//Fill the LCD display buffer as necessary
		LCDXTrack = LCDXTrack - LCDXIncrement;
		if (LCDXTrack < 0.0)
		{
			LCDXTrack = LCDXTrack + 1.0;
			if (LCDXPixel >= 0)
			{
			if (Pixel != 0)
			{
				LCDLineBuffer[LCDXPixel++] = COPPERCOLOR >> 8;
				LCDLineBuffer[LCDXPixel++] = COPPERCOLOR & 0xff;
			}
			else
			{
				LCDLineBuffer[LCDXPixel++] = BOARDCOLOR >> 8;
				LCDLineBuffer[LCDXPixel++] = BOARDCOLOR & 0xff;
			}
			if (BMPFlip == -1)//Scan LCD image in opposite direction if source image is top to bottom
				LCDXPixel -=4;
			}
		}
	}
	if (Error == 0)
	{
		//Now re-align on a 4 byte boundary
		if (BMPBPP == 24)
		{
			FlushCount = ((((PixelsRead - 1) & 3) + 1) & 3);
		}
		else if ((BMPBPP == 1))
		{
			FlushCount = (3 - (((PixelsRead - 1) / 8) & 3));
		}
		else
			FlushCount = 0;

		TotalBytesRead = TotalBytesRead + FlushCount;

		while(FlushCount != 0)
		{
			Error |= USB_Serial_GetBytes(1, (uint8_t*)&Pixel, USBTimeout);
			FlushCount--;
		}
	}

}

void ImageProc_ScanLine(uint32_t XStepsToRun, int8_t XScanDirection, uint8_t BlankActive)
{
	int32_t SpeedRampDistance;
	volatile uint32_t P1;
	volatile uint32_t P2;
	volatile uint32_t P3;

	PrintingActive = 0;//Enable reading from the print buffer only for active part of scanning process
	//Prepare to ramp up speed
	SpeedRampDistance = XLocator * XTicksPerInch;
	Stepper_InitStepSegment(XMaxStepClocks, SpeedRampDistance, XAccelerationTime, XScanDirection);
	if (XScanDirection == -1)
		SpeedRampDistance = SpeedRampDistance + BackLashCorrection;
	//Move, accelerating up.
	StepperDebug = 1;
	Stepper_MoveSteps(-SpeedRampDistance * XScanDirection, MOVE_X_MOTOR);
	StepperDebug = 0;
	P1 = StepPeriod;
	//Signal actively sending data by setting the indexing direction to non-zero
	if (BlankActive == LINEISBLANK)
	{
		ImageIndexDirection = 0;
	}
	else if (XScanDirection == BMPFlip)
	{
		//Scan the bitmap line out
		PrintingActive = 1;//Enable reading from the print buffer only for active part of scanning process
		ImageIndexDirection = 1;
		ImageIndex = 0;
	}
	else
	{
		//Scan the bitmap line out
		PrintingActive = 1;//Enable reading from the print buffer only for active part of scanning process
		ImageIndexDirection = -1;
		ImageIndex = WidthInSteps - 1;
	}
	Stepper_InitStepSegment(XNomStepClocks, SpeedRampDistance, XAccelerationTime, 0);
	Stepper_MoveSteps(-XStepsToRun * XScanDirection, MOVE_X_MOTOR);
	P2 = StepPeriod;
	PrintingActive = 0;
	//Decelerate now
//	ImageIndexDirection = 0;
	//Ensure laser is off
	GPIOPinWrite(LASER_PORT, LASER_BIT, SETLOW);
	//Prepare to ramp down speed
	Stepper_InitStepSegment(XMaxStepClocks, SpeedRampDistance, XAccelerationTime, -XScanDirection);
	SpeedRampDistance = XLocator * XTicksPerInch;
	if (XScanDirection == -1)
		SpeedRampDistance = SpeedRampDistance - BackLashCorrection;
	//Move accelerating down.
	Stepper_MoveSteps(-SpeedRampDistance * XScanDirection, MOVE_X_MOTOR);
	P3 = StepPeriod;
	//Seems to miss a tick if no delay here (and in scanblankline)
	WaitFor(2);
}

void ImageProc_ScanLine_original(uint32_t XStepsToRun, int8_t XScanDirection)
{
	int32_t SpeedRampDistance;
	volatile uint32_t P1;
	volatile uint32_t P2;
	volatile uint32_t P3;

	//Clear flags for image line indexing
	ImageIndexDirection = 0;
	//Prepare to ramp up speed
	Stepper_InitStepSegment(XAcceleration, XMinStepClocks, XMaxStepClocks, XNomStepClocks);
	SpeedRampDistance = XLocator * XTicksPerInch;
	if (XScanDirection == -1)
		SpeedRampDistance = SpeedRampDistance + BackLashCorrection;
	//Move, accelerating up.
	Stepper_MoveSteps(-SpeedRampDistance * XScanDirection, MOVE_X_MOTOR);
	P1 = StepPeriod;
	//Scan the bitmap line out
	PrintingActive = 1;//Enable reading from the print buffer only for active part of scanning process
	//Signal actively sending data by setting the indexing direction to non-zero
	if (XScanDirection == BMPFlip)
	{
		ImageIndexDirection = 1;
		ImageIndex = 0;
	}
	else
	{
		ImageIndexDirection = -1;
		ImageIndex = WidthInSteps - 1;
	}
	Stepper_InitStepSegment(0, XMinStepClocks, XMaxStepClocks, XNomStepClocks);
	Stepper_MoveSteps(-XStepsToRun * XScanDirection, MOVE_X_MOTOR);
	P2 = StepPeriod;
	PrintingActive = 0;
	//Decelerate now
//	ImageIndexDirection = 0;
	//Ensure laser is off
	GPIOPinWrite(LASER_PORT, LASER_BIT, SETLOW);
	//Prepare to ramp down speed
	Stepper_InitStepSegment(-XAcceleration, XMinStepClocks, XMaxStepClocks, XNomStepClocks);
	SpeedRampDistance = XLocator * XTicksPerInch;
	if (XScanDirection == -1)
		SpeedRampDistance = SpeedRampDistance - BackLashCorrection;
	//Move accelerating down.
	Stepper_MoveSteps(-SpeedRampDistance * XScanDirection, MOVE_X_MOTOR);
	P3 = StepPeriod;
	//Seems to miss a tick if no delay here (and in scanblankline)
	WaitFor(2);
}

void ImageProc_ScanBlankLine_not_used(uint32_t XStepsToRun, int8_t XScanDirection)
{
	PrintingActive = 0;
	//Ensure laser is off
	GPIOPinWrite(LASER_PORT, LASER_BIT, SETLOW);
//	//Clear flags for image line indexing
//	ImageIndexDirection = 0;
	//Prepare to ramp up speed
	Stepper_InitStepSegment(XAcceleration, XMinStepClocks, XMaxStepClocks, XNomStepClocks);
	//Move accelerating up.
	Stepper_MoveSteps(-XLocator * XTicksPerInch * XScanDirection, MOVE_X_MOTOR);
//	ImageIndexDirection = 0;
//	ImageIndex = 0;
	Stepper_InitStepSegment(0, XMinStepClocks, XMaxStepClocks, XNomStepClocks);
	Stepper_MoveSteps(-XStepsToRun * XScanDirection, MOVE_X_MOTOR);
//	//Decelerate now
//	ImageIndexDirection = 0;
	//Calculate parameters
	Stepper_InitStepSegment(-XAcceleration, XMinStepClocks, XMaxStepClocks, XNomStepClocks);
	//Move accelerating down.
	Stepper_MoveSteps(-XLocator * XTicksPerInch * XScanDirection, MOVE_X_MOTOR);
	//Seems to miss a tick if no delay here (and in scanline)
	WaitFor(2);
}

uint8_t GetNextLaserBit(void)
{
	uint16_t BytePosition;
	uint8_t BitPosition;
	uint8_t ReturnValue;

	if (ImageIndexDirection != 0)
	{
		//If not zero the sending the image
		BitPosition = (ImageIndex & 0x7);
		BytePosition = (ImageIndex >> 3);

		if (((LineBuffer[BytePosition] & (1 << BitPosition)) != 0) || (LaserDisable))
			ReturnValue = SETLOW;
		else
			ReturnValue = SETHIGH;

		//Move to next bit ready for next time
		ImageIndex = ImageIndex + ImageIndexDirection;
	}
	else
	{
		//Otherwise not sending an image, just moving
		ReturnValue = SETLOW;
	}

	return ReturnValue;
}

uint32_t ImageProc_ScanBMP(void)
{
	float LCDYTrack;
	uint16_t LCDYRow;
	float YPrintPos;
	float YReadTrack;
	float YSourceResolution;
	float YLCDResolution;
	float PrintHeight;
	float RealStepsToRun;
	int8_t XDirection = 1;//ToDo : Check
	uint32_t TotalTime;
	uint32_t ETA;
	uint16_t Mins, Secs;
	uint16_t RowCounter = 0;
	uint8_t  BLCTestCount = 0;
	uint8_t Temp;
	uint32_t i;
	uint32_t FlushCount = 0;

	if (FlipEnable == 1)
		XDirection = -1;
	else
		XDirection = 1;

	PrintHeight = (float)BMPHeight / ImageDPI;
//	YPrintResolution = 1.0/ImageDPI;
	YPrintResolution = 1.0/YPrintDPI;//Print resolution is governed by the laser dot size. Distance per unit step
	//This tracks the actual y positing in inches. Start at one increment in to help align with end of file. Using floats not great but simple
	YPrintPos = YPrintResolution;
	RealStepsToRun = 0.0;
	//This tracks how often we need to read a line of data from the source image
	YReadTrack = 0.0;
	//This tracks how often we need to update the LCD display
	LCDYTrack = 0.0;
	//Effective LCD DPI wrt source image
	YLCDResolution = 240.0 /(PrintHeight * YPrintDPI);
	//And where we are currently displaying
	LCDYRow = 239;
	//Source image resolution in inches
	YSourceResolution = 1.0/ImageDPI;//Distance per pixel

	ResetTime();
	TotalBytesRead = 0;

	XStepTracker = 0;
	//Scan through the source image
	while((YPrintPos < PrintHeight) & (Error == 0))
	{
		LaserEnable = 0;
		if ((UniDirection == 0) || ((UniDirection == 1) && (XDirection == 1)))// If uni-direction then only update when scanning positive direction
		{
			LaserEnable = 1;
			//Update actual print position. Not really needed but good to track for the moment
			YPrintPos = YPrintPos + YPrintResolution;
			//Track if we need to update the read data
			YReadTrack = YReadTrack + YPrintResolution;
			//Track if we need to update the LCD
			LCDYTrack = LCDYTrack + YLCDResolution;

			//If rolled over then read a new line from the file. Zero checked so initial startup will read a line
			//If source resolution is higher then print resolution then will loop multiple times
			while (YReadTrack > 0.0)
			{
				ImageProc_ScanBMPLine();
				YReadTrack = YReadTrack - YSourceResolution;
			}
			//Track if we need to update the LCD.
			while(LCDYTrack > 0.0)
			{
				LCDYTrack = LCDYTrack - 1;
				LCD_SetDataLine(LCDYRow, LCDLineBuffer);
				LCDYRow --;
			}
		}
		RowCounter++;
		//Now everyone is update actually send the scan line
		if (TestMode == 0)
			ImageProc_ScanLine(PrintBufferWidth, XDirection, LINEISACTIVE);

		XDirection =  - XDirection;

		if ((UniDirection == 0) || ((UniDirection == 1) && (XDirection == 1)))// If uni-direction then only update when scanning positive direction
		{
			//Now advance in Y
			RealStepsToRun = RealStepsToRun + (float)YTicksPerInch * YPrintResolution;
			StepsToRun = (int)RealStepsToRun;
			//Track difference of fractional steps required
			RealStepsToRun = RealStepsToRun - StepsToRun;
			//Step the Y direction now
			//Stepper_InitStepSegment(0, HomeYStepPeriodSlow, HomeYStepPeriodSlow, HomeYStepPeriodSlow);
			Stepper_InitStepSegment(HomeYStepPeriodSlow, 0, 0, 0);
			Stepper_MoveSteps(-StepsToRun, MOVE_Y_MOTOR);
			TotalTime = (TimeTick * PrintHeight)/YPrintPos;

			ETA = TotalTime - TimeTick;

			Mins = TotalTime / 60000;
			Secs = (TotalTime - (60000 * Mins)) / 1000;
			sprintf((char*)MiscBuffer, "Est = %3d:%02d  ", Mins, Secs);
			LCD_FontString(180,0,MiscBuffer,0);
			Mins = ETA / 60000;
			Secs = (ETA - (60000 * Mins)) / 1000;
			sprintf((char*)MiscBuffer, "ETA = %3d:%02d  ", Mins, Secs);
			LCD_FontString(180,20,MiscBuffer,0);
			if (TestBacklashCorrection == 1)
			{
				if (BLCTestCount == 3)
				{
					BLCTestCount = 0;
					BackLashCorrection++;
					if (BackLashCorrection == 51)
					{
						//If done all 100 tests then stop
						YPrintPos = PrintHeight;
					}
				}
				else
				{
					BLCTestCount++;
				}
			}
		}
//		sprintf((char*)MiscBuffer, "Tracker = %5d    ", XStepTracker);
//		LCD_FontString(0,40,MiscBuffer,0);
	}
	//Note how long we actually took
	TotalTime = TimeTick;
	//Move carriage out of the way now
	if ((TestMode == 0) && (MoveWhenFinished == 1))
	{
		StepsToRun = (float)YTicksPerInch * (7 - PrintHeight);
		//Stepper_InitStepSegment(0, HomeYStepPeriodFast, HomeYStepPeriodFast, HomeYStepPeriodFast);
		Stepper_InitStepSegment(HomeYStepPeriodFast, 0, 0, 0);
		Stepper_MoveSteps(-StepsToRun, MOVE_Y_MOTOR);
	}
	//Remove any stray data from the tail of the image
	if (TotalBytesRead != TotalBMPImageSize)
	{
		FlushCount = TotalBMPImageSize - TotalBytesRead;
		for (i = 0; i < FlushCount; i++)
		{
			USB_Serial_GetBytes(1, (uint8_t*)&Temp, USBTimeout);
		}
	}

//	//Make sure nothing left in the USB buffer
//	USB_SERIAL_FlushRX();

	return TotalTime;
}

void ImageProc_PrintBMP(void)
{
	uint32_t Time;
	uint32_t Mins, Secs;

	LCD_Clear(WHITE);
	POINT_COLOR = DARKGREEN;

	ImageProc_GetBMPHeader();

	if ((BMPBPP != 24) && (BMPBPP != 32) && (BMPBPP != 1))
	{
		LCD_Clear(RED);
		POINT_COLOR = WHITE;
		LCD_Print((uint8_t*)" Error : Bitmap not mono, 24bpp or 32bpp\n", 0);
		USB_SERIAL_FlushRX();
	}
	else
	{
		LCD_Print((uint8_t*)" Reading image\n", 0);
		//Should be a good image so move home
		Stepper_Home(HomeXStepPeriodFast, HomeXStepPeriodSlow, CHECK_X_LIMIT, MOVE_X_MOTOR, XTicksPermm);

		//Y home
		if (HomeBeforePrint == 1)
			Stepper_Home(HomeYStepPeriodFast, HomeYStepPeriodSlow, CHECK_Y_LIMIT, MOVE_Y_MOTOR, YTicksPermm);

		//If image is flipped then move carriage to far side of bed
		if (FlipEnable == 1)
			ImageProc_ScanLine((ImageFlipOffset - XLocator - XLocator - XOffset) * XTicksPerInch, 1, LINEISBLANK);//subtract xoffset distance
		else
			Stepper_MoveSteps(-XOffset * XTicksPerInch, MOVE_X_MOTOR);//Otherwise just move the xoffset distance

		//If checking backlash then force some settings
		if (TestBacklashCorrection == 1)
		{
			YPrintDPI = 1000;
			ImageDPI = 1000;
			UniDirection = 0;
			BackLashCorrection = -50;
			MirrorEnable = 0;
			FlipEnable = 0;

		}
		LCD_Clear(WHITE);
		ComProc_ProcessStat();
		//Print the image
		Time = ImageProc_ScanBMP();
		//Ensure the laser diode off
		GPIOPinWrite(LASER_PORT, LASER_BIT, 0);
		ImageIndexDirection = 0;
	}

	if (Error != 0)
	{
		LCD_Clear(RED);
		POINT_COLOR = WHITE;
		LCD_Print((uint8_t*) "Error printing!!\n", 0);
		LCD_Print((uint8_t*) "File copy cancelled\n", 0);
	}
	else
	{
		LCD_Clear(GREEN);
		POINT_COLOR = WHITE;
		Mins = Time / 60000;
		Secs = (Time - (60000 * Mins)) / 1000;
		sprintf((char*)MiscBuffer, "Actual = %3d:%02d\n", Mins, Secs);
		LCD_Print((uint8_t*) MiscBuffer, 0);
	}

	//Done
	FoundBM = 0;
	USBSerialMode = USBTEXTMODE;//Switch back to text mode for command processing
}
