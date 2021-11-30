/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @brief   Interrupt Service Routines.
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Button.h"
#include "stm32l0xx_it.h"
#include "relay.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint32_t delay = 0;
extern Button* bt;
extern uint8_t initSuccess;
extern ScreenID screen;
extern Settings set_flash[3], set_edit;
Relay relay[3] = { { GPIOA, LL_GPIO_PIN_7, 0 }, { GPIOB, LL_GPIO_PIN_0, 1 }, { GPIOB, LL_GPIO_PIN_1, 2 } };
uint64_t counter = 0;


 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void startDelay(uint32_t delay_ms)
{
	delay = delay_ms;
}


uint8_t isDelay()
{
	return delay != 0;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */
void callback10Hz();

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
extern "C" void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
extern "C" void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
extern "C" void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
extern "C" void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
extern "C" void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
extern "C" void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
extern "C" void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
extern "C" void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */
	
  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
extern "C" void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */	
	


	if (delay != 0)
		delay--;
	
	for (uint8_t i = 0; i < 2; i++)
	{
		bt[i].manage();
	}
	Button::commonManage();
	
	for (uint8_t i = 0; i < 3; i++)
	{
		relay[i].manage();
	}
	
	
	static uint32_t cnt = 0;

	if (cnt > 100)
	{
		if (screen == STATUS && initSuccess == 1)
			callback10Hz();
		cnt = 0;
	}
	else
		cnt++;
	

  /* USER CODE END SysTick_IRQn 0 */
  
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
 }

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles RCC global interrupt.
  */
extern "C" void RCC_IRQHandler(void)
{
  /* USER CODE BEGIN RCC_IRQn 0 */

  /* USER CODE END RCC_IRQn 0 */
  /* USER CODE BEGIN RCC_IRQn 1 */

  /* USER CODE END RCC_IRQn 1 */
}

extern "C" void EXTI0_1_IRQHandler(void)
{
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
	{
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
		
		if (!relay[0].relayAction)
			relay[0].relayStart = true;
		
		if (!relay[0].isActiveInputPin() && !relay[0].timerAction && set_flash[0].inputEdge && set_flash[0].pulseModeEnabledIn)
		{
			relay[0].relayStart = false;
			relay[0].relayAction = true;
		}
		
			
		
	}
	
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
	{
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
		
		if (!relay[1].relayAction)
			relay[1].relayStart = true;
		
	}
}

extern "C" void EXTI2_3_IRQHandler(void)
{
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
	{
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
		
		if (!relay[0].relayAction)
			relay[2].relayStart = true;
		
	}
	
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
	{
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
		/* USER CODE BEGIN LL_EXTI_LINE_3 */
		bt[0].activate();
		/* USER CODE END LL_EXTI_LINE_3 */
	}
}
extern "C" void EXTI4_15_IRQHandler(void)
{
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_4) != RESET)
	{
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_4);
	
		/* USER CODE BEGIN LL_EXTI_LINE_4 */
		bt[1].activate();
		/* USER CODE END LL_EXTI_LINE_4 */
	}
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
extern "C" void TIM2_IRQHandler(void)
{	
	
	
	if (TIM2->SR & TIM_SR_UIF)
		TIM2->SR &= ~TIM_SR_UIF;
}

extern "C" void TIM21_IRQHandler(void)
{	
	
		
	
	if (TIM21->SR & TIM_SR_UIF)
		TIM21->SR &= ~TIM_SR_UIF;
}

void RTC_IRQHandler(void)
{
	/* USER CODE BEGIN RTC_IRQn 0 */

	/* USER CODE END RTC_IRQn 0 */

	/* USER CODE BEGIN RTC_IRQn 1 */

	/* USER CODE END RTC_IRQn 1 */
}



/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
