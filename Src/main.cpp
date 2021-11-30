#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "Button.h"
#include "i2c.h"
#include "SSD1306.h"
#include "stm32l0xx_it.h"
#include "menu.h"
#include "rcc.h"
#include "flash.h"
#include "rtc.h"
#include "relay.h"
#include "string.h"

#define FLASH_START 0x08080000

uint8_t initSuccess = 0; 
void bt_defaultAction(uint32_t btNum);
void bt_longAction(uint32_t btNum);
void bt_doubleLongAction();
extern Relay relay[3];

void bt0_action()
{
	bt_defaultAction(0);
}

void bt1_action()
{
	bt_defaultAction(1);
}

void bt0_longAction()
{
	bt_longAction(0);
}

void bt1_longAction()
{
	bt_longAction(1);
}

/* Private variables ---------------------------------------------------------*/

uint32_t pickRelayNum, rtc_hours, rtc_minutes, rtc_seconds;

Button buttons[2] = { {GPIOB, 3, bt0_action, true},
                      {GPIOB, 4, bt1_action, true}};
Button* bt = buttons;
Settings set_flash[3], set_edit;
MenuItem* selectedItem = 0;
MenuItem startupItem;
ScreenID screen = STATUS;

MenuItem backButton_1(" <..", 0, 0, 0, &startupItem); 
MenuItem relayMenu(" ÐÅËÅ", &backButton_1, &pickRelayNum, 2);
MenuItem input(" ÂÕÎÄ", &relayMenu);
MenuItem output(" ÂÛÕÎÄ", &input);
MenuItem globalTime(" ÂÐÅÌß", &output);

MenuItem backButton_2(" <..", 0, 0, 0, &input); 
MenuItem inversePolarityIn(" ÈÍÂÅÐÑ", &backButton_2, &set_edit.polarityIn, 1);
MenuItem pulseModeIn(" ÈÌÏ-Ñ", &inversePolarityIn, &set_edit.pulseModeEnabledIn, 1);
MenuItem repeatModeIn(" ÏÎÂÒÎÐ", &pulseModeIn, &set_edit.inputRepetition, 999);
MenuItem inputEdge(" ÔÀÇÀ", &repeatModeIn, &set_edit.inputEdge, 1);
MenuItem pulseTimeIn(" ÂÕÂÐÅÌß", &inputEdge);

MenuItem backButton_3(" <..", 0, 0, 0, &output); 
MenuItem inversePolarityOut(" ÈÍÂÅÐÑ", &backButton_3, &set_edit.polarityOut, 1);
MenuItem pulseModeOut(" ÈÌÏ-Ñ", &inversePolarityOut, &set_edit.pulseModeEnabledOut, 1);
MenuItem repeatModeOut(" ÏÎÂÒÎÐ", &pulseModeOut, &set_edit.outputRepetition, 999);
MenuItem outputTime(" ÂÊËÐÂ", &repeatModeOut);
MenuItem outputDelay(" TD", &outputTime);
MenuItem t1val(" T1", &outputDelay);
MenuItem t2val(" T2", &t1val);
MenuItem pwm(" ØÈÌ", &t2val);

MenuItem backButton_4(" <..", 0, 0, 0, &globalTime);
MenuItem hours(" ×ÀÑÛ", &backButton_4, &rtc_hours, 23);
MenuItem minutes(" ÌÈÍÓÒÛ", &hours, &rtc_minutes, 59);
MenuItem seconds(" ÑÅÊÓÍÄÛ", &minutes, &rtc_seconds, 59);

MenuItem backButton_5(" <..", 0, 0, 0, &outputTime);
MenuItem outputTimeEnable(" ÂÊËÞ×", &backButton_5, &set_edit.outputTimeEn, 1);
MenuItem outputHours(" ×ÀÑÛ", &outputTimeEnable, &set_edit.outputHour, 23);
MenuItem outputMinutes(" ÌÈÍÓÒÛ", &outputHours, &set_edit.outputMin, 59);
MenuItem outputSeconds(" ÑÅÊÓÍÄÛ", &outputMinutes, &set_edit.outputSec, 59);

