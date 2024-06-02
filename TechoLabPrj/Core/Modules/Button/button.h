/*
 * button.h
 *
 *  Created on: 2 июн. 2024 г.
 *      Author: alexander
 */

#ifndef MODULES_BUTTON_BUTTON_H_
#define MODULES_BUTTON_BUTTON_H_

#define PIN_RESET 0
#define HOLD_TIME 5000

#include "main.h"
#include "cmsis_os.h"
#include "stdbool.h"
#include  <stdio.h>

class Button {
	private:
		uint8_t last_button_state = PIN_RESET;
		uint8_t current_button_state = PIN_RESET;

	public:

		struct ButtonEvent {
			uint8_t butpressed[16] = "Button pressed\n";
		    uint8_t butheld[27] = "Button held for 5 seconds\n";
		    uint8_t butreleased[17] = "Button released\n";
		};

		osStatus_t status;
		TickType_t press_start_time = 0;
		uint32_t last_interrupt_time = 0;
		uint32_t interrupt_time;
		uint32_t press_duration;

		void setLastState(uint8_t state){
			this->last_button_state = state;
		}
		void setCurrentState(uint8_t state){
			this->current_button_state = state;
		}

		uint8_t getLastState(){
			return this->last_button_state;
		}
		uint8_t getCurrentState(){
			return this->current_button_state;
		}

		const uint8_t* getButtonEvent();

};

#endif /* MODULES_BUTTON_BUTTON_H_ */
