#include "SSD1306.h"
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_i2c.h"
#include "stm32l0xx_it.h"


static SSD1306_t SSD1306_struct;
static const uint8_t SSD1306_NullBuffer[40] = { };
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
static void SSD1306_WriteCommand(uint8_t command);
uint8_t i = 0;


void SSD1306_Delay(uint32_t tick)
{
	LL_mDelay(tick);
}

void InitOLED(void)
{
	SSD1306_Reset();
	
	SSD1306_WriteCommand(OLED_DISPLAYOFF);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_SETDISPLAYCLOCKDIV);
	SSD1306_WriteCommand(0x80);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_SETMULTIPLEX);
	SSD1306_WriteCommand(0x3F);  						// 0x3F 128x64
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_SETDISPLAYOFFSET);
	SSD1306_WriteCommand(0x00);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_SETSTARTLINE | 0x00);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_CHARGEPUMP);  				// We use internal charge pump
	SSD1306_WriteCommand(0x14);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_MEMORYMODE);  				// Horizontal memory mode
	SSD1306_WriteCommand(0x00);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_SEGREMAP | 0x1);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_COMSCANDEC);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_SETCOMPINS);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(0x12);  						// 128x64
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_SETCONTRAST);   			// Max contrast
	SSD1306_WriteCommand(0x10);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_SETPRECHARGE);
	SSD1306_WriteCommand(0xF1);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_SETVCOMDETECT);
	SSD1306_WriteCommand(0x40);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_DISPLAYALLON_RESUME);
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_NORMALDISPLAY);   		// Non-inverted display
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_WriteCommand(OLED_DISPLAYON);  				// Turn display back on
	SSD1306_Delay(SSD1306_INIT_WAIT);
	
	SSD1306_Fill(Black);   								// Clear screen
	SSD1306_UpdateScreen();   							// Flush buffer to screen
	
	SSD1306_struct.CurrentX = 0;  						// Set default values for screen object
	SSD1306_struct.CurrentY = 0;	
	SSD1306_struct.Initialized = 1;
}

void SSD1306_Reset(void)
{
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
	SSD1306_Delay(SSD1306_RESET_WAIT);	
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5);
	SSD1306_Delay(SSD1306_RESET_WAIT);
}

static void SSD1306_WriteCommand(uint8_t command)
{
	SendDataI2C(OLED_ADRESS, 0x00, &command, 1);
}

uint8_t SendDataI2C(uint8_t Adress, uint8_t Register, uint8_t *Data, uint8_t Size)
{
	i++;
	startDelay(200);
	while (LL_I2C_IsActiveFlag_BUSY(I2C1) && isDelay()) ;
	if (!isDelay())
		return 1;

	LL_I2C_TransmitData8(I2C1, Register);
	LL_I2C_HandleTransfer(I2C1, Adress, LL_I2C_ADDRSLAVE_7BIT, Size + 1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

	for (uint32_t i = 0; i < Size; i++)
	{
		startDelay(100);
		while (!LL_I2C_IsActiveFlag_TXIS(I2C1) && isDelay()) ;
		if (!isDelay())
			return 1;

		LL_I2C_TransmitData8(I2C1, Data[i]);
	}
	return 0;
}

void SSD1306_Fill(SSD1306_COLOR color) 
{
	for (uint32_t i = 0; i < sizeof(SSD1306_Buffer); i++)
	{
		SSD1306_Buffer[i] = (color == Black) ? 0x00 : 0xFF;
	}
}

void SSD1306_UpdateScreen(void) 
{  
	for (uint8_t i = 0; i < 8; i++) 
	{
		SSD1306_WriteCommand(0xB0 + i);
		SSD1306_WriteCommand(0x00 + 40);
		SSD1306_WriteCommand(0x10);
		
		SendDataI2C(OLED_ADRESS, 0x40, (uint8_t*)SSD1306_NullBuffer, 40);
		SendDataI2C(OLED_ADRESS, 0x40, &SSD1306_Buffer[48 * i], 88);
	}
}

void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color)
{	
	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) 
	{
		return;											// Don't write outside the buffer
	}

	if (SSD1306_struct.Inverted)						// Check if pixel should be inverted
		{
			color = (SSD1306_COLOR)!color;
		}

	if (color == White)									// Draw in the right color
		{
			SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
		} 
	else 
	{
		SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
	}
}

void SSD1306_WriteChar16Bit(uint8_t ch, FontDef_16Bit Font, SSD1306_COLOR color)
{
	uint32_t b;

	if (SSD1306_WIDTH  <= (SSD1306_struct.CurrentX - 40 + Font.FontWidth)   || 
	    SSD1306_HEIGHT <= (SSD1306_struct.CurrentY + Font.FontHeight)) {}
	else
	{
		for (uint32_t i = 0; i < Font.FontHeight; i++)
		{
			b = Font.data[(ch - 43) * Font.FontHeight + i];
			for (uint32_t j = 0; j < Font.FontWidth; j++)
			{
				if ((b << j) & 0x8000) 
				{
					SSD1306_DrawPixel(SSD1306_struct.CurrentX + j, (SSD1306_struct.CurrentY + i), (SSD1306_COLOR) color);
				} 
				else 
				{
					SSD1306_DrawPixel(SSD1306_struct.CurrentX + j, (SSD1306_struct.CurrentY + i), (SSD1306_COLOR)!color);
				}
			}
		}

		SSD1306_struct.CurrentX += Font.FontWidth;
	}
}


