#pragma once

#include "stm32l0xx.h"
#include "gpio.h"
#include "i2c.h"
#include "fonts.h"
#include "utils.h"

#define SSD1306_INIT_WAIT				2
#define SSD1306_RESET_WAIT				2

#define SSD1306_WIDTH                   (uint8_t)48
#define SSD1306_HEIGHT                  (uint8_t)64

#define OLED_ADRESS                     (uint8_t)0x78
#define OLED_SETCONTRAST                (uint8_t)0x81
#define OLED_DISPLAYALLON_RESUME        (uint8_t)0xA4
#define OLED_DISPLAYALLON               (uint8_t)0xA5
#define OLED_NORMALDISPLAY              (uint8_t)0xA6
#define OLED_INVERTDISPLAY              (uint8_t)0xA7
#define OLED_DISPLAYOFF                 (uint8_t)0xAE
#define OLED_DISPLAYON                  (uint8_t)0xAF
#define OLED_SETDISPLAYOFFSET           (uint8_t)0xD3
#define OLED_SETCOMPINS                 (uint8_t)0xDA
#define OLED_SETVCOMDETECT              (uint8_t)0xDB
#define OLED_SETDISPLAYCLOCKDIV         (uint8_t)0xD5
#define OLED_SETPRECHARGE               (uint8_t)0xD9
#define OLED_SETMULTIPLEX               (uint8_t)0xA8
#define OLED_SETLOWCOLUMN               (uint8_t)0x00
#define OLED_SETHIGHCOLUMN              (uint8_t)0x10
#define OLED_SETSTARTLINE               (uint8_t)0x40
#define OLED_MEMORYMODE                 (uint8_t)0x20
#define OLED_COLUMNADDR                 (uint8_t)0x21
#define OLED_PAGEADDR                   (uint8_t)0x22
#define OLED_COMSCANINC                 (uint8_t)0xC0
#define OLED_COMSCANDEC                 (uint8_t)0xC8
#define OLED_SEGREMAP                   (uint8_t)0xA0
#define OLED_CHARGEPUMP                 (uint8_t)0x8D
#define OLED_SWITCHCAPVCC               (uint8_t)0x2
#define OLED_NOP                        (uint8_t)0xE3

typedef enum 
{
	Black = 0x00, 
	White = 0x01
} SSD1306_COLOR;

typedef struct {
	uint16_t CurrentX;
	uint16_t CurrentY;
	uint8_t Inverted;
	uint8_t Initialized;
} SSD1306_t;

void InitOLED(void);
void SSD1306_Reset(void);
void SSD1306_UpdateScreen(void);
void SSD1306_Fill(SSD1306_COLOR color);
void SSD1306_Garp(uint8_t y, uint8_t step);
void SSD1306_SetCursor(uint8_t x, uint8_t y);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
uint8_t SendDataI2C(uint8_t Adress, uint8_t Register, uint8_t *Data, uint8_t Size);

void SSD1306_WriteChar8Bit(uint8_t ch, FontDef_8Bit Font, SSD1306_COLOR color);
void SSD1306_WriteString8Bit(uint8_t x, uint8_t y, uint8_t* str, FontDef_8Bit Font, SSD1306_COLOR color);
void SSD1306_WriteNum8Bit(uint8_t x, uint8_t y, uint16_t num, uint8_t lengh, FontDef_8Bit Font, SSD1306_COLOR color);

void SSD1306_WriteChar16Bit(uint8_t ch, FontDef_16Bit Font, SSD1306_COLOR color);
void SSD1306_WriteString16Bit(uint8_t x, uint8_t y, uint8_t* str, FontDef_16Bit Font, SSD1306_COLOR color);
void SSD1306_WriteNum16Bit(uint8_t x, uint8_t y, uint16_t num, uint8_t lengh, FontDef_16Bit Font, SSD1306_COLOR color);