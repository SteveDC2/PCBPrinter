#include <string.h>
#include <stdio.h>
#include <font.h>
#include <lcd.h>
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include <HQFont.h>
#include "Main.h"
#include "Helpers.h"
#include "SPI.h"

uint16_t BACK_COLOR = BLACK;
uint16_t POINT_COLOR = WHITE;
uint16_t CursorX = 0;
uint16_t CursorY = 0;

uint8_t FontSelected = 0;
uint8_t FontSpacing = 2;

#define CharacterMode 0

void LCD_DATA_MODE()
{
	LCD_DATA;
}

void LCD_COMMAND_MODE()
{
	LCD_COMMAND;
}

void LCD_CS_ENABLE()
{
#ifdef SOFTSPI
#else
	while(SSIBusy(SSI0_BASE));//Make sure previous command finished
#endif
	LCD_CS_ACTIVE;
}

void LCD_CS_DISABLE()
{
#ifdef SOFTSPI
#else
	while(SSIBusy(SSI0_BASE));//Make sure previous command finished
#endif
	LCD_CS_INACTIVE;
}

void LCD_SetFont(uint8_t FontIndex)
{
	FontSelected = FontIndex;
}

void LCD_Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{  
   SPI_WR_Reg(0x2a);
   SPI_WR_Data8(x1>>8);
   SPI_WR_Data8(x1);
   SPI_WR_Data8(x2>>8);
   SPI_WR_Data8(x2);
  
   SPI_WR_Reg(0x2b);
   SPI_WR_Data8(y1>>8);
   SPI_WR_Data8(y1);
   SPI_WR_Data8(y2>>8);
   SPI_WR_Data8(y2);

   SPI_WR_Reg(0x2C);

}
void Lcd_Init(void)
{
  LCD_CS_ENABLE();

	//Issue a software reset
	SPI_WR_Reg(0x01);  
	
 //China init code
	
	SPI_WR_Reg(0xCB);  
    SPI_WR_Data8(0x39); 
    SPI_WR_Data8(0x2C); 
    SPI_WR_Data8(0x00); 
    SPI_WR_Data8(0x34); 
    SPI_WR_Data8(0x02); 

    SPI_WR_Reg(0xCF);  
    SPI_WR_Data8(0x00); 
    SPI_WR_Data8(0XC1); 
    SPI_WR_Data8(0X30); 
 	
	SPI_WR_Reg(0xE8);  
    SPI_WR_Data8(0x85); 
    SPI_WR_Data8(0x00); 
    SPI_WR_Data8(0x78); 
 	
	SPI_WR_Reg(0xEA);  
    SPI_WR_Data8(0x00); 
    SPI_WR_Data8(0x00); 
 	
	SPI_WR_Reg(0xED);  
    SPI_WR_Data8(0x64); 
    SPI_WR_Data8(0x03); 
    SPI_WR_Data8(0X12); 
    SPI_WR_Data8(0X81); 

    SPI_WR_Reg(0xF7);  
    SPI_WR_Data8(0x20); 
   
  //Steve changes to set 320x240 instead of 240x320
	SPI_WR_Reg(0x2a);  //height = 240
    SPI_WR_Data8(0x00); 
    SPI_WR_Data8(0x00); 
    SPI_WR_Data8(0X00); 
    SPI_WR_Data8(0Xef); 

	SPI_WR_Reg(0x2b);  //width = 320
    SPI_WR_Data8(0x00); 
    SPI_WR_Data8(0x00); 
    SPI_WR_Data8(0X01); 
    SPI_WR_Data8(0X3f); 

    SPI_WR_Reg(0xC0);    //Power control 
    SPI_WR_Data8(0x23);   //VRH[5:0] 
   
    SPI_WR_Reg(0xC1);    //Power control 
    SPI_WR_Data8(0x10);   //SAP[2:0];BT[3:0] 
 
    SPI_WR_Reg(0xC5);    //VCM control 
    SPI_WR_Data8(0x3e); //对比度调节
    SPI_WR_Data8(0x28); 
 
    SPI_WR_Reg(0xC7);    //VCM control2 
    SPI_WR_Data8(0x86);  //--
 
    SPI_WR_Reg(0x36);    // Memory Access Control 
    SPI_WR_Data8(0xe8); //

    SPI_WR_Reg(0x3A);    
    SPI_WR_Data8(0x55); 

    SPI_WR_Reg(0xB1);    
    SPI_WR_Data8(0x00);  
    SPI_WR_Data8(0x18); 
 
    SPI_WR_Reg(0xB6);    // Display Function Control 
    SPI_WR_Data8(0x08); 
    SPI_WR_Data8(0x82);
    SPI_WR_Data8(0x27);  
 	
	SPI_WR_Reg(0xF2);    // 3Gamma Function Disable 
    SPI_WR_Data8(0x00); 
 	
	SPI_WR_Reg(0x26);    //Gamma curve selected 
    SPI_WR_Data8(0x01); 
 
 	SPI_WR_Reg(0xE0);    //Set Gamma 
    SPI_WR_Data8(0x0F); 
    SPI_WR_Data8(0x31); 
    SPI_WR_Data8(0x2B); 
    SPI_WR_Data8(0x0C); 
    SPI_WR_Data8(0x0E); 
    SPI_WR_Data8(0x08); 
    SPI_WR_Data8(0x4E); 
    SPI_WR_Data8(0xF1); 
    SPI_WR_Data8(0x37); 
    SPI_WR_Data8(0x07); 
    SPI_WR_Data8(0x10); 
    SPI_WR_Data8(0x03); 
    SPI_WR_Data8(0x0E); 
    SPI_WR_Data8(0x09); 
    SPI_WR_Data8(0x00); 

    SPI_WR_Reg(0XE1);    //Set Gamma 
    SPI_WR_Data8(0x00); 
    SPI_WR_Data8(0x0E); 
    SPI_WR_Data8(0x14); 
    SPI_WR_Data8(0x03); 
    SPI_WR_Data8(0x11); 
    SPI_WR_Data8(0x07); 
    SPI_WR_Data8(0x31); 
    SPI_WR_Data8(0xC1); 
    SPI_WR_Data8(0x48); 
    SPI_WR_Data8(0x08); 
    SPI_WR_Data8(0x0F); 
    SPI_WR_Data8(0x0C); 
    SPI_WR_Data8(0x31); 
    SPI_WR_Data8(0x36); 
    SPI_WR_Data8(0x0F); 

    SPI_WR_Reg(0x11);    //Exit Sleep 
		LCD_CS_DISABLE();
    WaitFor(120); 
		
		LCD_CS_ENABLE();
    SPI_WR_Reg(0x29);    //Display on 
    SPI_WR_Reg(0x2c); 
    LCD_CS_DISABLE();
}
void LCD_Clear(uint16_t Color)
{
	uint16_t i,j;
	LCD_CS_ENABLE();
	LCD_Address_set(0,0,LCD_W-1,LCD_H-1);
	LCD_DATA_MODE();
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
				SPI_WR_Data(Color);
	    }
	  }
	LCD_CS_DISABLE();
	CursorX = 0;
	CursorY = 0;
	BACK_COLOR = Color;
}

