#include "main.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
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
#include "driverlib/ROM_map.h"
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
#include "USBSerial.h"
#include "Init.h"
#include "TSC.h"
#include "LCD.h"
#include "SPI.h"
#include "USBSerial.h"
#include "EEPROMUtils.h"
#include "Helpers.h"
#include "Stepper.h"
#include "ImageProcessing.h"

//*****************************************************************************
//
// Initialization
//
//*****************************************************************************
	
void ConfigureUSB(void)
	{
    //
    // Enable the UART that we will be redirecting.
    //
    ROM_SysCtlPeripheralEnable(USB_UART_PERIPH);

    //
    // Initialize the transmit and receive buffers.
    //
    USBBufferInit(&g_sTxBuffer);
    USBBufferInit(&g_sRxBuffer);

    //
    // Set the USB stack mode to Device mode with VBUS monitoring.
    //
    USBStackModeSet(0, eUSBModeForceDevice, 0);

    //
    // Pass our device information to the USB library and place the device
    // on the bus.
    //
    USBDCDCInit(0, &g_sCDCDevice);

    //
    // Enable interrupts now that the application is ready to start.
    //
    ROM_IntEnable(USB_UART_INT);
	}


void ConfigurePins(void)
	{
    //
    // Configure the required pins for USB operation.
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_GPIOPinTypeUSBAnalog(GPIO_PORTD_BASE, GPIO_PIN_5 | GPIO_PIN_4);
    //
    // Enable the GPIO ports
    //
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		
	//Unlock the GPIO inputs which are locked
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_6 | GPIO_PIN_7;  //DIR 1 & 2

//		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
//		HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= SW_N | LED_W | LED_C | LED_E  | GPIO_PIN_0;
		

	//Stepper resetb
	ROM_GPIOPinTypeGPIOOutput(STEP_RESET_PORT, STEP_RESET_BIT);
	//Stepper enable
	ROM_GPIOPinTypeGPIOOutput(STEP_ENABLE_PORT, STEP_ENABLE_BIT);
	//Stepper direction
	ROM_GPIOPinTypeGPIOOutput(Y_DIR_PORT, Y_DIR_BIT);
	ROM_GPIOPinTypeGPIOOutput(X_DIR_PORT, X_DIR_BIT);
	//Stepper step
	ROM_GPIOPinTypeGPIOOutput(Y_STEP_PORT, Y_STEP_BIT);
	ROM_GPIOPinTypeGPIOOutput(X_STEP_PORT, X_STEP_BIT);
	//Limit switches with weak pull up.
	ROM_GPIOPadConfigSet(XLIMIT_SWITCH_PORT, XLIMIT_SWITCH_BIT, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	ROM_GPIOPadConfigSet(YLIMIT_SWITCH_PORT, YLIMIT_SWITCH_BIT, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	ROM_GPIOPinTypeGPIOInput(XLIMIT_SWITCH_PORT, XLIMIT_SWITCH_BIT);
	ROM_GPIOPinTypeGPIOInput(YLIMIT_SWITCH_PORT, YLIMIT_SWITCH_BIT);
	//Laser control
	ROM_GPIOPinTypeGPIOOutput(LASER_PORT, LASER_BIT);
	//PANIC LED
	ROM_GPIOPinTypeGPIOOutput(PANIC_PORT, PANIC_BIT);

	//Set stepper reset state
	GPIOPinWrite(STEP_RESET_PORT, STEP_RESET_BIT, SETLOW);

	//Ensure laser is off
	GPIOPinWrite(LASER_PORT, LASER_BIT, SETLOW);

	//Ensure panic bit is off
	GPIOPinWrite(PANIC_PORT, PANIC_BIT, SETLOW);

}

void ConfigurePWM(void)
{
	// Set the PWM clock to the system clock.
	ROM_SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
	
	// The PWM peripheral must be enabled for use.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

	//Enable peripherals on ports D & E
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

	//Configure the pins as PWM
	ROM_GPIOPinConfigure(GPIO_PB7_M0PWM1);
	ROM_GPIOPinConfigure(GPIO_PB6_M0PWM0);
	ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);
	ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);
	ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);
	ROM_GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_6);
	ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
	ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
	ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);

	//Configure the PWM module
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
	PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

	//Set the PWM period
	ROM_PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 64000);
	ROM_PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, 64000);

	//Set the initial width
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 32000);
	ROM_PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, 32000);
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 32000);
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 32000);
	ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 32000);

	//Enable the PWM outputs
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
	ROM_PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT, true);
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
	ROM_PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);

	//Enable the PWM modules
	ROM_PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	ROM_PWMGenEnable(PWM1_BASE, PWM_GEN_1);
}



void Init_SystemInit()
	{
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    ROM_FPULazyStackingEnable();

    // Set the clocking to run from the PLL at 80MHz
//    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_INT);
	
    // Enable the system tick.
    ROM_SysTickPeriodSet(ROM_SysCtlClockGet() / SYSTICKS_PER_SECOND);
    ROM_SysTickIntEnable();
    ROM_SysTickEnable();
}
	
