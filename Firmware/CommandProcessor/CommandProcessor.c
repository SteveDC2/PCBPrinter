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

uint8_t CommandCircularBuffer[COMMAND_BUFFER_SIZE];
uint8_t CommandBuffer[COMMAND_BUFFER_SIZE];
uint8_t CommandCount;
uint8_t CommandWritePointer;
uint8_t CommandReadPointer;
uint8_t* PolarityMessage[] = {"positive", "negative"};
uint8_t* OffOnMessage[] = {"off", "on"};
uint8_t* AxisMessage[] = {"X", "Y"};

void GetNextCommand(void)
{
	static uint8_t Character = 0;
	static uint8_t LastCharacter = 0;
	static uint8_t CommandLength = 0;
	uint8_t CommandFound = 0;
	int32_t DataRead;

	while((FoundBM == 0) && (CommandFound == 0))
	{
//		if ((DataAvailable = USBBufferDataAvailable((tUSBBuffer *)&g_sRxBuffer)) == 0)
//			WaitFor(2);
		DataRead = USBBufferRead((tUSBBuffer *)&g_sRxBuffer, &Character, 1);
		if (DataRead != 0)
		{
			if ((Character == 'M') && (LastCharacter == 'B'))
			{
				FoundBM = 1;
				CommandLength = 0;
			}
			else
			{
				//Convert line feed to carriage return
				if (Character == 10)
					Character = 13;
				//If multiple carriage returns, then just ignore
				if ((Character == 13) && (LastCharacter ==13))
					Character = 0;
				//If first character and a space then ignore it
				if ((Character == 32) && (CommandLength == 0))
					Character = 0;
				//If carriage return and something in the buffer then a command has been found
				if ((Character == 13) && (CommandLength != 0))
				{
					//Terminate the command string
					CommandBuffer[CommandLength] = 0;
					//Reset ready for next command
					CommandLength = 0;
					if (CommandBuffer[0] != ';')//';' is a comment. Only valid at line start
						//Flag command found
						CommandFound = 1;
				}
				//If valid character then put in the buffer
				else if (Character != 0)
				{
					CommandBuffer[CommandLength] = toupper(Character);
					CommandLength++;
				}
			}
			LastCharacter = Character;
		}
	}
}
	
void ShowMessage(unsigned char* Buffer)
{
	LCD_Print((uint8_t*) Buffer, 0);
	USB_Serial_SendMessage(Buffer);
}

void ComProc_ProcessStat()
{
	ShowMessage("Status\n");
	ShowMessage("******\n");
	sprintf((char*)MiscBuffer, "Image DPI = %d\n",ImageDPI);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "Image Polarity = %s\n",PolarityMessage[ImagePolarity]);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "Laser disable = %s\n",OffOnMessage[LaserDisable]);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "Mirror image = %s\n",OffOnMessage[MirrorEnable]);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "Flip image = %s\n",OffOnMessage[FlipEnable]);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "UniDirection = %s\n",OffOnMessage[UniDirection]);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "Home before print = %s\n",OffOnMessage[HomeBeforePrint]);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "Move when finished = %s\n",OffOnMessage[MoveWhenFinished]);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "Image flip offset = %2.3f\n",ImageFlipOffset);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "Backlash correction = %s\n",OffOnMessage[BackLashCorrection]);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "BLC test mode = %s\n",OffOnMessage[TestBacklashCorrection]);
	ShowMessage(MiscBuffer);
	sprintf((char*)MiscBuffer, "Test mode = %s\n",OffOnMessage[TestMode]);
	ShowMessage(MiscBuffer);
}

