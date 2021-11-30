#include "utils.h"

int8_t Int16ToString(uint16_t var16, uint8_t * str, uint8_t len)
{
	uint8_t *tmp_str_ptr = str + len - 1;
	int8_t size = (int8_t)len;
	
	while (len--)
	{
		*(tmp_str_ptr--) = (var16 % 10) + '0';
		var16 /= 10;
	}
	
	return size;
}

int8_t Int32ToString(uint32_t var32, uint8_t * str, uint8_t len)
{
	// 4294967295 max 10
	
	uint8_t *tmp_str_ptr = str + len - 1;
	int8_t size = (int8_t)len;
	
	while (len--)
	{
		*(tmp_str_ptr--) = (var32 % 10) + '0';
		var32 /= 10;
	}
	
	return size;
}
