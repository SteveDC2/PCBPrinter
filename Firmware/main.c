//Use command 'copy Test.bmp \\.\COM16' to print
#include "main.h"
#include "math.h"
#include "Init.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_uart.h"
#include "inc/hw_sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/usb.h"
#include "driverlib/pwm.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/eeprom.h"
#include "usblib/usblib.h"
#include "usblib/usbcdc.h"
#include "usblib/usb-ids.h"
#include "usblib/device/usbdevice.h"
#include "usblib/device/usbdcdc.h"
#include "utils/ustdlib.h"
#include "usb_serial_structs.h"
#include "utils/uartstdio.h"
#include "driverlib/ssi.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "LCD.h"
#include "TSC.h"
#include "Helpers.h"
#include "SPI.h"
#include "Menus.h"
#include "Bitmaps.h"
#include "USBSerial.h"
#include "CommandProcessor.h"
#include "EEPROMUtils.h"
#include "ImageProcessing.h"
#include "inc/hw_memmap.h"
#include "Stepper.h"

uint8_t LineBuffer[LINE_BUFFER_DEPTH];

const unsigned char FWVersion[] = {"FW version 1.00"};
const unsigned char BuildDate[] = "Build date = " __DATE__;
const unsigned char BuildTime[] = "Build Time = " __TIME__;

uint32_t SerialNumber = 0;
DeviceInfoType DeviceInfo = {.CurrentConfigurationIndex = 0, .NLFormat = DOS_FORMAT, .EchoEnable = 1};

unsigned char MiscBuffer[640];

uint16_t YTicksPermm;
float YTicksPerInch;
uint32_t MaxYTravel;
uint32_t MaxYTravelTicks;
uint32_t CurrentYPosition;

uint16_t XTicksPermm;
float XTicksPerInch;
uint32_t MaxXTravel;
uint32_t MaxXTravelTicks;
uint32_t CurrentXPosition;
uint16_t ImageDPI;
uint8_t Error;
uint8_t LaserEnable = 0;
float YPrintResolution;
float XOffset = 0;

int32_t XMinStepClocks;
int32_t XMaxStepClocks;
int32_t XNomStepClocks;

uint8_t OKSendUSB = 1;//Assume OK to send to terminal initially

//*****************************************************************************
//
// Interrupt handler for the system tick counter.
//
//*****************************************************************************
void
SysTickIntHandler(void)
{
	if (DelayCounter > 0)
		DelayCounter--;
	if (ResetTimeTick != 0)
	{
		TimeTick = 0;
		ResetTimeTick = 0;
	}
	else
	{
		TimeTick++;
	}
}

void SqrtTest()
{
	uint32_t i;
	volatile uint32_t Result;
	uint32_t TimeTaken;
	volatile float temp = 1234.3;
	#define Itterations 100000

	POINT_COLOR = DARKGREEN;

	sprintf((char*)MiscBuffer, "Running %d itterations\n", Itterations);
	LCD_Print(MiscBuffer, 0);

	ResetTime();
	for (i = 0; i < Itterations; i++)
	{
		Result = sqrt(temp);
		temp = temp + 13.1;
	}
	TimeTaken = TimeTick;
	sprintf((char*)MiscBuffer, "math.h library time = %d ms\n", TimeTaken);
	LCD_Print(MiscBuffer, 0);

	ResetTime();
	for (i = 0; i < Itterations; i++)
	{
		Result = SquareRoot(temp);
		temp = temp + 13.1;
	}
	TimeTaken = TimeTick;
	sprintf((char*)MiscBuffer, "SquareRoot() time = %d ms\n", TimeTaken);
	LCD_Print(MiscBuffer, 0);

	ResetTime();
	for (i = 0; i < Itterations; i++)
	{
		Result = SquareRootRounded(temp);
		temp = temp + 13.1;
	}
	TimeTaken = TimeTick;
	sprintf((char*)MiscBuffer, "SquareRootRounded() time = %d ms\n", TimeTaken);
	LCD_Print(MiscBuffer, 0);

	ResetTime();
	for (i = 0; i < Itterations; i++)
	{
		Result = fisqrt(temp);
		temp = temp + 13.1;
	}
	TimeTaken = TimeTick;
	sprintf((char*)MiscBuffer, "fisqrt() time = %d ms\n", TimeTaken);
	LCD_Print(MiscBuffer, 0);

	ResetTime();
	for (i = 0; i < Itterations; i++)
	{
		Result = int_sqrt32(temp);
		temp = temp + 13.1;
	}
	TimeTaken = TimeTick;
	sprintf((char*)MiscBuffer, "int_sqrt32() time = %d ms\n", TimeTaken);
	LCD_Print(MiscBuffer, 0);

	while(1);
}

//*****************************************************************************
//
// This is the main application entry function.
//
//*****************************************************************************
int
main(void)
{
	//Initialize stack and clocks

	Init_SystemInit();

	Init_PeripheralInit();

	Init_SetDefaults();

//	LCD_DisplayImage(0, 0, (uint16_t*)dilbert_reduced16, 0);

	Stepper_EnableSteppers(STEPPERS_ON);
	Stepper_ResetSteppers();

	//SqrtTest();

//	TSC_WaitTouched();


	//Move home
//	Stepper_Home(HomeXStepPeriodFast, HomeXStepPeriodSlow, CHECK_X_LIMIT, MOVE_X_MOTOR, XTicksPermm);
//	Stepper_Home(HomeYStepPeriodFast, HomeYStepPeriodSlow, CHECK_Y_LIMIT, MOVE_Y_MOTOR, YTicksPermm);

	LCD_Clear(WHITE);
	POINT_COLOR = DARKGREEN;
	sprintf((char*)MiscBuffer, "X steps/mm %d, /\"%4.4f\n", XTicksPermm, XTicksPerInch);
//	LCD_FontString(0, 0, MiscBuffer, 0);
	LCD_Print(MiscBuffer, 0);
	sprintf((char*)MiscBuffer, "Y steps/mm %d, /\"%4.4f\n", YTicksPermm, YTicksPerInch);
//	LCD_FontString(0, 30, MiscBuffer, 0);
	LCD_Print(MiscBuffer, 0);
	sprintf((char*)MiscBuffer, "Assuming Image DPI %d\n", ImageDPI);
//	LCD_FontString(0, 60, MiscBuffer, 0);
	LCD_Print(MiscBuffer, 0);


//	LoopMove(MOVE_X_MOTOR, MaxXTravelTicks, HomeXStepPeriodFast);

	Stepper_EnableSteppers(STEPPERS_OFF);

	LCD_Print("To print image copy file to com port\n", 0);
	LCD_Print("E.g.\n", 0);
	LCD_Print("\"copy file.bmp \\\\.\\COM2\"\n", 0);

//	GPIOPinWrite(LASER_PORT, LASER_BIT, SETHIGH);
//	GPIOPinWrite(LASER_PORT, LASER_BIT, SETLOW);


    LCD_SetFont(0);
	POINT_COLOR = RED;
	ImageIndexDirection = 0;

//	Stepper_LoopMove(MOVE_X_MOTOR, 1000, HomeXStepPeriodFast);

	while(1)
	{
//		Menus_ShowMenu((MenuItemDescriptor*)&TopMenu);
//		if (FoundBM == 1)
//		{
//			//BM header found so process the bitmap
//			ImageProc_PrintBMP();
//			Error = 0;
//			//Finished so turn off steppers
//			Stepper_EnableSteppers(STEPPERS_OFF);
//			//Clear the flag
//			FoundBM = 0;
//		}
//		if (CommandCount != 0)
			ComProc_ProcessCommand();
	}
}
