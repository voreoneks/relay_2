#pragma once 

#include "stm32l0xx.h"
#include <stdint.h>

typedef struct 
{
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;			/*!< Font height in pixels */
	const uint16_t *data;		/*!< Pointer to data font data array */
} FontDef_16Bit;

typedef struct {
	const unsigned char FontWidth;
	const unsigned char FontHeight;
	const unsigned char *fontEn;
	const unsigned char *fontRu;
} FontDef_8Bit;

extern FontDef_8Bit Font_7x9;
extern FontDef_16Bit Font_16x26;


