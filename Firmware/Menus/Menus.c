#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "Main.h"
#include "Helpers.h"
#include "MENUS.h"
#include "Bitmaps.h"
#include "LCD.h"
#include "TSC.h"

//////////////////////////////////////////////////
//Menu structure
//////////////////////////////////////////////////
//
//  Top -------Display------ADC raw
//         |            |---ADC voltage
//         |            |---CC
//         |            |---Power
//         |
//         |---Scope--------CC
//         |            |---Power
//         |            |---Misc
//         |
//         |---Set----------I2C address
//         |            |---Port A power
//         |            |---Port B power
//         |            |---ADCIn1
//         |
//         |---Monitor------PD1
//         |            |---PD2
//         |            |---I2C1
//         |            |---I2C2
//         |            |---Uart
//         |
//         |---Test---------Version info
//                      |---Self test
//                      |---TSC calibration
//                      |---Resistor calibration
//                      |---USB test

/*
const char *VSYS_Setting[4] = {"Off", "5V", "12V", "20V"};

const char DisplayMenuItem1[] = {"ADC raw readings"};
const char DisplayMenuItem2[] = {"ADC voltage readings"};
const char DisplayMenuItem3[] = {"CC readings"};
const char DisplayMenuItem4[] = {"Power readings"};
const MenuItemDescriptor DisplayMenu[] = {{.MenuText = (char*)DisplayMenuItem1, MENU_TYPE_FUNCTION, &Actions_DisplayAllADC, (void*)1}, 
                                    {.MenuText = (char*)DisplayMenuItem2, MENU_TYPE_FUNCTION, &Actions_DisplayAllADC, (void*)2}, 
                                    {.MenuText = (char*)DisplayMenuItem3, MENU_TYPE_FUNCTION, (void*)-1, (void*)-1},
                                    {.MenuText = (char*)DisplayMenuItem4, MENU_TYPE_FUNCTION, (void*)-1, (void*)-1},
                                    {.MenuText = (char*)(void*)-1,        MENU_TYPE_NONE,     (void*)-1, (void*)-1}
                                   };

const char ScopeMenuItem1[] = {"CC lines"};
const char ScopeMenuItem2[] = {"Power lines"};
const char ScopeMenuItem3[] = {"Misc signals"};
const MenuItemDescriptor ScopeMenu[] =   {{.MenuText = (char*)ScopeMenuItem1, MENU_TYPE_FUNCTION, &Scope_Display, (void*)SCOPE_SOURCE_CC},
                                    {.MenuText = (char*)ScopeMenuItem2, MENU_TYPE_FUNCTION, (void*)-1, (void*)-1},
                                    {.MenuText = (char*)ScopeMenuItem3, MENU_TYPE_FUNCTION, (void*)-1, (void*)-1},
                                    {.MenuText = (char*)(void*)-1,      MENU_TYPE_NONE,     (void*)-1, (void*)-1}
                                   };
																	 
const char SetVSYS1MenuItem1[] = {"Off"};
const char SetVSYS1MenuItem2[] = {"5V"};
const char SetVSYS1MenuItem3[] = {"12V"};
const char SetVSYS1MenuItem4[] = {"20V"};
const MenuItemDescriptor SetVSYS1Menu[] = {{.MenuText = (char*)SetVSYS1MenuItem1, MENU_TYPE_FUNCTION, &Actions_SetVSYS, (void*)0x10}, 
                                     {.MenuText = (char*)SetVSYS1MenuItem2, MENU_TYPE_FUNCTION, &Actions_SetVSYS, (void*)0x11},
                                     {.MenuText = (char*)SetVSYS1MenuItem3, MENU_TYPE_FUNCTION, &Actions_SetVSYS, (void*)0x12},
                                     {.MenuText = (char*)SetVSYS1MenuItem4, MENU_TYPE_FUNCTION, &Actions_SetVSYS, (void*)0x13},
                                     {.MenuText = (char*)(void*)-1,         MENU_TYPE_NONE,     (void*)-1,        (void*)-1}
                                    };

const char SetVSYS2MenuItem1[] = {"Off"};
const char SetVSYS2MenuItem2[] = {"5V"};
const char SetVSYS2MenuItem3[] = {"12V"};
const char SetVSYS2MenuItem4[] = {"20V"};
const MenuItemDescriptor SetVSYS2Menu[] = {{.MenuText = (char*)SetVSYS2MenuItem1, MENU_TYPE_FUNCTION, &Actions_SetVSYS, (void*)0x20}, 
                                     {.MenuText = (char*)SetVSYS2MenuItem2, MENU_TYPE_FUNCTION, &Actions_SetVSYS, (void*)0x21},
                                     {.MenuText = (char*)SetVSYS2MenuItem3, MENU_TYPE_FUNCTION, &Actions_SetVSYS, (void*)0x22},
                                     {.MenuText = (char*)SetVSYS2MenuItem4, MENU_TYPE_FUNCTION, &Actions_SetVSYS, (void*)0x23},
                                     {.MenuText = (char*)(void*)-1,         MENU_TYPE_NONE,     (void*)-1,        (void*)-1}
                                    };
																	 
const char SetMenuItem1[] = {"I2C ADC"};
const char SetMenuItem2[] = {"Port A Power"};
const char SetMenuItem3[] = {"Port B Power"};
const char SetMenuItem4[] = {"ADCIn1"};
//const char SetMenuItem5[] = {"ADCIn2"};
const MenuItemDescriptor SetMenu[]      ={{.MenuText = (char*)SetMenuItem1, MENU_TYPE_FUNCTION, (void*)Actions_SetI2CADC, (void*)-1}, 
                                    {.MenuText = (char*)SetMenuItem2, MENU_TYPE_FUNCTION, (void*)Actions_SetPower, (void*)0}, 
                                    {.MenuText = (char*)SetMenuItem3, MENU_TYPE_FUNCTION, (void*)Actions_SetPower, (void*)1}, 
                                    {.MenuText = (char*)SetMenuItem4, MENU_TYPE_FUNCTION, (void*)Actions_SetADCIn, (void*)9}, 
//                                    {.MenuText = (char*)SetMenuItem5, MENU_TYPE_FUNCTION, (void*)Actions_SetADCIn, (void*)2}, 
                                    {.MenuText = (char*)(void*)-1,    MENU_TYPE_NONE,   (void*)-1,               (void*)-1}
                                   };

																		
const char SetPowerMenuItem1[] = {"Port A VSYS"};
const char SetPowerMenuItem2[] = {"Port B VSYS"};
const char SetPowerMenuItem3[] = {"Port A VPPEXT"};
const char SetPowerMenuItem4[] = {"Port B VPPEXT"};
const MenuItemDescriptor SetPowerMenu[]      ={{.MenuText = (char*)SetPowerMenuItem1, MENU_TYPE_PARENT, (void*)SetVSYS1Menu, (void*)-1}, 
                                    {.MenuText = (char*)SetPowerMenuItem2, MENU_TYPE_PARENT, (void*)SetVSYS2Menu, (void*)-1}, 
                                    {.MenuText = (char*)SetPowerMenuItem3, MENU_TYPE_FUNCTION, &Actions_SetVPPEXTMenu, (void*)0x0}, 
                                    {.MenuText = (char*)SetPowerMenuItem4, MENU_TYPE_FUNCTION, &Actions_SetVPPEXTMenu, (void*)0x1}, 
                                    {.MenuText = (char*)(void*)-1,    MENU_TYPE_NONE,     (void*)-1,              (void*)-1}
                                   };


const char MonitorMenuItem1[] = {"PD modem 1"};
const char MonitorMenuItem2[] = {"PD modem 2"};
const char MonitorMenuItem3[] = {"I2C 1"};
const char MonitorMenuItem4[] = {"I2C 2"};
const char MonitorMenuItem5[] = {"UART"};
const MenuItemDescriptor MonitorMenu[] = {{.MenuText = (char*)MonitorMenuItem1, MENU_TYPE_FUNCTION, (void*)-1, (void*)-1}, 
                                    {.MenuText = (char*)MonitorMenuItem2, MENU_TYPE_FUNCTION, (void*)-1, (void*)-1},
                                    {.MenuText = (char*)MonitorMenuItem3, MENU_TYPE_FUNCTION, (void*)-1, (void*)-1},
                                    {.MenuText = (char*)MonitorMenuItem4, MENU_TYPE_FUNCTION, (void*)-1, (void*)-1},
                                    {.MenuText = (char*)MonitorMenuItem5, MENU_TYPE_FUNCTION, (void*)-1, (void*)-1},
                                    {.MenuText = (char*)(void*)-1,        MENU_TYPE_NONE,     (void*)-1, (void*)-1}
                                   };

const char TestMenuItem1[] = {"Version info"};
const char TestMenuItem2[] = {"Run self test"};
const char TestMenuItem3[] = {"Run touch calibration"};
const char TestMenuItem4[] = {"Run Resistor calibration"};
const char TestMenuItem5[] = {"USB tests"};
const MenuItemDescriptor TestMenu[] = {{.MenuText = (char*)TestMenuItem1, MENU_TYPE_FUNCTION, &Actions_DisplayVersions, (void*)-1}, 
                                       {.MenuText = (char*)TestMenuItem2, MENU_TYPE_FUNCTION, &Diags_BoardDiagnostics, (void*)-1},
                                       {.MenuText = (char*)TestMenuItem3, MENU_TYPE_FUNCTION, &TSC_Calibrate, (void*)-1},
                                       {.MenuText = (char*)TestMenuItem4, MENU_TYPE_FUNCTION, &Diags_ResistorCalibration, (void*)-1},
                                       {.MenuText = (char*)TestMenuItem5, MENU_TYPE_FUNCTION, &Diags_TestUSB, (void*)-1},
                                       {.MenuText = (char*)(void*)-1,     MENU_TYPE_NONE,     (void*)-1,      (void*)-1}
                                      };
																	 
const char TopMenuItem1[] = {"Display"};
const char TopMenuItem2[] = {"Scope"};
const char TopMenuItem3[] = {"Set"};
const char TopMenuItem4[] = {"Monitor"};
const char TopMenuItem5[] = {"Test"};
const MenuItemDescriptor TopMenu[] = {{.MenuText = (char*)TopMenuItem1, MENU_TYPE_PARENT, (void*)DisplayMenu, (void*)-1},
                                {.MenuText = (char*)TopMenuItem2, MENU_TYPE_PARENT, (void*)ScopeMenu, (void*)-1},
                                {.MenuText = (char*)TopMenuItem3, MENU_TYPE_PARENT, (void*)SetMenu, (void*)-1},
                                {.MenuText = (char*)TopMenuItem4, MENU_TYPE_PARENT, (void*)MonitorMenu, (void*)-1},
                                {.MenuText = (char*)TopMenuItem5, MENU_TYPE_PARENT, (void*)TestMenu, (void*)-1},
                                {.MenuText = (char*)(void*)-1,    MENU_TYPE_NONE,   (void*)-1,       (void*)-1}
                               };
*/

