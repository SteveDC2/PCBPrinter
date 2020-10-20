#include "math.h"
#include "main.h"
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
#include <limits.h>
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
#include "Stepper.h"
#include "USBSerial.h"
#include "CommandProcessor.h"
#include "EEPROMUtils.h"
#include "ImageProcessing.h"
#include "inc/hw_memmap.h"

#pragma FUNC_ALWAYS_INLINE(Stepper_PrepareStepSettings);
#pragma FUNC_ALWAYS_INLINE(Stepper_GetNextStepPeriod);
#pragma FUNC_ALWAYS_INLINE(Stepper_InitStepSegment);

uint8_t StepperDebug = 0;
unsigned char AxisName[2] = {'X', 'Y'};
uint8_t CheckLimits = CHECK_X_LIMIT | CHECK_Y_LIMIT;
uint8_t ErrorStatus = 0;
uint32_t HomeYStepPeriodFast;
uint32_t HomeYStepPeriodSlow;
uint32_t HomeXStepPeriodFast;
uint32_t HomeXStepPeriodSlow;
uint32_t StepPeriod;
uint32_t StepsToRun = 0;
uint8_t StepsRunning = 0;
uint32_t StepPulseWidth = 1000;
uint8_t StepPulseMask = 0;
uint32_t DrivePort;
uint32_t DriveBit;

//Acceleration variables
int32_t AccelerationDistance;
int32_t AccelerationTimeuS;
int32_t XAccelerationTime = 200000;//Time to accelerate in us
uint32_t DistanceSqrt;
uint32_t DistanceSqrtLast;
float Acceleration;
int8_t AccelDecel = 1;//1 = accelerate, -1 = decellerate
float XAcceleration;
int32_t XStepTracker = 0;
uint8_t DirectionSet;

float Stepper_CalcAccelleration(int32_t RampDistance, int32_t RampTimeuS)
{
	return (2.0 * (float)RampDistance / ((float)RampTimeuS * (float)RampTimeuS));
}

void Stepper_InitStepSegment(int32_t Period, int32_t RampDistance, int32_t RampTimeuS, int8_t UpDown)
{
	Acceleration = Stepper_CalcAccelleration(RampDistance, RampTimeuS);
	if (UpDown > 0)
	{
		StepPeriod = Period;
		AccelDecel = 1;
		AccelerationDistance = 0;
		AccelerationTimeuS = 0;
		DistanceSqrtLast = 1;
	}
	else if (UpDown < 0)
	{
		//Decelerating so leave current period alone since will increase during motion
		AccelDecel = -1;
		AccelerationDistance = RampDistance;
	}
	else
	{
		StepPeriod = Period;
	}
}

int32_t Stepper_GetNextStepPeriod()
{
	uint32_t PeriodClocks;
	uint32_t DeltaTime;
	uint32_t AccelerationTime;
	static uint32_t LastTime = 0;

	if (Acceleration == 0)
	{
		PeriodClocks = StepPeriod;
	}
	else
	{
		//Move to next tick location
		AccelerationDistance = AccelerationDistance + AccelDecel;
		//Calculate the total time to go this distance
		AccelerationTime = SQRT((2 * AccelerationDistance) / Acceleration);

		DeltaTime = AccelerationTime - LastTime;

		PeriodClocks = DeltaTime;

		if (PeriodClocks < XMinStepClocks)
			PeriodClocks = XMinStepClocks;
		else if (PeriodClocks > XMaxStepClocks)
			PeriodClocks = XMaxStepClocks;

		if (StepperDebug == 1)
			printf("%d\t%d\t%d\t%d\n", AccelerationDistance, LastTime, AccelerationTime, DeltaTime);

		//Note the last distance time
		LastTime = AccelerationTime;

	}
	return PeriodClocks;
}