void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_CS_ENABLE();
	LCD_Address_set(x,y,x,y);
	SPI_WR_Data(POINT_COLOR); 	    
	LCD_CS_DISABLE();
} 	 

void LCD_DrawPoint_big(uint16_t x,uint16_t y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
} 

void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{          
	uint16_t i,j; 
	LCD_CS_ENABLE();
	LCD_Address_set(xsta,ysta,xend,yend);
	for(i=ysta;i<=yend;i++)
		for(j=xsta;j<=xend;j++)
		  SPI_WR_Data(color);
	LCD_CS_DISABLE();
}  

void LCD_DrawLineFast(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Pattern)
{
	int delta_x,delta_y; 
	uint16_t Mask = 0x0001;
	uint16_t Count;
	
	delta_x=x2-x1;
	delta_y=y2-y1; 
	Count = delta_x + delta_y + 1;
	
	LCD_Address_set(x1,y1,x2,y2);
	while(Count-- != 0)
	{
		if ((Pattern & Mask) != 0)
			SPI_WR_Data(POINT_COLOR);	
		else
			SPI_WR_Data(BACK_COLOR);	
		if (Mask == 0x8000)
			Mask = 0x01;
		else
			Mask = Mask << 1;
	}
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t Pattern)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	uint16_t Mask = 0x0001;

	delta_x=x2-x1;
	delta_y=y2-y1; 
	if ((delta_x == 0) || (delta_y == 0))
		LCD_DrawLineFast(x1, y1, x2, y2, Pattern);
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)
		incx=1;
	else if(delta_x==0)
		incx=0;
	else 
	{
		incx=-1;
		delta_x=-delta_x;
	} 
	if(delta_y>0)
		incy=1; 
	else if(delta_y==0)
		incy=0;
	else
	{
		incy=-1;
		delta_y=-delta_y;
	} 
	if( delta_x>delta_y)
		distance=delta_x;
	else 
		distance=delta_y; 
	for(t=0;t<=distance+1;t++ )
	{  
		if ((Pattern & Mask) != 0)
		  LCD_DrawPoint(uRow,uCol);
		if (Mask == 0x8000)
			Mask = 0x01;
		else
			Mask = Mask << 1;
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t pattern)
{
	LCD_DrawLineFast(x1,y1,x2,y1, pattern);
	LCD_DrawLineFast(x1,y1,x1,y2, pattern);
	LCD_DrawLineFast(x1,y2,x2,y2, pattern);
	LCD_DrawLineFast(x2,y1,x2,y2, pattern);
}

