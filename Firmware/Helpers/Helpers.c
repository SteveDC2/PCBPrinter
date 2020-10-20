#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "driverlib/rom.h"
#include "Bitmaps.h"
#include "LCD.h"
#include "TSC.h"
#include "Main.h"
#include "Helpers.h"
#include "math.h"

#pragma FUNC_ALWAYS_INLINE(SquareRoot);
#pragma FUNC_ALWAYS_INLINE(SquareRootRounded);
#pragma FUNC_ALWAYS_INLINE(fisqrt);
#pragma FUNC_ALWAYS_INLINE(int_sqrt32);

volatile uint32_t DelayCounter = 0;
volatile uint32_t TimeTick = 0;
volatile uint16_t ResetTimeTick = 0;

#define YESBUTTON 10 ,195, 120, 35
#define NOBUTTON 190,195, 120, 35


uint32_t SquareRoot(uint32_t a_nInput)
{
    uint32_t op  = a_nInput;
    uint32_t res = 0;
    uint32_t one = 1uL << 30; // The second-to-top bit is set: use 1u << 14 for uint16_t type; use 1uL<<30 for uint32_t type


    // "one" starts at the highest power of four <= than the argument.
    while (one > op)
    {
        one >>= 2;
    }

    while (one != 0)
    {
        if (op >= res + one)
        {
            op = op - (res + one);
            res = res +  2 * one;
        }
        res >>= 1;
        one >>= 2;
    }
    return res;
}

uint32_t SquareRootRounded(uint32_t a_nInput)
{
    uint32_t op  = a_nInput;
    uint32_t res = 0;
    uint32_t one = 1uL << 30; // The second-to-top bit is set: use 1u << 14 for uint16_t type; use 1uL<<30 for uint32_t type


    // "one" starts at the highest power of four <= than the argument.
    while (one > op)
    {
        one >>= 2;
    }

    while (one != 0)
    {
        if (op >= res + one)
        {
            op = op - (res + one);
            res = res +  2 * one;
        }
        res >>= 1;
        one >>= 2;
    }

    /* Do arithmetic rounding to nearest integer */
    if (op > res)
    {
        res++;
    }

    return res;
}

uint32_t ftbl[33]={0,1,1,2,2,4,5,8,11,16,22,32,45,64,90,128,181,256,362,512,724,1024,1448,2048,2896,4096,5792,8192,11585,16384,23170,32768,46340};
uint32_t ftbl2[32]={ 32768,33276,33776,34269,34755,35235,35708,36174,36635,37090,37540,37984,38423,38858,39287,39712,40132,40548,40960,41367,41771,42170,42566,42959,43347,43733,44115,44493,44869,45241,45611,45977};

uint32_t fisqrt(uint32_t val)
{
	uint32_t cnt=0;
	uint32_t t=val;
    while (t) {cnt++;t>>=1;}
    if (6>=cnt)    t=(val<<(6-cnt));
    else           t=(val>>(cnt-6));

    return (ftbl[cnt]*ftbl2[t&31])>>15;
}

uint32_t int_sqrt32(uint32_t x)
{
    uint32_t res=0;
    uint32_t add= 0x8000;
    int i;
    for(i=0;i<16;i++)
    {
        uint32_t temp=res | add;
        uint32_t g2=temp*temp;
        if (x>=g2)
        {
            res=temp;
        }
        add>>=1;
    }
    return res;
}


void ResetTime()
{
	ResetTimeTick = 1;
	while(ResetTimeTick == 1);
}

void WaitFor(unsigned int Delayms)
{
	DelayCounter = Delayms;
	while(DelayCounter != 0);
}

uint8_t InRegion(uint16_t CheckX, uint16_t CheckY, uint16_t X ,uint16_t Y, uint16_t W, uint16_t H)
{
	if ((CheckX >= X) && (CheckX <= X + W - 1) && (CheckY >= Y) && (CheckY <= Y + H - 1))
		return 1;
	else 
		return 0;
}

uint8_t InRange(uint16_t Target, uint16_t Value, uint16_t Tolerance)
{
	if (abs(Target - Value) <= Tolerance)
		return 0;
	else
		return 1;
}

uint8_t VerifyAction(uint8_t *Message)
{
	//Display warning and make sure really want to xxxxx
	LCD_Clear(BLACK);
	LCD_DisplayImage((LCD_W / 2) - ((uint16_t)StopImage[0] / 2), 0, (uint16_t*)StopImage, 0);
	
	BACK_COLOR = BLACK;
	BACK_COLOR = LCD_DisplayFrame(0, 137, LCD_W, LCD_H - 137, (uint16_t*)BlueBlankSmall);
	
	LCD_FontStringCentered(145,(const uint8_t*)"Are you sure you want to", 1);
	LCD_FontStringCentered(168,(const uint8_t*)Message, 1);

	
	LCD_DisplayButton(YESBUTTON, (uint16_t*)GreenBlankSmall, (uint8_t*)"Yes", 1);
	LCD_DisplayButton(NOBUTTON, (uint16_t*)GreenBlankSmall, (uint8_t*)"No", 1);
	
	while (1)
	{
		while (!TSC_UpdateCoordinates());//Wait for touch and update coordinates

		if (InRegion(TSC_X, TSC_Y, YESBUTTON))
			return 1;
		else if (InRegion(TSC_X, TSC_Y, NOBUTTON))
			return 0;
	}
}
