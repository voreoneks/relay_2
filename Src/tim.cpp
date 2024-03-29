/**
  ******************************************************************************
  * File Name          : TIM.c
  * Description        : This file provides code for the configuration
  *                      of the TIM instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* TIM2 init function */
void MX_TIM2_Init(void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = { 0 };

	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

	/* TIM7 interrupt Init */
	NVIC_EnableIRQ(TIM2_IRQn);

	TIM_InitStruct.Prescaler = 31999;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 999;
	LL_TIM_Init(TIM2, &TIM_InitStruct);
	LL_TIM_DisableARRPreload(TIM2);
	LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
	LL_TIM_DisableMasterSlaveMode(TIM2);
} 

void MX_TIM21_Init(void)
{
	LL_TIM_InitTypeDef TIM_InitStruct = { 0 };

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM21);

	/* TIM7 interrupt Init */
	NVIC_EnableIRQ(TIM21_IRQn);

	TIM_InitStruct.Prescaler = 31999;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 999;
	LL_TIM_Init(TIM21, &TIM_InitStruct);
	LL_TIM_DisableARRPreload(TIM21);
	LL_TIM_SetTriggerOutput(TIM21, LL_TIM_TRGO_RESET);
	LL_TIM_DisableMasterSlaveMode(TIM21);
} 


/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