void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;

		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 

void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode)
{
  uint8_t temp;
  uint8_t pos,t;
	uint16_t x0=x;
	uint16_t colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    

	num=num-' ';
	LCD_CS_ENABLE();
	LCD_Address_set(x,y,x+8-1,y+16-1);
	if(!mode)
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=asc2_1608[(uint16_t)num*16+pos];
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				SPI_WR_Data(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(uint16_t)num*16+pos];
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);
		        temp>>=1; 
		    }
		}
	}
	LCD_CS_DISABLE();
	POINT_COLOR=colortemp;	    	   	 	  
}   

uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 

void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;
	num=(uint16_t)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',CharacterMode);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,CharacterMode); 
	}
} 

void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len)
{         	
	uint8_t t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',CharacterMode); 
	}
} 

void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;}
        LCD_ShowChar(x,y,*p,CharacterMode);
        x+=8;
        p++;
    }  
}

void LCD_FontCharacter(uint16_t x,uint16_t y,uint8_t character,uint8_t mode)
{
	uint8_t BitPattern;
	uint8_t BitMask;
	uint8_t Width;
	uint8_t Height;
	uint8_t CharacterOffset;
	uint16_t DataOffset;
	uint16_t h,v;
	
	if ((character >= FontList[FontSelected]->CStart) && (character <= FontList[FontSelected]->CEnd))
	{
	  CharacterOffset = character - FontList[FontSelected]->CStart;
		Width = FontList[FontSelected]->Descriptors[CharacterOffset].Width;
		Height = FontList[FontSelected]->Descriptors[CharacterOffset].Height;
		DataOffset = FontList[FontSelected]->Descriptors[CharacterOffset].Offset;
		if (mode == 0)//Mode 0 = write all data so even background gets displayed. Faster since no need to keep setting the address
		{
			//Set window bounds to allow continuous writes
			LCD_Address_set(x,y,x+Width+FontSpacing-1,y+Height-1);
			for (v = 0; v < Height; v++)
			{
				BitPattern = FontList[FontSelected]->FontBitmaps[DataOffset++];
				BitMask = 0x80;
				for (h = 0; h < Width; h++)
				{
					if ((BitPattern & BitMask) == 0)
					{
						SPI_WR_Data(BACK_COLOR);	
					}
					else
					{
						SPI_WR_Data(POINT_COLOR);	
					}
					BitMask = BitMask >> 1;
					if (BitMask == 0)
					{
						if (h != (Width - 1))
						{
							BitMask = 0x80;
							BitPattern = FontList[FontSelected]->FontBitmaps[DataOffset++];
						}
					}
				}
				//Additional inter-character spacing
				for (h = 0; h < FontSpacing; h++)
				{
					SPI_WR_Data(BACK_COLOR);	
				}
			}
		}
		else//Mode 1 = write only active data so background not touched. Slow since need to set address for every pixel
		{
			for (v = 0; v < Height; v++)
			{
				BitPattern = FontList[FontSelected]->FontBitmaps[DataOffset++];
				BitMask = 0x80;
				for (h = 0; h < Width; h++)
				{
					if ((BitPattern & BitMask) != 0)
					{
						LCD_DrawPoint(x + h, y + v);
					}
					BitMask = BitMask >> 1;
					if (BitMask == 0)
					{
						if (h != (Width - 1))
						{
							BitMask = 0x80;
							BitPattern = FontList[FontSelected]->FontBitmaps[DataOffset++];
						}
					}
				}
			}
		}
	}
	LCD_CS_DISABLE();
}