int32_t Stepper_GetNextStepPeriod_old()
{
	int32_t PeriodClocks;
	int32_t Test = 8;

	float Factor = XAcceleration * 80000/(256 * Test);

	if (Acceleration == 0)
	{
		PeriodClocks = StepPeriod;
	}
	else
	{
		AccelerationDistance = AccelerationDistance + AccelDecel;
//		DistanceSqrt = (float)SQRT((float)AccelerationDistance * 65536.0)/256.0;//This limits the distance to about 16 inches at ~2000 ticks per inch
//		PeriodClocks = (float)Acceleration * (DistanceSqrt - DistanceSqrtLast) * 80000000/1000;//Combine this into "acceleration" later
		DistanceSqrt = SQRT(AccelerationDistance * 65536 * Test);//This limits the distance to about 16 inches at ~2000 ticks per inch
		if (AccelDecel == 1)
		{
			PeriodClocks = (int)(Factor * (DistanceSqrt - DistanceSqrtLast));

			if (StepPeriod < PeriodClocks)//Needed due to sqrt accuracy issues when deltas get small
				PeriodClocks = StepPeriod;
		}
		else
		{
			PeriodClocks = (int)-(Factor * (DistanceSqrt - DistanceSqrtLast));

			if (StepPeriod > PeriodClocks)//Needed due to sqrt accuracy issues when deltas get small
				PeriodClocks = StepPeriod;
		}

		if (PeriodClocks < XMinStepClocks)
			PeriodClocks = XMinStepClocks;
		else if (PeriodClocks > XMaxStepClocks)
			PeriodClocks = XMaxStepClocks;

		if (StepperDebug == 1)
			printf("%d\t%d\t%d\t%d\t%d\n", AccelerationDistance, PeriodClocks, DistanceSqrt, DistanceSqrtLast, (DistanceSqrt - DistanceSqrtLast));

		DistanceSqrtLast = DistanceSqrt;
	}
	return PeriodClocks;
}

void Stepper_AccelerationTest()
{
	Stepper_InitStepSegment(XAcceleration, XMinStepClocks, XMaxStepClocks, XNomStepClocks);
	Stepper_MoveSteps(1000, MOVE_X_MOTOR);

	Stepper_InitStepSegment(-XAcceleration, XMinStepClocks, XMaxStepClocks, XNomStepClocks);
	Stepper_MoveSteps(-1000, MOVE_X_MOTOR);

}

void Stepper_PrepareStepSettings(void)
{
	//Check the end stops and calculate stepping period ready to fire interrupt
	if (StepsToRun != 0)
	{
		//Decrement steps. Should probably move to StepperPulseInterrupt since currently it is possible for a new segment to start before the second half of this pulse finishes
		StepsToRun--;
		//Check the limit switches if enabled (i.e. heading towards a switch)
		if ((ROM_GPIOPinRead(XLIMIT_SWITCH_PORT, XLIMIT_SWITCH_BIT) != 0) && ((CheckLimits & CHECK_X_LIMIT) != 0))
		{
			//An end stop that is active has been hit so flag error and stop moving. Don't change the state of the current location
			ErrorStatus |= ENDSTOP_ERROR1;
		}
		else if ((ROM_GPIOPinRead(YLIMIT_SWITCH_PORT, YLIMIT_SWITCH_BIT) != 0) && ((CheckLimits & CHECK_Y_LIMIT) != 0))
		{
			//An end stop that is active has been hit so flag error and stop moving. Don't change the state of the current location
			ErrorStatus |= ENDSTOP_ERROR2;
		}
		else
		{
			//In the simple PCB printer the step direction and enables are set in the printing loops and not in the interrupts as in JASMIN
			StepPeriod = Stepper_GetNextStepPeriod();
		}
	}
	else
	{
		//No steps (left) to run. Should probably move to StepperPulseInterrupt since currently it is possible for a new segment to start before the second half of this pulse finishes
		StepsRunning = 0;
		//Disable interrupts so no erroneous interrupts
		TimerDisable(TIMER0_BASE, TIMER_A);
	}
	if (ErrorStatus != 0)
	{
		//Stop motion
		StepsToRun = 0;
		StepsRunning = 0;
		//Disable interrupts so no erroneous interrupts
		TimerDisable(TIMER0_BASE, TIMER_A);
	}
}

