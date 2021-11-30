#pragma once

#include "stdint.h"

void FlashWrite(uint32_t address, uint8_t *data, uint32_t size);

void FlashRead(uint32_t address, uint8_t *data, uint32_t size);