uint16_t LCD_StringHeight(void)
{
	return FontList[FontSelected]->CHeight;
}

uint16_t LCD_StringWidth(const uint8_t *p)
{
	uint16_t Width = 0;
	uint8_t CharacterOffset;
	
  while(*p!='\0')
  {       
	    CharacterOffset = *p - FontList[FontSelected]->CStart;
      Width+=(FontList[FontSelected]->Descriptors[CharacterOffset].Width + FontSpacing);
      p++;
  }  
	return Width;
}

void LCD_ClearString(uint16_t x,uint16_t y,const uint8_t *p)
{
	uint16_t ClearWidth;
	uint16_t ClearHeight;
	
	ClearWidth = LCD_StringWidth(p);
	ClearHeight = LCD_StringHeight();
	LCD_Fill(x, y, ClearWidth, ClearHeight, BACK_COLOR);
}


void LCD_FontString(uint16_t x,uint16_t y,const uint8_t *p,uint8_t mode)
{         
	uint8_t Width;
	uint8_t CharacterOffset;
	
  while(*p!='\0')
  {       
	    CharacterOffset = *p - FontList[FontSelected]->CStart;
		  Width = FontList[FontSelected]->Descriptors[CharacterOffset].Width;
      LCD_FontCharacter(x,y,*p,mode);
      x+=(Width + FontSpacing);
      p++;
  }  
}

void LCD_Print(uint8_t *p, uint8_t mode)
{
	uint8_t Width;
	uint8_t Height;
	uint8_t CharacterOffset;

	CharacterOffset = *p - FontList[FontSelected]->CStart;
//  Height = FontList[FontSelected]->Descriptors[CharacterOffset].Height;
	Height = FontList[FontSelected]->CHeight;
  while(*p!='\0')
  {       
	    CharacterOffset = *p - FontList[FontSelected]->CStart;
		  Width = FontList[FontSelected]->Descriptors[CharacterOffset].Width;
		  if ((CursorX + Width + FontSpacing >=LCD_W) || (*p == '\n'))//If character will fall of end of line or a new line then move to next line
			{
				CursorX = 0;
				CursorY = CursorY + Height;
			}
			if (*p != '\n')
			{
				if ((CursorY + Height) > (LCD_H - 1))//Reached end of display so scroll and update cursor position
				{
					LCD_SoftScroll(Height);
					CursorY = CursorY - Height;
				}
        LCD_FontCharacter(CursorX,CursorY,*p,mode);
        CursorX+=(Width + FontSpacing);
			}
      p++;
  }  
}

