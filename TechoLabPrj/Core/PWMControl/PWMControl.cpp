/*
 * PWMControl.cpp
 *
 *  Created on: Jun 4, 2024
 *      Author: alexander
 */

#include "PWMControl.h"

void PWMControl::unpacket(uint8_t *packet, int *PWMData)
{
	PWMData[0] = (packet[0]-OFFSET_CHAR_TO_INT) * 100 + (packet[1]-OFFSET_CHAR_TO_INT) * 10 + (packet[2]-OFFSET_CHAR_TO_INT);
	PWMData[1] = (packet[3]-OFFSET_CHAR_TO_INT) * 100 + (packet[4]-OFFSET_CHAR_TO_INT) * 10 + (packet[5]-OFFSET_CHAR_TO_INT);
	PWMData[2] = (packet[6]-OFFSET_CHAR_TO_INT) * 100 + (packet[7]-OFFSET_CHAR_TO_INT) * 10 + (packet[8]-OFFSET_CHAR_TO_INT);
}

void PWMControl::setPWMValue(TIM_HandleTypeDef *htim, uint32_t channel, int value)
{
	__HAL_TIM_SET_COMPARE(htim, channel, value);
}

