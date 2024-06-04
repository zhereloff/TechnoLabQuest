/*
 * PWMControl.h
 *
 *  Created on: Jun 4, 2024
 *      Author: alexander
 */

#ifndef PWMCONTROL_PWMCONTROL_H_
#define PWMCONTROL_PWMCONTROL_H_

#define OFFSET_CHAR_TO_INT 48

#include "main.h"

class PWMControl {

public:
	void unpacket(uint8_t *packet, int *PWMData);
	void setPWMValue(TIM_HandleTypeDef *htim, uint32_t channel, int value);

};

#endif /* PWMCONTROL_PWMCONTROL_H_ */