/////////////////////////////////////////////////
// Interrupt handler for stepper pulse
// Handles all position tracking and end stops
/////////////////////////////////////////////////
void Stepper_StepperInterrupt(void)
{

	//Clear the interrupt flag
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	//Arm the second timer to create the step pulse
	TimerLoadSet(TIMER1_BASE, TIMER_A, StepPeriod >> 1);

	//Pulse the steppers high. Low is cleared in next delayed interrupt
	GPIOPinWrite(DrivePort, DriveBit, SETHIGH);

	if (DriveBit == 2)
		if (DirectionSet == 0)
			XStepTracker++;
		else
			XStepTracker--;

	//And load in to timer
	//Might need to either disable the timer first or switch to a cascaded load/enable architecture
	TimerLoadSet(TIMER0_BASE, TIMER_A, StepPeriod);
	//Trigger clear timer interrupt
	TimerEnable(TIMER1_BASE, TIMER_A);

	//Set the laser diode as necessary for this pulse
	if ((LaserEnable == 1) && (PrintingActive == 1))
		GPIOPinWrite(LASER_PORT, LASER_BIT, GetNextLaserBit());
	else
		GPIOPinWrite(LASER_PORT, LASER_BIT, SETLOW);

	//Now calculate the next period settings
	Stepper_PrepareStepSettings();
}

/////////////////////////////////////////////////
// Interrupt handler for stepper pulse width
/////////////////////////////////////////////////
void Stepper_StepperPulseInterrupt(void)
{
	//This interrupt is initiated by the stepper interrupt and allows precise pulse width control
	//Clear the pulse
	GPIOPinWrite(DrivePort, DriveBit, SETLOW);
	//Clear the interrupt flag
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	//Disable the timer (not sure if necessary)
	TimerDisable(TIMER1_BASE, TIMER_A);
}

void Stepper_MoveSteps(int32_t StepCount, uint8_t MotorToMove)
{
	uint8_t DirectionSet;

	if (StepCount <0)
		DirectionSet = SETHIGH;
	else
		DirectionSet = SETLOW;

	StepsRunning = 1;
	StepsToRun = abs(StepCount);
	if (MotorToMove == MOVE_Y_MOTOR)
	{
		GPIOPinWrite(Y_DIR_PORT, Y_DIR_BIT, DirectionSet);//0 = towards switch
		StepPulseMask = Y_STEP_BIT;
		DrivePort = Y_STEP_PORT;
		DriveBit = Y_STEP_BIT;
	}
	else
	{
		GPIOPinWrite(X_DIR_PORT, X_DIR_BIT, DirectionSet);//0 = towards switch
		StepPulseMask = X_STEP_BIT;
		DrivePort = X_STEP_PORT;
		DriveBit = X_STEP_BIT;
	}
	Stepper_PrepareStepSettings();
	//If end stops were not hit then do motion until finished with movement
	if (StepsRunning != 0)
	{
		//Enable the step interrupts
		TimerLoadSet(TIMER0_BASE, TIMER_A, StepPeriod);
		//Trigger interrupt processing
		//Enable the stepper interrupt to execute the move
		TimerEnable(TIMER0_BASE, TIMER_A);
		//And wait until done
		while(StepsRunning != 0);
	}
}

void Stepper_EnableSteppers(uint8_t State)
{
	GPIOPinWrite(STEP_ENABLE_PORT, STEP_ENABLE_BIT, (State & 1) * 255);
}

