#include "Button.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_gpio.h"

Button* Button::dpButton1 = 0;
Button* Button::dpButton2 = 0;
void(*Button::doubleLongPressAction)() = 0;
bool Button::doublePressActionPerformed = false;
void(*Button::inactivityAction)() = 0;
uint32_t Button::inactivityMaxTime = 0;
uint32_t Button::inactivityCounter = 0;
bool Button::inactivityActionPerformed = true;


Button::Button(GPIO_TypeDef* port, uint8_t pin, void(*action)(), bool onRelease)
{
	ioPin = pin;
	ioPort = port;
	action_ = action;
	onReleaseMode = onRelease;
}

void Button::manage()
{
	if (pressStart)
	{
		pressCounter++;

		// Catch long pressing
		if((pressCounter >= pressMaxTime) && pressMaxTime)
		{
			if (!doublePressActionPerformed && !longPressed)
			{
				longPressed = true;
				
				if(longAction_ && !(dpButton1->pressStart && dpButton2->pressStart))
					longAction_();
				if (repeatMaxTime != 0)
				{
					repeatPeriodCounter = 0;
				}		
			}
		}
	}
	
	if (!LL_EXTI_IsEnabledIT_0_31(1 << ioPin))
	{
		if (sysCounter < 3)
			sysCounter++;
		else
		{
			if ((action_ != 0) && !longPressed && LL_GPIO_IsInputPinSet(ioPort, 1 << ioPin))
				action_();
			sysCounter = 0;
			longPressed = false;
			LL_EXTI_ClearFlag_0_31(1 << ioPin);
			LL_EXTI_EnableIT_0_31(1 << ioPin);
			pressCounter = 0;
		}
	}
	

	// Catch repeating action after long pressing
	if (repeatMode && repeatMaxTime && pressStart && longPressed && !(dpButton1->pressStart && dpButton2->pressStart))
	{
		if (repeatPeriodCounter >= repeatMaxTime)
		{
			if (action_)
				action_();
			inactivityCounter = 0;
			repeatPeriodCounter = 0;
		}
		else
			repeatPeriodCounter++;
	}

	if (dpButton1 && dpButton2)
	{
		if (dpButton1->longPressed && dpButton2->longPressed)
		{
			if (doubleLongPressAction && !doublePressActionPerformed)
			{
				doubleLongPressAction();
				doublePressActionPerformed = true;
			}
		}
	}
}

void Button::activate()
{
	if (LL_GPIO_IsInputPinSet(ioPort, 1 << ioPin)) // Button release
	{
		if (onReleaseMode)
			pressStart = false;

		if (repeatMode)
		{
			repeatPeriodCounter = 0;
		}
		pressCounter = 0;
		doublePressActionPerformed = false;
		LL_EXTI_DisableIT_0_31(1 << ioPin);
	}
	else // Button press
	{
		pressStart = true;
		LL_EXTI_DisableIT_0_31(1 << ioPin);
	}
	inactivityCounter = 0;
	inactivityActionPerformed = false;
}

void Button::commonManage()
{
	if (!inactivityActionPerformed)
	{
		if (inactivityCounter < inactivityMaxTime)
		{
			inactivityCounter++;
		}
		else
		{
			inactivityCounter = 0;
			if (inactivityAction)
				inactivityAction();
			inactivityActionPerformed = true;
		}
	}

}

int Button::getPressing()
{
	return pressStart;
}

void Button::setLongPressingAction(uint32_t timeMs, void(*action)())
{
	longAction_ = action;
	pressMaxTime = timeMs;
}

void Button::setRepeatTime(uint32_t periodMs)
{
	repeatMaxTime = periodMs;
}

void Button::setAction(void(*action)())
{
	action_ = action;
}

void Button::clearLongPress()
{
	pressCounter = 0;
	repeatMode = false;
	pressStart = false;
}

void Button::setInactivityAction(uint32_t periodMs, void(*action)())
{
	inactivityAction = action;
	inactivityMaxTime = periodMs;
}

void Button::setDoubleLongPressAction(void(*action)(), Button& bt1, Button& bt2)
{
	doubleLongPressAction = action;
	dpButton1 = &bt1;
	dpButton2 = &bt2;
}

void Button::enableAutoRepeat(bool enable)
{
	repeatMode = enable;
}