void
ProcessHelp(void)
{
	USB_Serial_SendMessage((unsigned char*)"PCB Printer\n\n");
	USB_Serial_SendMessage((unsigned char*)"  HELP       : Display this help\n");
	USB_Serial_SendMessage((unsigned char*)"  STAT       : Display current settings\n");
	USB_Serial_SendMessage((unsigned char*)"  RUNINY     : Execute Y lead thread runin sequence\n");
	USB_Serial_SendMessage((unsigned char*)"  LON        : Turn on the laser\n");
	USB_Serial_SendMessage((unsigned char*)"  LOFF       : Turn off the laser\n");
	USB_Serial_SendMessage((unsigned char*)"  HOMEX      : Send laser home in X\n");
	USB_Serial_SendMessage((unsigned char*)"  HOMEY      : Send laser home in Y\n");
	USB_Serial_SendMessage((unsigned char*)"  MX n.n     : Move head X inches \n");
	USB_Serial_SendMessage((unsigned char*)"  MY n.n     : Move head Y inches \n");
	USB_Serial_SendMessage((unsigned char*)"\n");

	USB_Serial_SendMessage((unsigned char*)"  HBP x      : Enable/disable home before print\n");
	USB_Serial_SendMessage((unsigned char*)"  MWF x      : Enable/disable move head out of the way when finished\n");
	USB_Serial_SendMessage((unsigned char*)"  LDIS x     : Enable/disable laser\n");
	USB_Serial_SendMessage((unsigned char*)"\n");

	USB_Serial_SendMessage((unsigned char*)"  DPI nnn    : Set source BMP image DPI\n");
	USB_Serial_SendMessage((unsigned char*)"  YPR nnn    : Set Y print resolution DPI\n");
	USB_Serial_SendMessage((unsigned char*)"  UNI x      : Enable/disable uni-directional exposure\n");
	USB_Serial_SendMessage((unsigned char*)"  MIR x      : Set/disable image mirroring\n");
	USB_Serial_SendMessage((unsigned char*)"  FLIP x     : Set/disable image flipping\n");
	USB_Serial_SendMessage((unsigned char*)"  POL x      : Set BMP image polarity inversion\n");
	USB_Serial_SendMessage((unsigned char*)"  XO n.n     : Set print X offset in inches \n");
	USB_Serial_SendMessage((unsigned char*)"\n");

	USB_Serial_SendMessage((unsigned char*)"  IFO n.n    : Image flip offset (inches)\n");
	USB_Serial_SendMessage((unsigned char*)"  BLC nnn    : Set backlash correction\n");
	USB_Serial_SendMessage((unsigned char*)"  TBLC x     : Set backlash test mode\n");
	USB_Serial_SendMessage((unsigned char*)"  TEST x     : Set scanning test mode\n");
	USB_Serial_SendMessage((unsigned char*)"\n");
}
	
	void 
		ProcessSetSerial(unsigned char * Buffer)
	{
		SerialNumber = strtol ((const char*)Buffer, NULL, 0);
		EEPROMProgram((uint32_t*)&SerialNumber, EEPROMSize - sizeof(SerialNumber), sizeof(SerialNumber));		
	}
	
	void 
		ProcessReset(void)
	{
		EEPROMMassErase();
		USB_Serial_SendMessage((unsigned char*)"EEPROM reset. Power cycle to continue");
	}
	
	void
		ProcessSetDisplayFormat(uint32_t Format)
	{
		DeviceInfo.NLFormat = Format;
		EEPROMProgram((uint32_t *)&DeviceInfo, 0, sizeof(DeviceInfo));
	}
	
	void MoveLaser(float Distance, int Axis)
	{
		float TicksPerInch;

		sprintf((char*)MiscBuffer, "Move %c = %f\n",AxisMessage[Axis], Distance);
		ShowMessage(MiscBuffer);
		if (Axis == MOVE_X_MOTOR)
		{
			StepPeriod = HomeXStepPeriodSlow;
			TicksPerInch = XTicksPerInch;
		}
		else
		{
			StepPeriod = HomeXStepPeriodSlow;
			TicksPerInch = YTicksPerInch;
		}
		CheckLimits = CHECK_X_LIMIT | CHECK_Y_LIMIT;
		Stepper_EnableSteppers(STEPPERS_ON);
		Stepper_MoveSteps(-Distance * TicksPerInch, Axis);
		Stepper_EnableSteppers(STEPPERS_OFF);
	}

	void ComProc_ProcessCommand(void)
		{
			uint8_t State = 0;
			float MoveDistance;

			GetNextCommand();
			if (FoundBM == 1)
			{
				//BM header found so process the bitmap
				ImageProc_PrintBMP();
				Error = 0;
				//Finished so turn off steppers
				Stepper_EnableSteppers(STEPPERS_OFF);
				//Clear the flag
				FoundBM = 0;

			}
			else
			{
				if (CommandBuffer[0] == '#')
				{
					//Skip comments
				}
				else if      (memcmp((const char*)&CommandBuffer, "HELP", 4) == 0)
				{
					ProcessHelp();
				}
				else if (memcmp((const char*)&CommandBuffer, "HOMEX", 4) == 0)
				{
					Stepper_Home(HomeXStepPeriodFast, HomeXStepPeriodSlow, CHECK_X_LIMIT, MOVE_X_MOTOR, XTicksPermm);
				}
				else if (memcmp((const char*)&CommandBuffer, "HOMEY", 4) == 0)
				{
					Stepper_Home(HomeYStepPeriodFast, HomeYStepPeriodSlow, CHECK_Y_LIMIT, MOVE_Y_MOTOR, YTicksPermm);
				}
				else if (memcmp((const char*)&CommandBuffer, "ACCEL", 5) == 0)
				{
					Stepper_AccelerationTest();
				}
				else if (memcmp((const char*)&CommandBuffer, "RUNINY", 6) == 0)
				{
					Stepper_RunInY();
				}
				else if (memcmp((const char*)&CommandBuffer, "LON", 3) == 0)
				{
					GPIOPinWrite(LASER_PORT, LASER_BIT, SETHIGH);
					ShowMessage((unsigned char*)"WARNING : Laser On");
				}
				else if (memcmp((const char*)&CommandBuffer, "LOFF", 3) == 0)
				{
					GPIOPinWrite(LASER_PORT, LASER_BIT, SETLOW);
					ShowMessage((unsigned char*)"Laser Off");
				}
				else if (memcmp((const char*)&CommandBuffer, "UNI ", 4) == 0)
				{
					UniDirection = strtol((char*)&CommandBuffer[4], NULL, 10) & 1;
					sprintf((char*)MiscBuffer, "Set : UniDirection = %s\n",OffOnMessage[UniDirection]);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "BLC ", 4) == 0)
				{
					BackLashCorrection = strtol((char*)&CommandBuffer[4], NULL, 10) & 1;
					sprintf((char*)MiscBuffer, "Set : Backlash correction = %s\n",OffOnMessage[BackLashCorrection]);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "MWF ", 4) == 0)
				{
					MoveWhenFinished = strtol((char*)&CommandBuffer[4], NULL, 10) & 1;
					sprintf((char*)MiscBuffer, "Set : Move when finished = %s\n",OffOnMessage[MoveWhenFinished]);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "HBP ", 4) == 0)
				{
					HomeBeforePrint = strtol((char*)&CommandBuffer[4], NULL, 10) & 1;
					sprintf((char*)MiscBuffer, "Set : Home before print = %s\n",OffOnMessage[HomeBeforePrint]);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "IFO ", 4) == 0)
				{
					ImageFlipOffset = strtof((char*)&CommandBuffer[4], NULL);
					sprintf((char*)MiscBuffer, "Set : Image flip offset = %2.3f\n",ImageFlipOffset);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "FLIP ", 5) == 0)
				{
					FlipEnable = strtol((char*)&CommandBuffer[5], NULL, 10) & 1;
					sprintf((char*)MiscBuffer, "Set : Image flip = %s\n",OffOnMessage[FlipEnable]);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "TBLC ", 5) == 0)
				{
					TestBacklashCorrection = strtol((char*)&CommandBuffer[5], NULL, 10) & 1;
					sprintf((char*)MiscBuffer, "Set : BLC test = %s\n",OffOnMessage[TestBacklashCorrection]);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "LDIS ", 5) == 0)
				{
					LaserDisable = strtol((char*)&CommandBuffer[5], NULL, 10) & 1;
					sprintf((char*)MiscBuffer, "Set : Laser disable = %s\n",OffOnMessage[LaserDisable]);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "DPI ", 4) == 0)
				{
					ImageDPI = strtol((char*)&CommandBuffer[4], NULL, 10);
					sprintf((char*)MiscBuffer, "Set : Image DPI = %d\n",ImageDPI);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "YPR ", 4) == 0)
				{
					YPrintDPI = strtol((char*)&CommandBuffer[4], NULL, 10);
					sprintf((char*)MiscBuffer, "Set : Y print res = %d\n",YPrintDPI);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "POL ", 4) == 0)
				{
					ImagePolarity = strtol((char*)&CommandBuffer[4], NULL, 10) & 1;
					sprintf((char*)MiscBuffer, "Set : Image polarity = %s\n",PolarityMessage[ImagePolarity]);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "MIR ", 4) == 0)
				{
					MirrorEnable = strtol((char*)&CommandBuffer[4], NULL, 10) & 1;
					sprintf((char*)MiscBuffer, "Set : Image mirror = %s\n",OffOnMessage[MirrorEnable]);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "XO ", 3) == 0)
				{
					XOffset = strtof((char*)&CommandBuffer[3], NULL);
					sprintf((char*)MiscBuffer, "Set : X offset = %f\n",XOffset);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "MX ", 3) == 0)
				{
					MoveDistance = strtof((char*)&CommandBuffer[3], NULL);
					MoveLaser(MoveDistance, MOVE_X_MOTOR);
				}
				else if (memcmp((const char*)&CommandBuffer, "MY ", 3) == 0)
				{
					MoveDistance = strtof((char*)&CommandBuffer[3], NULL);
					MoveLaser(MoveDistance, MOVE_Y_MOTOR);
				}
				else if (memcmp((const char*)&CommandBuffer, "TEST ", 5) == 0)
				{
					TestMode = strtol((char*)&CommandBuffer[5], NULL, 10) & 1;
					sprintf((char*)MiscBuffer, "Set : Test mode = %s\n",OffOnMessage[TestMode]);
					ShowMessage(MiscBuffer);
				}
				else if (memcmp((const char*)&CommandBuffer, "STAT", 4) == 0)
				{
					ComProc_ProcessStat();
				}
				else if (memcmp((const char*)&CommandBuffer, "STSERIAL ", 9) == 0)
				{
					ProcessSetSerial(&CommandBuffer[9]);
				}
				else if (memcmp((const char*)&CommandBuffer, "ReSeT", 5) == 0)
				{
					ProcessReset();
				}
				else if (memcmp((const char*)&CommandBuffer, "SETUNIX", 5) == 0)
				{
					ProcessSetDisplayFormat(UNIX_FORMAT);
				}
				else if (memcmp((const char*)&CommandBuffer, "SETDOS", 5) == 0)
				{
					ProcessSetDisplayFormat(DOS_FORMAT);
				}
				else if (memcmp((const char*)&CommandBuffer, "SETOLD", 5) == 0)
				{
					ProcessSetDisplayFormat(PRE_OSX_FORMAT);
				}
				else if (memcmp((const char*)&CommandBuffer, "ECHOON", 6) == 0)
				{
					DeviceInfo.EchoEnable = 1;
					EEPROMProgram((uint32_t *) &DeviceInfo, 0, sizeof(DeviceInfo));
				}
				else if (memcmp((const char*)&CommandBuffer, "ECHOOFF", 7) == 0)
				{
					DeviceInfo.EchoEnable = 0;
					EEPROMProgram((uint32_t *) &DeviceInfo, 0, sizeof(DeviceInfo));
				}
				else if (CommandBuffer[0] != 0)
				{
					ShowMessage((unsigned char*)"Unknown command\n");
					ShowMessage((unsigned char*)CommandBuffer);
					ShowMessage((unsigned char*)"\n");
					State = 1;
				}

				if (State == 0)
					USB_Serial_SendMessage((unsigned char*)"OK");
				else
					USB_Serial_SendMessage((unsigned char*)"NAK");

		}
}