void Stepper_Home(uint32_t PeriodFast, uint32_t PeriodSlow, uint8_t LimitCheck, uint8_t MotorToMove, uint16_t TicksPermm)
{
	//Homeing is 3 step procedure
	//1) Move in fairly fast
	//2) Move out some
	//3) Move in much slower

	uint8_t Axis;

	Axis = AxisName[MotorToMove];
	sprintf((char*)MiscBuffer, "HOME - %c\n\n", Axis);
	LCD_Print((uint8_t*)MiscBuffer, 0);
	Stepper_EnableSteppers(STEPPERS_ON);
//	LCD_Print((uint8_t*)"Steppers enabled\n", 0);

	//No acceleration for homing
	Acceleration = 0;

	//Home towards switch
	StepPeriod = PeriodFast;
	CheckLimits = LimitCheck;
	ErrorStatus = 0;
	LCD_Print((uint8_t*)"Seeking endstop\n", 0);
//	Stepper_MoveSteps(-1, MotorToMove, MOVE_POSITIVE);
	Stepper_MoveSteps(INT_MAX, MotorToMove);
	WaitFor(100);

	//Move away a little now
	CheckLimits = 0;
	ErrorStatus = 0;
//	LCD_Print((uint8_t*)"Backing off\n", 0);
	Stepper_MoveSteps(-TicksPermm * 5, MotorToMove);
	WaitFor(100);

	//Now move back slowly
	StepPeriod = PeriodSlow;
	CheckLimits = LimitCheck;
	ErrorStatus = 0;
//	LCD_Print((uint8_t*)"Re-seeking endstop\n", 0);
//	Stepper_MoveSteps(-1, MotorToMove, MOVE_POSITIVE);
	Stepper_MoveSteps(INT_MAX, MotorToMove);
	LCD_Print((uint8_t*)"Endstop located\n", 0);
	WaitFor(100);

	//Move away a little now
	CheckLimits = 0;
	ErrorStatus = 0;
//	LCD_Print((uint8_t*)"Setting home\n", 0);
	Stepper_MoveSteps(-TicksPermm * 5, MotorToMove);
	CurrentYPosition = 0;
	LCD_Print((uint8_t*)"Home set\n", 0);
}

void Stepper_ResetSteppers(void)
{
	GPIOPinWrite(STEP_RESET_PORT, STEP_RESET_BIT, SETLOW);
	WaitFor(200);
	GPIOPinWrite(STEP_RESET_PORT, STEP_RESET_BIT, SETHIGH);
}

void Stepper_LoopMove(uint32_t Motor, uint32_t MaxTravel, uint32_t Period)
{
	uint32_t Count = 0;

	StepPeriod = 3000;//Period;

	Stepper_EnableSteppers(STEPPERS_ON);

	//No acceleration for loop test at the moment
	Acceleration = 0;

	while(1)
	{
		CheckLimits = CHECK_X_LIMIT | CHECK_Y_LIMIT;
		Stepper_MoveSteps(-MaxTravel, Motor);
		WaitFor(100);
		Stepper_MoveSteps(MaxTravel, Motor);
		sprintf((char*)MiscBuffer, "Scan count %d    ", Count++);
		LCD_FontString(0, 100, MiscBuffer, 0);
		WaitFor(100);
	}

}

void Stepper_RunInY()
{
	uint16_t CyclesToRun;

	//Home the X carriage first

	Stepper_Home(HomeXStepPeriodFast, HomeXStepPeriodSlow, CHECK_X_LIMIT, MOVE_X_MOTOR, XTicksPermm);

	//Y home
	Stepper_Home(HomeYStepPeriodFast, HomeYStepPeriodSlow, CHECK_Y_LIMIT, MOVE_Y_MOTOR, YTicksPermm);

	CyclesToRun = 100;
	StepPeriod = HomeYStepPeriodFast;
	LCD_Clear(WHITE);
	POINT_COLOR = GREEN;
	while((CyclesToRun != 0) & (CommandCount == 0))
	{
		//Now move back and forth
		sprintf((char*)MiscBuffer, "Cycles remaining %d    ", CyclesToRun);
		LCD_FontString(0, 0, MiscBuffer, 0);
		Stepper_MoveSteps(-YTicksPermm * 185, MOVE_Y_MOTOR);
		WaitFor(50);
		Stepper_MoveSteps(YTicksPermm * 185, MOVE_Y_MOTOR);
		WaitFor(50);
		CyclesToRun--;
	}
}