//void SSD1306_WriteChar16Bit(uint8_t ch, FontDef_16Bit Font, SSD1306_COLOR color)
//{
//	uint32_t b;
//
//	if (SSD1306_WIDTH  <= (SSD1306_struct.CurrentX + Font.FontWidth)   || 
//	    SSD1306_HEIGHT <= (SSD1306_struct.CurrentY + Font.FontHeight)) {}
//	else
//	{
//		for (uint32_t i = 0; i < Font.FontHeight; i++)
//		{
//			b = Font.data[(ch - 32) * Font.FontHeight + i];
//			for (uint32_t j = 0; j < Font.FontWidth; j++)
//			{
//				if ((b << j) & 0x8000) 
//				{
//					SSD1306_DrawPixel(SSD1306_struct.CurrentX + j, (SSD1306_struct.CurrentY + i), (SSD1306_COLOR) color);
//				} 
//				else 
//				{
//					SSD1306_DrawPixel(SSD1306_struct.CurrentX + j, (SSD1306_struct.CurrentY + i), (SSD1306_COLOR)!color);
//				}
//			}
//		}
//
//		SSD1306_struct.CurrentX += Font.FontWidth;
//	}
//}

void SSD1306_WriteString16Bit(uint8_t x, uint8_t y, uint8_t* str, FontDef_16Bit Font, SSD1306_COLOR color)
{
	SSD1306_SetCursor(x, y);
	
	while (*str) 
	{
		SSD1306_WriteChar16Bit(*str, Font, color);
		str++;
	}
}

void SSD1306_WriteChar8Bit(uint8_t ch, FontDef_8Bit Font, SSD1306_COLOR color)
{
	uint32_t pixel, x;
	uint32_t width = Font.FontWidth;
	uint32_t start = 0;
  
	if ((((SSD1306_struct.CurrentX + Font.FontWidth / 2) < SSD1306_WIDTH)  && ((SSD1306_struct.CurrentY + Font.FontHeight / 2) < SSD1306_HEIGHT)))
	{
		uint32_t height = Font.FontHeight;
		if ((SSD1306_struct.CurrentY + Font.FontHeight) > SSD1306_HEIGHT)
			height = SSD1306_HEIGHT - SSD1306_struct.CurrentY;

		if ((ch == '\'') || (ch == '!') || (ch == ';') || (ch == ',') || (ch == ':') || (ch == '.') || (ch == ' '))
		{
			width = 3;
			start = 2;
		}

		// write char to display buffer
		for(uint32_t y = 0 ; y < Font.FontHeight ; y++)
		{
			// get font pixel
			if(ch < 127)
				pixel = Font.fontEn[(ch - 32) * Font.FontHeight + y];
			else
				pixel = Font.fontRu[(ch - 192) * Font.FontHeight + y];
			// write pixel to display buffer
			x = width;

			if ((SSD1306_struct.CurrentX + width) > SSD1306_WIDTH)
				x = SSD1306_WIDTH - SSD1306_struct.CurrentX + 2;

			while (x--)
			{
				if ((pixel >> start) & 0x0001)
					SSD1306_DrawPixel(SSD1306_struct.CurrentX + x, (SSD1306_struct.CurrentY + y), color);
				else
					SSD1306_DrawPixel(SSD1306_struct.CurrentX + x, (SSD1306_struct.CurrentY + y), (SSD1306_COLOR)(!color));
				pixel >>= 1;
			}
		} 
	}
  
	// going to next position
	SSD1306_struct.CurrentX += width;
}

void SSD1306_WriteNum16Bit(uint8_t x, uint8_t y, uint16_t num, uint8_t lengh, FontDef_16Bit Font, SSD1306_COLOR color)
{
	uint8_t text[5] = { 0, 0, 0, 0, 0 };
	
	Int16ToString(num, text, lengh);
	SSD1306_WriteString16Bit(x, y, (uint8_t*)text, Font, color);
}

void SSD1306_WriteString8Bit(uint8_t x, uint8_t y, uint8_t* str, FontDef_8Bit Font, SSD1306_COLOR color)
{
	SSD1306_SetCursor(x, y);
	
	while (*str) 
	{
		SSD1306_WriteChar8Bit(*str, Font, color);
		str++;
	}
}

void SSD1306_WriteNum8Bit(uint8_t x, uint8_t y, uint16_t num, uint8_t lengh, FontDef_8Bit Font, SSD1306_COLOR color)
{
	uint8_t text[5] = { 0, 0, 0, 0, 0 };
	
	Int16ToString(num, text, lengh);
	SSD1306_WriteString8Bit(x, y, (uint8_t*)text, Font, color);
}

void SSD1306_Garp(uint8_t y, uint8_t step)
{ 
	static uint8_t us_line;

	for (int i = 0; i < 10; i++)
	{
		SSD1306_DrawPixel(us_line, y + i, Black);
		SSD1306_DrawPixel(us_line, y - i, Black);
	}
		
	SSD1306_DrawPixel(us_line, y, White);
	
	if (us_line < 48) us_line = us_line + step; 
	else us_line = 0;
}

void SSD1306_SetCursor(uint8_t x, uint8_t y) 
{
	SSD1306_struct.CurrentX = x;
	SSD1306_struct.CurrentY = y;
}