void ConfigureLCD(void)
	{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

		//Unlock the GPIO inputs which are locked
		HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
		HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= GPIO_PIN_0;

		//Command/data selected through PortA-7
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_7);
		//Chip select software controlled through PortA-3
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);
		
		//TSC chip select
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);
		//TSC touch interrupt
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_1);
		
    //Backlight pin type
	  ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);

#ifdef SOFTSPI
		//Soft SPI
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
		ROM_GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_4);
#else		
		//SPI is on port A so enable port A peripherals
		SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		//Configure pin muxing for SPI
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA4_SSI0RX);
    GPIOPinConfigure(GPIO_PA5_SSI0TX);
		//Configure the GPIO settings for the SSI pins
		GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_2);
    //Configure and enable the SSI port for SPI master mode.  Use SSI0,
    //system clock supply, idle clock level low and active low clock in
    //freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    //For SPI mode, you can set the polarity of the SSI clock when the SSI
    //unit is idle.  You can also configure what clock edge you want to
    //capture data on.
		SPI_SetSpeed(LCD_SPISpeed);
    //Enable the SSI0 module.
    SSIEnable(SSI0_BASE);
		//Flush the receive buffer of garbage
		while(SSIDataGetNonBlocking(SSI0_BASE, (uint32_t*)&MiscBuffer[0])) {}
#endif
	  //Weak pull up for MISO
	  ROM_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	  //Weak pull up for TSC interrupt
	  ROM_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	  //Set higher output drive for MOSI & clock
	  ROM_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
	  ROM_GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
			
	  LCD_CS_DISABLE();
		TSC_CS_INACTIVE;
	}

void ConfigureStepperTimers(void)
{
	//Initialize timer for main stepping interrupt
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	//Initialize timer for pulse generation
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
//	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC_UP);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

}

void Init_PeripheralInit(void)
	{
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		

    // Configure GPIO pins
		ConfigurePins();
		
		// Configure PWM outputs for LEDs
//		ConfigurePWM();

	    //Configure SPI for LCD
	    ConfigureLCD();
	    Lcd_Init();
	    LCD_Clear(BACK_COLOR);
		
		//Configure USB
		ConfigureUSB();
		
		//Configure timers used for stepper pulse
		ConfigureStepperTimers();

		//Initialize the EEPROM sub-system
		EEPROM_Initialize();
		
		//Read the serial number from EEPROM
		EEPROMRead((uint32_t*)&SerialNumber, EEPROMSize - sizeof(SerialNumber), sizeof(SerialNumber));		
	}


void Init_ReadEEPROMDefaults()
{
	int State;
	
	//Read the last configuration index used
	EEPROMRead((uint32_t *) &DeviceInfo, 0, sizeof(DeviceInfo));
	
	//Read settings from EEPROM and check if valid settings exist at indicated configuration
	State = EEPROM_GetSettings(&CurrentConfiguration, DeviceInfo.CurrentConfigurationIndex);
	//If the settings are not valid then try the default index 0
	if (State == -1)
	{
		DeviceInfo.CurrentConfigurationIndex = 0;
		State = EEPROM_GetSettings(&CurrentConfiguration, DeviceInfo.CurrentConfigurationIndex);
		EEPROMProgram((uint32_t *) &DeviceInfo, 0, sizeof(DeviceInfo));
	}
	//If STILL not valid then set the defaults in index 0
	if (State == -1)
	{
		//Default configuration is invalid so re-configure and store
		strncpy((char*)&CurrentConfiguration.ConfigurationName, "Default config", 16);
		
		EEPROM_StoreSettings(&CurrentConfiguration, DeviceInfo.CurrentConfigurationIndex, VALID_KEY);
		EEPROMProgram((uint32_t *) &DeviceInfo, 0, sizeof(DeviceInfo));
	}
	if (DeviceInfo.NLFormat >= LAST_FORMAT)
	{
		DeviceInfo.NLFormat = 0;
		DeviceInfo.EchoEnable = 1;
		EEPROMProgram((uint32_t *) &DeviceInfo, 0, sizeof(DeviceInfo));
	}
}	
	
void Init_SetDefaults(void)
{
	YTicksPermm = Y_TICKS_PER_MM;
	YTicksPerInch =25.4 * YTicksPermm;
	MaxYTravel = MAX_Y_MM;//Max travel in mm
	MaxYTravelTicks = YTicksPermm * MaxYTravel;
	CurrentYPosition = 0;

	XTicksPermm = X_TICKS_PER_MM;
	XTicksPerInch =25.4 * (float) XTicksPermm;
	MaxXTravel = 100;//Max travel in mm
	MaxXTravelTicks = XTicksPermm * MaxXTravel;
	CurrentXPosition = 0;

	HomeYStepPeriodFast = 10000;
	HomeYStepPeriodSlow = 30000;
	HomeXStepPeriodFast = 12000;
	HomeXStepPeriodSlow = 36000;

	XMinStepClocks = 4000;
	XMaxStepClocks = 40000;
	XNomStepClocks = 4000;//Used for actual scanning of image. If this changes then don't forget to change acceleration factor too
//	XAcceleration = 3.15;//Calculated for each segment now
	XAccelerationTime = 200000;//Acceleration time in us

	ImageDPI = 1000;
	YPrintDPI = 2540;

	USBTimeout = 3000;
}