uint16_t Pow2(uint8_t n)
{
    int i;
    int number = 1;

    for (i = 0; i < n; ++i)
        number *= 2;

    return(number);
}

void LCD_DisplayImage(uint16_t X, uint16_t Y, uint16_t* Image16, uint8_t Mode)
{
	uint16_t Width;
	uint16_t Height;
	uint8_t  Type;
	uint16_t  PaletteEntries;
	uint16_t ImageWords;
	uint16_t h, w, i, j;
	uint16_t *Palette;
	uint16_t *Data;
	uint16_t Count;
	uint16_t ColorIndexMask;
	uint8_t  ColorIndex;
	uint16_t Color;
	uint8_t  CountShift;
	
	Width          = Image16[0];
	Height         = Image16[1];
	Type           = Image16[2] >> 8;
	PaletteEntries = Image16[2] & 0xff;
	if (PaletteEntries == 0) //really should be 256. This way is simpler than zero indexing and I am too lazy to change anyhow :)
		PaletteEntries = 256;
	ImageWords     = Image16[3];

	ColorIndexMask = (1 << Type) - 1;
	CountShift     = Type;
	
	//Point to the palette
	Palette = Image16 + 4;
	//Point to the image data
	Data = Palette + PaletteEntries;
	if (Mode == 0)
	{
		LCD_CS_ENABLE();
		LCD_Address_set(X,Y,X + Width - 1,Y + Height - 1);
		LCD_DATA_MODE();
		if (Type == 0)
		{
			for(i=0;i<(Width * Height / 2);i++)
			{
				Color = Palette[(*Data) >> 8];
				SPI_WR_Data(Color);
				Color = Palette[(*Data) & 0xff];
				SPI_WR_Data(Color);
				Data++;
			}
		}
		else
		{
			for(i=0;i<ImageWords;i++)
			{
				ColorIndex = (*Data & ColorIndexMask);
				Color = Palette[ColorIndex];
				Count = (*Data >> CountShift);
				for(j = 0; j <= Count; j++)
				{
					SPI_WR_Data(Color);
				}
				Data++;
			}
		}
		LCD_CS_DISABLE();
	}
	else//Masked mode, color 0 is not drawn
	{
		LCD_CS_ENABLE();
		if (Type == 0)
		{
			for (h = 0; h < Height; h++)
			{
				for (w = 0; w < Width; w=w+2)
				{
					Color = Palette[(*Data) >> 8];
					if (Color != 0)
					{
						LCD_Address_set(X + w,Y + h,X + w,Y + h);
						SPI_WR_Data(Color);
					}
					Color = Palette[(*Data) & 0xff];
					if (Color != 0)
					{
						LCD_Address_set(X + w + 1,Y + h,X + w + 1,Y + h);
						SPI_WR_Data(Color);
					}
					Data++;
				}
			}
		}
		else
		{
			w = 0;
			h = 0;
			for(i=0;i<ImageWords;i++)
			{
				ColorIndex = (*Data & ColorIndexMask);
				Color = Palette[ColorIndex];
				Count = (*Data >> CountShift);
				for(j = 0; j <= Count; j++)
				{
					if (ColorIndex != 0)
					{
						LCD_Address_set(X + w,Y + h,X + w,Y + h);
						SPI_WR_Data(Color);
					}
					w++;
					if (w == Width)
					{
						w = 0;
						h++;
					}
				}
				Data++;
			}
		}
		LCD_CS_DISABLE();
	}
}

