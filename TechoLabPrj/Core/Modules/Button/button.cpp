/*
 * button.cpp
 *
 *  Created on: 2 июн. 2024 г.
 *      Author: alexander
 */

#include "button.h"



const uint8_t* Button::getButtonEvent()
{
	static ButtonEvent events;

	if(getCurrentState() == GPIO_PIN_SET && getLastState() == GPIO_PIN_RESET)
	{
		this->press_start_time = osKernelGetTickCount();
		return events.butPressed;
	}

	else if (getCurrentState() == GPIO_PIN_SET && getLastState() == GPIO_PIN_SET)
	{
		if (osKernelGetTickCount() - this->press_start_time >= HOLD_TIME)
		{
			this->press_start_time = osKernelGetTickCount();
			return events.butHeld;
		}
	}

	else if (getCurrentState() == GPIO_PIN_RESET && getLastState() == GPIO_PIN_SET)
	{
		uint32_t press_duration = osKernelGetTickCount() - this->press_start_time;
		if (press_duration < HOLD_TIME)
		{
			return events.butReleased;
		}
	}

	return nullptr;
}