MenuItem backButton_6(" <..", 0, 0, 0, &outputDelay);
MenuItem outputDelayHours(" ×ÀÑÛ", &backButton_6, &set_edit.outputDelayH, 999);
MenuItem outputDelayMinutes(" ÌÈÍÓÒÛ", &outputDelayHours, &set_edit.outputDelayM, 59);
MenuItem outputDelaySeconds(" ÑÅÊÓÍÄÛ", &outputDelayMinutes, &set_edit.outputDelayS, 59);

MenuItem backButton_7(" <..", 0, 0, 0, &t1val);
MenuItem t1Hours(" ×ÀÑÛ", &backButton_7, &set_edit.t1Hour, 999);
MenuItem t1Minutes(" ÌÈÍÓÒÛ", &t1Hours, &set_edit.t1Min, 59);
MenuItem t1Seconds(" ÑÅÊÓÍÄÛ", &t1Minutes, &set_edit.t1Sec, 59);

MenuItem backButton_8(" <..", 0, 0, 0, &t2val);
MenuItem t2Hours(" ×ÀÑÛ", &backButton_8, &set_edit.t2Hour, 999);
MenuItem t2Minutes(" ÌÈÍÓÒÛ", &t2Hours, &set_edit.t2Min, 59);
MenuItem t2Seconds(" ÑÅÊÓÍÄÛ", &t2Minutes, &set_edit.t2Sec, 59);

MenuItem backButton_9(" <..", 0, 0, 0, &pulseTimeIn);
MenuItem InputHours(" ×ÀÑÛ", &backButton_9, &set_edit.inputHour, 999);
MenuItem InputMinutes(" ÌÈÍÓÒÛ", &InputHours, &set_edit.inputMin, 59);
MenuItem InputSeconds(" ÑÅÊÓÍÄÛ", &InputMinutes, &set_edit.inputSec, 59);

int main(void)
{
	/* MCU Configuration--------------------------------------------------------*/
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	/* RCC_IRQn interrupt configuration */
	NVIC_EnableIRQ(RCC_IRQn);

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM2_Init(); 
	MX_TIM21_Init();
	MX_RTC_Init();
	MX_I2C1_Init();

//	LL_TIM_ClearFlag_UPDATE(TIM2);
//	LL_TIM_EnableIT_UPDATE(TIM2);
	
//	firstFlashWrite();
	
	for(uint8_t i = 0 ; i < 3 ; i++)
	{
		FlashRead(FLASH_START + sizeof(set_flash[0]) * i, (uint8_t*)&set_flash[i], sizeof(set_flash[0]));
	}
	
	set_flash[2].outputDelayS = 5;
	set_flash[2].outputRepetition = 3;
	set_flash[2].pulseModeEnabledOut = 1;
	set_flash[2].t1Sec = 5;
	set_flash[2].t2Sec = 3;
	
	buttons[0].setLongPressingAction(1000, bt0_longAction);
	buttons[1].setLongPressingAction(1000, bt1_longAction);
	Button::setInactivityAction(10000, returnToStatus);
	buttons[0].setRepeatTime(10);
	buttons[1].setRepeatTime(10);
	Button::setDoubleLongPressAction(bt_doubleLongAction, buttons[0], buttons[1]);
	
	LL_mDelay(10);
	
	InitOLED();	
	SSD1306_WriteString8Bit(20, 25, (uint8_t*)"DL", Font_7x9, White);
	SSD1306_UpdateScreen();
	LL_mDelay(2000);
	drawStatus();
	initSuccess = 1;
	
	set_edit = set_flash[pickRelayNum];
	for (uint8_t i = 0; i < 3; i++)
		relay[i].setOutput(set_flash[i].polarityOut);
	
		relay[2].relayAction = true;
	
	
	while(1);

}