uint16_t LCD_DisplayFrame(uint16_t X, uint16_t Y, uint16_t FWidth, uint16_t FHeight, uint16_t* Image16)
{
	uint16_t Width;
	uint16_t Height;
	uint8_t  Type;
	uint16_t SegmentWidth;
	uint16_t SegmentHeight;
	uint16_t GapWidth;
	uint16_t GapHeight;
	uint8_t  PaletteEntries;
	uint16_t *Palette;
	uint16_t *DataStart;
	uint16_t *CurrentData;
	uint16_t y;
	uint16_t x;
	uint16_t Color;
	uint16_t CenterColor;

	Width          = Image16[0];
	Height         = Image16[1];
	Type           = Image16[2] >> 8;
	PaletteEntries = Image16[2] & 0xff;

  if (Type == 0)
	{
		//Point to the palette
		Palette = Image16 + 4;
		//Point to the image data
		DataStart = Palette + PaletteEntries;
		
		//Calculate the width/height of the left/right/top/bottom segments and the gap sizes
		SegmentWidth = Width >> 1;//Split half on left, half on right
		GapWidth = FWidth - Width;//Since segment must be an even number the gap is always fwidth - width
		SegmentHeight = Height >> 1;//Split half on left, half on right
		GapHeight = FHeight - Height;//Since segment must be an even number the gap is always fheight - height

		LCD_Address_set(X,Y,X + FWidth - 1,Y + FHeight - 1);

	//Top part of the frame
		CurrentData = DataStart;
		for (y = 0; y < SegmentHeight; y++)
		{
			//Upper left segment
			for (x = 0; x < SegmentWidth; x+=2)
			{
				Color = Palette[(*CurrentData) >> 8];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				Color = Palette[(*CurrentData) & 0xff];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				CurrentData++;
			}
			//Use last color for upper middle gap section
			for (x = 0; x < GapWidth; x++)
			{
				SPI_WR_Data(Color);
			}
			//Upper right segment
			for (x = 0; x < SegmentWidth; x+=2)
			{
				Color = Palette[(*CurrentData) >> 8];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				Color = Palette[(*CurrentData) & 0xff];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				CurrentData++;
			}		
		}
		
		//Middle 
		DataStart = CurrentData;//Note the start of the left, mid source
		for (y = 0; y < GapHeight; y++)
		{
			CurrentData = DataStart;//Reset back at the start of each line of the mid section
			//Left segment
			for (x = 0; x < SegmentWidth; x+=2)
			{
				Color = Palette[(*CurrentData) >> 8];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				Color = Palette[(*CurrentData) & 0xff];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				CurrentData++;
			}
			//Use last color for middle gap section
			for (x = 0; x < GapWidth; x++)
			{
				SPI_WR_Data(Color);
			}
			//Right segment
			for (x = 0; x < SegmentWidth; x+=2)
			{
				Color = Palette[(*CurrentData) >> 8];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				Color = Palette[(*CurrentData) & 0xff];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				CurrentData++;
			}		
		}
		CurrentData = DataStart;//Reset back at the start of each line of the mid section
		//Bottom 
		for (y = 0; y < SegmentHeight; y++)
		{
			//Lower left segment
			for (x = 0; x < SegmentWidth; x+=2)
			{
				Color = Palette[(*CurrentData) >> 8];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				Color = Palette[(*CurrentData) & 0xff];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				CurrentData++;
			}
			//Use last color for lower  middle gap section
			for (x = 0; x < GapWidth; x++)
			{
				SPI_WR_Data(Color);
			}
			//Lower right segment
			for (x = 0; x < SegmentWidth; x+=2)
			{
				Color = Palette[(*CurrentData) >> 8];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				Color = Palette[(*CurrentData) & 0xff];
				if (Color == 0x0000)
					Color = BACK_COLOR;
				SPI_WR_Data(Color);
				CurrentData++;
			}		
		}	
		
		//Find the middle pixel color
		x = (Height * Width / 2) + Width / 2;
		DataStart = Palette + PaletteEntries + (x / 2);
		CenterColor = Palette[*DataStart & 0xff];

		return CenterColor;
	}
	else
	{
		return 0;
	}
}

