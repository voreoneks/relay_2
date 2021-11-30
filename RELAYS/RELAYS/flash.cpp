#include "flash.h"
#include "stm32l0xx_ll_system.h"

void FlashWrite(uint32_t address, uint8_t *data, uint32_t size)
{
	while (FLASH->SR & FLASH_SR_BSY);
	
	if (FLASH->PECR & FLASH_PECR_PELOCK)
	{
		FLASH->PEKEYR = 0x89ABCDEF;
		FLASH->PEKEYR = 0x02030405;
	}
	
	for (uint32_t i = 0; i < size; i++)
	{
		*(uint8_t *)(address + i) = data[i];
	}
	
	while (FLASH->SR & FLASH_SR_BSY);
	
	FLASH->PECR |= FLASH_PECR_PELOCK;
}

void FlashRead(uint32_t address, uint8_t *data, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++)
	{
		data[i] = *(uint8_t *)(address + i); 
	}
}