void bt_defaultAction(uint32_t btNum)
{
	switch (screen)
	{
	case STATUS:
		 
		break;
	case SETTINGS:
			
		scrollMenu(*selectedItem->getParent(false), btNum);
		
		break;
	case PARAMETER:
		
		editValueCallback(!btNum, selectedItem->argMax() < 2);
		
//		if (selectedItem->getParent(false) == &globalTime)
//		{
//			LL_RTC_DisableWriteProtection(RTC);
//			LL_RTC_EnterInitMode(RTC);
//			LL_RTC_TIME_SetHour(RTC, __LL_RTC_CONVERT_BIN2BCD(rtc_hours));
//			LL_RTC_TIME_SetMinute(RTC, __LL_RTC_CONVERT_BIN2BCD(rtc_minutes));
//			LL_RTC_TIME_SetSecond(RTC, __LL_RTC_CONVERT_BIN2BCD(rtc_seconds));
//			LL_RTC_ExitInitMode(RTC);
//			LL_RTC_EnableWriteProtection(RTC);
//		}
		
		break;
	default:
		break;
	}
}

void bt_longAction(uint32_t btNum)
{
	switch (screen)
	{
	case STATUS:
		
		if (!btNum)
		{
			drawMenu(startupItem, 0);
			screen = SETTINGS;
		}
		
		break;
	case SETTINGS:
		
		if (!btNum)
		{
			if (selectedItem == &backButton_1)
			{
				set_flash[pickRelayNum] = set_edit;
				FlashWrite(FLASH_START + sizeof(set_flash[0]) * pickRelayNum, (uint8_t*)&set_flash[pickRelayNum], sizeof(set_flash[0]));
				drawStatus();
				screen = STATUS;
			}
			else if ((selectedItem == &backButton_2) || (selectedItem == &backButton_3) 
				|| (selectedItem == &backButton_4) || (selectedItem == &backButton_5) 
				|| (selectedItem == &backButton_6) || (selectedItem == &backButton_7) 
				|| (selectedItem == &backButton_8) || (selectedItem == &backButton_9))
			{
				drawMenu(*selectedItem->getParent(true), 0);
			}
			else if (selectedItem->argMax() > 0)
			{
				drawValueEdit(selectedItem->argMax() < 2);
				screen = PARAMETER;
			}
			else 
			{ 
				drawMenu(*selectedItem, 0);
			}
			
			if (selectedItem == &relayMenu)
			{
				set_flash[pickRelayNum] = set_edit;
				FlashWrite(FLASH_START + sizeof(set_flash[0]) * pickRelayNum, (uint8_t*)&set_flash[pickRelayNum], sizeof(set_flash[0]));
			}
		}
		
		
		
			
		
		break;
	case PARAMETER:
		
		bt[0].enableAutoRepeat(true);
		bt[1].enableAutoRepeat(true);
		
		break;
	default:
		break;
	}
}

void bt_doubleLongAction()
{
	switch (screen)
	{
	case STATUS:
		
		break;
	case SETTINGS:
		
		break;
	case PARAMETER:
		
		bt[0].enableAutoRepeat(false);
		bt[1].enableAutoRepeat(false);
		
		if (selectedItem == &relayMenu)
			set_edit = set_flash[pickRelayNum];
		
		drawMenu(*selectedItem->getParent(false), 0);
		screen = SETTINGS;		
		
		break;
	default:
		break;
	}
}

void firstFlashWrite()
{
	memset(&set_flash, 0, sizeof(set_flash));
	
	for (uint8_t i = 0; i < 3; i++)
	{
		FlashWrite(FLASH_START + sizeof(set_flash[0]) * i, (uint8_t*)&set_flash[i], sizeof(set_flash[0]));
	}
}