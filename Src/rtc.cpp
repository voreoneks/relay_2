/**
  ******************************************************************************
  * File Name          : RTC.c
  * Description        : This file provides code for the configuration
  *                      of the RTC instances.
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
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* RTC init function */
void MX_RTC_Init(void)
{
	LL_RTC_InitTypeDef RTC_InitStruct = { 0 };
	LL_RTC_TimeTypeDef RTC_TimeStruct = { 0 };
	LL_RTC_DateTypeDef RTC_DateStruct = { 0 };
	
	LL_RTC_WaitForSynchro(RTC);

	/** Initialize RTC and set the Time and Date
	*/
	RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
	RTC_InitStruct.AsynchPrescaler = 124;
	RTC_InitStruct.SynchPrescaler = 295;
	LL_RTC_Init(RTC, &RTC_InitStruct);
	/** Initialize RTC and set the Time and Date
	*/
	RTC_TimeStruct.Hours = 4;
	RTC_TimeStruct.Minutes = 19;
	RTC_TimeStruct.Seconds = 58;
	LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_TimeStruct);
	RTC_DateStruct.WeekDay = LL_RTC_WEEKDAY_MONDAY;
	RTC_DateStruct.Month = LL_RTC_MONTH_JANUARY;
	RTC_DateStruct.Year = 0;
	LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BIN, &RTC_DateStruct);
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
