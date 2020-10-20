#ifndef __TSC_H
#define __TSC_H

#include <stdint.h>

#define TSC_CS_ACTIVE      GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0);
#define TSC_CS_INACTIVE    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, GPIO_PIN_6);

#define TOUCH_PRESSED      (GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_1) == 0)

#define TSC_SPISpeed 2500000

extern uint16_t TSC_Read(uint8_t Channel);
extern uint8_t TSC_UpdateIfTouchedRaw(void);
extern uint8_t TSC_UpdateCoordinates(void);

extern void TSC_Calibrate(void);
extern void TSC_WaitTouched(void);
extern void TSC_WaitReleased(void);
extern void TSC_WaitTouchedReleased(void);
extern void TSC_WaitAndUpdateCoordinates(void);

extern uint16_t TSC_X;
extern uint16_t TSC_Y;

#endif
