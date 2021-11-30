#include "relay.h"
#include "tim.h"


extern Settings set_flash[3], set_edit;

Relay::Relay(GPIO_TypeDef* inPort, uint8_t inPin, uint8_t relayNumber)
{
	if (inPort)
		inputPort = inPort;
	inputPin = inPin;
	relayNum = relayNumber;
}

//void Relay::start()
//{
//	tim2MaxDelay = convertToSeconds(set_flash[relayNum].outputDelayH, set_flash[relayNum].outputDelayM, set_flash[relayNum].outputDelayS) - 1; 
//	if (tim2MaxDelay > 3599999)
//		tim2MaxDelay = 0;
//	TIM2->CR1 |= TIM_CR1_CEN;
//}

// Callback executed after delay TD after relay input signal occurs
//void Relay::TIM2_Callback()
//{
//	if (tim2CountDelay < tim2MaxDelay)
//		tim2CountDelay++;
//	else 
//	{
//		if (set_flash[relayNum].pulseModeEnabledOut)
//		{
//			if (tim2CountDuration == 0)
//			{
//				setOutput(set_flash[relayNum].polarityOut);
//				set_flash[relayNum].pulseCounter++;
//			}
//			else if (tim2CountDuration == set_flash[relayNum].t1)
//			{
//				setOutput(!set_flash[relayNum].polarityOut);
//			}
//			
//			if (tim2CountDuration == set_flash[relayNum].t1 + set_flash[relayNum].t2)
//			{
//				tim2CountDuration = 0;
//				setOutput(set_flash[relayNum].polarityOut);
//				set_flash[relayNum].pulseCounter++;
//			}
//			
//			tim2CountDuration++;
//			
//			if (set_flash[relayNum].pulseCounter == set_flash[relayNum].outputRepetition + 1)
//			{
//				if (set_flash[relayNum].pulseCounter > set_flash[relayNum].outputRepetition)
//					set_flash[relayNum].pulseCounter = set_flash[relayNum].outputRepetition;
//				TIM2->CR1 &= ~TIM_CR1_CEN;
//				setOutput(set_flash[relayNum].polarityOut);
//			}
//		}
//		else
//			setOutput(set_flash[relayNum].polarityOut);
//	}
//}

void Relay::setOutput(bool state)
{
	if (state)
		LL_GPIO_SetOutputPin(inputPort, inputPin);
	else
		LL_GPIO_ResetOutputPin(inputPort, inputPin);
}

bool Relay::getOutputState()
{
	return LL_GPIO_IsInputPinSet(inputPort, inputPin);
}

bool Relay::isActiveInputPin()
{
	if (set_flash[relayNum].polarityIn && LL_GPIO_IsInputPinSet(inputPort, inputPin))
		activeInputPinSet = true;
	else if (!(set_flash[relayNum].polarityIn) && !(LL_GPIO_IsInputPinSet(inputPort, inputPin)))
		activeInputPinSet = true;
	else
		activeInputPinSet = false;
	
	return activeInputPinSet;
}

uint32_t Relay::convertToSeconds(uint32_t hours, uint32_t minutes, uint32_t seconds)
{
	uint32_t value;
	value = hours * 3600 + minutes * 60 + seconds; 
	return value;
}

void Relay::timer(uint32_t hours, uint32_t minutes, uint32_t seconds)
{
	if (!timerAction)
	{
		_hours = hours;
		_minutes = minutes;
		_seconds = seconds * 1000;
	}
	
	timerAction = true;
	
	if (_seconds > 0)
		_seconds--;
	else if (_minutes > 0 && _seconds == 0)
	{
		_seconds = 59999;
		_minutes--;
	}
	else if (_hours > 0 && _minutes == 0)
	{
		_seconds = 59999;
		_minutes = 59;
		_hours--;
	}
	
	if (_hours == 0 && _minutes == 0 && _seconds == 0)
		timerAction = false;
	
}

void Relay::repetitionCounter(uint32_t repetition)
{	
	if (!repetitionAction)
		_repetition = repetition;
	
	repetitionAction = true;
	
	if (_repetition > 0 && !timerAction)
		_repetition--;
	
	if (_repetition == 0)
	{
		relayStart = false;
		relayAction = true;
		timerAction = false;
		repetitionAction = false;
		t1Action = false;
	}
}


void Relay::manage()
{
	if (relayStart)
	{
		if (isActiveInputPin())
		{
			if (set_flash[relayNum].pulseModeEnabledIn)
			{
				timer(set_flash[relayNum].inputHour, set_flash[relayNum].inputMin, set_flash[relayNum].inputSec);
				repetitionCounter(set_flash[relayNum].inputRepetition);
			}
			else
			{
				relayStart = false; 
				relayAction = true;
			}
		}
		else
		{
			if (set_flash[relayNum].pulseModeEnabledIn)
			{
				if (timerAction)
					repetitionAction = false;
				timerAction = false;
			}
			else
			{	
				relayAction = false;
			}
		}
	}
	
	
	if (relayAction)
	{
		if (!t1Action && !t2Action)
			timer(set_flash[relayNum].outputDelayH, set_flash[relayNum].outputDelayM, set_flash[relayNum].outputDelayS);
		
		if (set_flash[relayNum].pulseModeEnabledOut)
		{
			if (!t1Action && !t2Action && !timerAction) 
			{
				setOutput(!set_flash[relayNum].polarityOut);
				t1Action = true;
			}
			
			if (t1Action)
			{
				timer(set_flash[relayNum].t1Hour, set_flash[relayNum].t1Min, set_flash[relayNum].t1Sec);
				if (!timerAction)
				{
					setOutput(set_flash[relayNum].polarityOut);
					t1Action = false;
					t2Action = true; 
				}
			}
			if (t2Action)
			{
				timer(set_flash[relayNum].t2Hour, set_flash[relayNum].t2Min, set_flash[relayNum].t2Sec); 
				if (!timerAction) 
				{
					setOutput(!set_flash[relayNum].polarityOut);
					t1Action = true;
					t2Action = false;
				}
			}
			if (!timerAction && !t2Action && t1Action)
			{
				repetitionCounter(set_flash[relayNum].outputRepetition);
				if (!repetitionAction) 
					relayAction = false;
			}
		}
		else
		{
			setOutput(!set_flash[relayNum].polarityOut);
		}
		
	}
}
