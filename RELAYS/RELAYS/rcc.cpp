#include "rcc.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_pwr.h"

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config()
{
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
	while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1);
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
	LL_RCC_HSI_Enable();

	/* Wait till HSI is ready */
	while (LL_RCC_HSI_IsReady() != 1);
	LL_RCC_HSI_SetCalibTrimming(16);
	
	LL_RCC_LSI_Enable();
	while (LL_RCC_LSI_IsReady() != 1)
	{

	}
	LL_PWR_EnableBkUpAccess();
	LL_RCC_ForceBackupDomainReset();
	LL_RCC_ReleaseBackupDomainReset();
	LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
	LL_RCC_EnableRTC();
	
	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_4, LL_RCC_PLL_DIV_2);
	LL_RCC_PLL_Enable();

	/* Wait till PLL is ready */
	while (LL_RCC_PLL_IsReady() != 1);
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	/* Wait till System clock is ready */
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);
	SysTick_Config(32000);
}
