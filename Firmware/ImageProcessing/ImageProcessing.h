#ifndef IMAGEPROCESSING_IMAGEPROCESSING_H_
#define IMAGEPROCESSING_IMAGEPROCESSING_H_

#define LINEISBLANK  0
#define LINEISACTIVE 1

extern int16_t ImageIndexDirection;
extern uint8_t ImagePolarity;
extern uint8_t TestMode;
extern uint8_t UniDirection;
extern uint8_t LaserDisable;
extern uint8_t PrintingActive;
extern int32_t BackLashCorrection;
extern uint8_t MirrorEnable;
extern uint8_t FlipEnable;
extern float ImageFlipOffset;
extern uint16_t YPrintDPI;
extern uint8_t TestBacklashCorrection;
extern uint8_t MoveWhenFinished;
extern uint8_t HomeBeforePrint;

void ImageProc_GetBMPHeader(void);
void ImageProc_PrintBMP(void);
uint8_t GetNextLaserBit(void);


#endif /* IMAGEPROCESSING_IMAGEPROCESSING_H_ */
