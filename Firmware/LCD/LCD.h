#ifndef __LCD_H
#define __LCD_H		

#include <stdint.h>

#define LCD_W 320
#define LCD_H 240

#define LCD_COMMAND        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, 0);
#define LCD_DATA           GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_7, GPIO_PIN_7);
#define LCD_CS_ACTIVE      GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0);
#define LCD_CS_INACTIVE    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3);
#define LCD_CLK_HIGH       GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2);
#define LCD_CLK_LOW        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0);
#define LCD_DATA_HIGH      GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, GPIO_PIN_5);
#define LCD_DATA_LOW       GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5, 0);

#define LCD_SPISpeed 40000000

extern uint16_t BACK_COLOR;
extern uint16_t POINT_COLOR;

extern uint16_t CursorX;
extern uint16_t CursorY;

void Lcd_Init(void); 
void LCD_Clear(uint16_t Color);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char da);
uint8_t LCD_WRRD_DATA8(char da);
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);
void LCD_CS_DISABLE(void);
void LCD_CS_ENABLE(void);

//Generic simple graphics routines
void LCD_DrawPoint(uint16_t x,uint16_t y);
void LCD_DrawPoint_big(uint16_t x,uint16_t y);
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y);
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Pattern);
void LCD_DrawLineFast(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Pattern);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t pattern);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_FillPRNG(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color, uint16_t key);
void LCD_DrawCross(uint16_t x, uint16_t y, uint16_t Color);
void LCD_SoftScroll(uint8_t Height);
void LCD_GetChunk(uint8_t* pBuffer, uint16_t Size);
void LCD_SetChunk(uint8_t* pBuffer, uint16_t Size);
void LCD_Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_SetDataLine(uint8_t Line, uint8_t *Buffer);

//Low resolution font routines
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len);
void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p);

//High quality font routines
void LCD_FontCharacter(uint16_t x,uint16_t y,uint8_t character,uint8_t mode);
void LCD_FontString(uint16_t x,uint16_t y,const uint8_t *p,uint8_t mode);
void LCD_SetFont(uint8_t FontIndex);
void LCD_Print(uint8_t *p, uint8_t mode);
uint16_t LCD_StringHeight(void);
uint16_t LCD_StringWidth(const uint8_t *p);
void LCD_ClearString(uint16_t x,uint16_t y,const uint8_t *p);
uint16_t LCD_TextCenterOffset(uint16_t FWidth, const uint8_t *p);
void LCD_FontStringCentered(uint16_t y,const uint8_t *p,uint8_t mode);
void LCD_PrintFloat(float Value, uint8_t Mode);

//Bitmap image display (compressed and uncompressed)
void LCD_DisplayImage(uint16_t X, uint16_t Y, uint16_t* Image16, uint8_t Mode);
uint16_t LCD_DisplayFrame(uint16_t X, uint16_t Y, uint16_t FWidth, uint16_t FHeight, uint16_t* Image16);
void LCD_DisplayButton(uint16_t X, uint16_t Y, uint16_t FWidth, uint16_t FHeight, uint16_t* Image16, uint8_t *p, uint8_t Mode);
void LCD_DisplaySlider(uint16_t X, uint16_t Y, uint16_t FWidth, uint16_t FHeight, uint16_t* BaseImage16, uint16_t* SliderImage16, uint8_t *p, uint8_t Mode, int16_t Min, int16_t Max, int16_t Val);
int32_t LCD_GetSliderValue(uint16_t TSX, uint16_t X, uint16_t* SliderImage16, uint16_t FWidth, uint16_t Min, uint16_t Max);

void showhanzi(unsigned int x,unsigned int y,unsigned char index);

#define WHITE    	 0xFFFF
#define BLACK    	 0x0000	  
#define BLUE     	 0x001F  
#define RED        0xF800
#define MAGENTA    0xF81F
#define GREEN      0x07E0
#define CYAN       0x7FFF
#define YELLOW     0xFFE0
#define BROWN 		 0XBC40
#define BRRED 		 0XFC07
#define GRAY  		 0X8430

#define DARKBLUE   0X01CF
#define LIGHTBLUE  0X7D7C
#define GRAYBLUE   0X5458

#define DARKGREEN  0X0600
#define LGRAY 		 0XC618

#define LGRAYBLUE  0XA651
#define LBBLUE     0X2B12

#define SOLID_LINE  0xffff
#define DOTTED_LINE 0xAAAA
#define LIGHT_LINE  0x4444
					  		 
#endif  
	 
	 