void LCD_DisplayButton(uint16_t X, uint16_t Y, uint16_t FWidth, uint16_t FHeight, uint16_t* Image16, uint8_t *p, uint8_t Mode)
{
	uint16_t TextHeight;
	uint16_t PreviousBackColor;

	PreviousBackColor = BACK_COLOR;
	//First display the button frame
	BACK_COLOR = LCD_DisplayFrame(X, Y, FWidth, FHeight, Image16);
	//Calculate where the text needs to start to be centered in the frame
	TextHeight = LCD_StringHeight();
	LCD_FontString(LCD_TextCenterOffset(FWidth, p) + X,((FHeight - TextHeight) / 2) + Y, p, Mode);
	BACK_COLOR = PreviousBackColor;
}

int32_t LCD_GetSliderValue(uint16_t TSX, uint16_t X, uint16_t* SliderImage16, uint16_t FWidth, uint16_t Min, uint16_t Max)
{
	int32_t Value;
	uint16_t SliderIconWidth;
	int32_t SliderX;
	uint16_t SliderActiveWidth;
	
	SliderIconWidth = SliderImage16[0];
	SliderActiveWidth = FWidth - SliderIconWidth;

	//Pixel within slider which is touched. Includes offset for slider marker width
	SliderX = TSX - X - (SliderIconWidth / 2);
	//Limit to bounds of slider length, correcting for marker width
	if (SliderX < 0)
		SliderX = 0;
	if (SliderX >= FWidth - SliderIconWidth)
		SliderX = FWidth - SliderIconWidth;
	
	Value = (Max - Min) * SliderX / SliderActiveWidth;
	
	return Value;
}

void LCD_DisplaySlider(uint16_t X, uint16_t Y, uint16_t FWidth, uint16_t FHeight, uint16_t* BaseImage16, uint16_t* SliderImage16, uint8_t *p, uint8_t Mode, int16_t Min, int16_t Max, int16_t Val)
{
	uint16_t SliderIconWidth;
	uint16_t SliderIconHeight;
	uint16_t SliderDisplayPosMin;
	uint16_t SliderDisplayPosMax;
	uint16_t SliderDisplayPos;
	
	LCD_DisplayButton(X, Y, FWidth, FHeight, BaseImage16, (unsigned char*)"", Mode);
	
	SliderIconWidth = SliderImage16[0];
	SliderIconHeight = SliderImage16[1];
	
	SliderDisplayPosMin = X + (SliderIconWidth / 2);
	SliderDisplayPosMax = X + FWidth - (SliderIconWidth / 2) - 1;
	
	SliderDisplayPos = X + (float) Val * (SliderDisplayPosMax - SliderDisplayPosMin) / (Max - Min);
		
//	LCD_DisplayButton(SliderDisplayPos, Y, SliderIconWidth, FHeight, SliderImage16, "", 1);
	LCD_DisplayImage(SliderDisplayPos, Y + ((FHeight - SliderIconHeight) / 2), SliderImage16, 1);
	
	LCD_FontString(LCD_TextCenterOffset(FWidth, p) + X,((FHeight - LCD_StringHeight()) / 2) + Y, p, Mode);

	
}

//For a given object width and text string calculate the x position for the string start relative to the object frame
uint16_t LCD_TextCenterOffset(uint16_t FWidth, const uint8_t *p)
{
	return (FWidth - LCD_StringWidth(p)) / 2;
}

void LCD_DrawCross(uint16_t x, uint16_t y, uint16_t Color)
{
	POINT_COLOR = Color;
	LCD_DrawLine(x - 10, y, x + 10, y, SOLID_LINE);
	LCD_DrawLine(x, y - 10, x, y + 10, SOLID_LINE);
}

