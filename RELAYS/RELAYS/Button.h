#pragma once

#include "stm32l0xx.h"

class Button
{
public:		
	Button(GPIO_TypeDef* port, uint8_t pin, void(*action)(), bool onRelease = false);
	void manage();
	static void commonManage();
	void activate();
	int getPressing();
	void setLongPressingAction(uint32_t timeMs, void(*action)() = 0);
	void setRepeatTime(uint32_t periodMs);
	void setAction(void(*action)());
	void clearLongPress();
	void enableAutoRepeat(bool enable);
	static void setInactivityAction(uint32_t periodMs, void(*action)() = 0);
	static void setDoubleLongPressAction(void(*action)(), Button& bt1, Button& bt2);
private:
	static Button * dpButton1, * dpButton2;
	static void(*doubleLongPressAction)();
	static bool doublePressActionPerformed;
	static void(*inactivityAction)();
	static uint32_t inactivityMaxTime;
	static uint32_t inactivityCounter;
	static bool inactivityActionPerformed;
	void(*action_)() = 0;
	void(*longAction_)() = 0;
	GPIO_TypeDef* ioPort;
	uint8_t ioPin;
	uint32_t pressCounter;
	uint8_t repeatPeriodCounter;
	uint8_t repeatMaxTime = 0;
	uint32_t pressMaxTime = 0;
	uint16_t sysCounter;
	bool onReleaseMode = false;
	bool pressStart = false;
	bool longPressed = false;
	bool repeatMode = false;
};