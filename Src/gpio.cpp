/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
#include "main.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
 	
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE0);
  
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE1);
	
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE2);
	
  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE3);

  LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTB, LL_SYSCFG_EXTI_LINE4);
  
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_7, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_7, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_0, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_0, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_1, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_VERY_HIGH);
	
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT); //OLED Reset
  LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_5, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_VERY_HIGH);

	
	
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_0, LL_GPIO_PULL_NO);
	
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_1, LL_GPIO_PULL_NO);
	
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_NO);
	
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_NO);

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_4, LL_GPIO_PULL_NO);
	
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_NO);
	
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_3, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_3, LL_GPIO_PULL_NO);
	
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_INPUT);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_4, LL_GPIO_PULL_NO);



  LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0 | LL_EXTI_LINE_1 | LL_EXTI_LINE_2 | LL_EXTI_LINE_3 | LL_EXTI_LINE_4);
  LL_EXTI_DisableEvent_0_31(LL_EXTI_LINE_0 | LL_EXTI_LINE_1 | LL_EXTI_LINE_2 | LL_EXTI_LINE_3 | LL_EXTI_LINE_4);
  LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0 | LL_EXTI_LINE_1 | LL_EXTI_LINE_2 | LL_EXTI_LINE_3 | LL_EXTI_LINE_4);
  LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_0 | LL_EXTI_LINE_1 | LL_EXTI_LINE_2 | LL_EXTI_LINE_3 | LL_EXTI_LINE_4);
  /* EXTI interrupt init*/
	
  NVIC_EnableIRQ(EXTI0_1_IRQn);
  NVIC_EnableIRQ(EXTI2_3_IRQn);
  NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