void LCD_FontStringCentered(uint16_t y,const uint8_t *p,uint8_t mode)
{
  LCD_FontString((LCD_W - LCD_StringWidth(p)) / 2, y, p, mode);
}

void LCD_SoftScroll(uint8_t Height)
{
	uint8_t LinesToscroll;
	uint16_t Line;
	uint8_t* TempBuffer = (uint8_t*)&LineBuffer[0];

	LinesToscroll = LCD_H - Height;
	for (Line = 0; Line < LinesToscroll; Line ++)
	{
		LCD_Address_set(0,Line + Height,LCD_W - 1,Line + Height);//Single horizontal line at source location
		SPI_RXFlush();//Make sure all garbage RX data is flushed
		LCD_CS_ENABLE();
	  SPI_WR_Reg(0x3E);//Memory read command
		LCD_GetChunk(TempBuffer, LCD_W);
		LCD_CS_DISABLE();
		LCD_Address_set(0,Line,LCD_W - 1,Line);//Single horizontal line at destination location
	  SPI_WR_Reg(0x2C);//Memory write command
		LCD_SetChunk(TempBuffer, LCD_W);
	}
    LCD_CS_DISABLE();
	//Now clear the bottom of the screen
	LCD_Fill(0, LCD_H - 1 - Height, LCD_W - 1, LCD_H - 1, BACK_COLOR);
}

void LCD_SetDataLine(uint8_t Line, uint8_t *Buffer)
{
	LCD_Address_set(0,Line,LCD_W - 1,Line);//Single horizontal line at destination location
	SPI_WR_Reg(0x2C);//Memory write command
	LCD_SetChunk(Buffer, LCD_W);
	LCD_CS_DISABLE();

}

//void LCD_GetChunk(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t Size)
void LCD_GetChunk(uint8_t* pBuffer, uint16_t Size)
{
	uint16_t i;
	uint32_t Temp;
	uint16_t Color;
	uint16_t Red, Green, Blue;

	SSIDataPut(SSI0_BASE, 0x00);//Dummy send
	SSIDataGet(SSI0_BASE, &Temp);//Get the readback data, ignore first values returned
	SSIDataPut(SSI0_BASE, 0x00);//Dummy send
	SSIDataGet(SSI0_BASE, &Temp);//Get the readback data, ignore first values returned
	for (i = 0; i < Size; i++)
	{
		SSIDataPut(SSI0_BASE, 0x00);//Dummy send
		SSIDataGet(SSI0_BASE, &Temp);//Get the readback data. Seems to be in 24 bit mode but with strange bit allocations
		Red = (Temp & 0x7f) >> 2;
		SSIDataPut(SSI0_BASE, 0x00);//Dummy send
		SSIDataGet(SSI0_BASE, &Temp);//Get the readback data value 2
		Green = (Temp & 0x7f) >> 1;
		SSIDataPut(SSI0_BASE, 0x00);//Dummy send
		SSIDataGet(SSI0_BASE, &Temp);//Get the readback data value 3
		Blue = (Temp & 0x7f) >> 2;
		Color = (Red << 11) | (Green << 5) | Blue;
		*pBuffer++ = (uint8_t) (Color >> 8); //Store byte in the buffer
		*pBuffer++ = (uint8_t) Color & 0xff; //Store byte in the buffer
	}	
}

void LCD_SetChunk(uint8_t* pBuffer, uint16_t Size)
{
	uint16_t i;
	
	for (i = 0; i < Size; i++)
	{
		SSIDataPut(SSI0_BASE, *pBuffer++);
		SSIDataPut(SSI0_BASE, *pBuffer++);
	}
}

void LCD_PrintFloat(float Value, uint8_t Mode)
{
	sprintf((char*)&MiscBuffer, "%2.2f", Value);
	LCD_Print((uint8_t*)&MiscBuffer, Mode);
}