void Menus_ShowMenu(MenuItemDescriptor *CurrentMenu)
{
	uint8_t ItemCount = 0;
	uint16_t MenuLeft = 10;
	uint16_t MenuTop = 15;
	uint16_t MenuSpacing = 40;
	uint8_t ButtonTouched;
	uint16_t TSC_X_Down_At;
	
	MenuSpacing = (LCD_H - MenuTop - MenuTop) / 5;
	
	BACK_COLOR = BLACK;
  BACK_COLOR = LCD_DisplayFrame(0, 0, LCD_W, LCD_H, (uint16_t*)BlueBlankSmall);

	POINT_COLOR = RED;
  LCD_SetFont(1);
	while(CurrentMenu[ItemCount].MenuItemType != MENU_TYPE_NONE)
	{
		LCD_DisplayButton(MenuLeft, (ItemCount * MenuSpacing) + MenuTop, LCD_W - MenuLeft - MenuLeft, MenuSpacing - 4, (uint16_t*)BlueBlankSmall, (unsigned char*)CurrentMenu[ItemCount].MenuText, 1);
		ItemCount++;
	}
	TSC_WaitTouched();
	TSC_X_Down_At = TSC_X;
	ButtonTouched = (TSC_Y - MenuTop) / MenuSpacing;
	if (ButtonTouched < ItemCount)//If valid button is touched then highlight the touched button
	{
		LCD_DisplayButton(MenuLeft, (ButtonTouched * MenuSpacing) + MenuTop, LCD_W - MenuLeft - MenuLeft, MenuSpacing - 4, (uint16_t*)GreenBlankSmall, (unsigned char*)CurrentMenu[ButtonTouched].MenuText, 1);
	}
	TSC_WaitReleased();
	
	if ((TSC_X - TSC_X_Down_At) > 40)
	{
		//Swipe right gesture to exit menu
		return;
	}
	else if (ButtonTouched < ItemCount)//If valid button is touched then process it
	{
		//Process the menu button
		if (CurrentMenu[ButtonTouched].MenuItemType == MENU_TYPE_PARENT)//This item is a parent so call the children
		{
			Menus_ShowMenu(CurrentMenu[ButtonTouched].MenuActionPointer);
		}
		else if (CurrentMenu[ButtonTouched].MenuItemType == MENU_TYPE_FUNCTION)
		{
			//This is a bottom level entry so call the corresponding function
			if (CurrentMenu[ButtonTouched].MenuActionPointer == (void*)-1)
			{
				//Function stubb so don't actually call it
			}
			else
			{
				if (CurrentMenu[ButtonTouched].Parameter == (void*)-1)
					((void(*)())CurrentMenu[ButtonTouched].MenuActionPointer)();
				else
					((void(*)(uint32_t))CurrentMenu[ButtonTouched].MenuActionPointer)((uint32_t)CurrentMenu[ButtonTouched].Parameter);
			}
		}
	}
}

