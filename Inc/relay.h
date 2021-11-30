#pragma once 

#include "stdint.h"
#include "stm32l0xx.h"


class Relay
{
public:
	Relay(GPIO_TypeDef* inPort, uint8_t inPin, uint8_t relayNumber);
	void start();
	void TIM2_Callback();
	void setOutput(bool state);
	bool getOutputState();
	bool isActiveInputPin();
	void manage();
	void timer(uint32_t hours, uint32_t minutes, uint32_t seconds);
	void repetitionCounter(uint32_t repetition);
	uint32_t convertToSeconds(uint32_t hours, uint32_t minutes, uint32_t seconds);
	bool relayAction = false;
	bool activeInputPinSet = false;
	bool timerAction = false;
	bool repetitionAction = false;
	bool relayStart = false;
	bool t1Action = false;
	bool t2Action = false;
	uint32_t pulseCounterIn = 0;

private:
	uint32_t _hours;
	uint32_t _minutes;
	uint32_t _seconds;
	uint32_t tim2MaxDelay, tim2CountDelay = 0;
	uint16_t tim2MaxDuration, tim2CountDuration = 0; 
	uint8_t relayNum;
	uint32_t _repetition;
	GPIO_TypeDef* inputPort;
	uint8_t inputPin;
};

