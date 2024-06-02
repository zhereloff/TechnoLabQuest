/*
 * ringBuffer.h
 *
 *  Created on: 2 июн. 2024 г.
 *      Author: alexander
 */

#ifndef RINGBUFFER_RINGBUFFER_H_
#define RINGBUFFER_RINGBUFFER_H_

#include "main.h"

class RingBuffer {
public:
	private:
    	static const uint16_t BUFFER_SIZE = 256;
    	uint8_t buffer[BUFFER_SIZE];
    	volatile uint16_t write_index;
    	volatile uint16_t read_index;
	public:
    	RingBuffer(): write_index(0), read_index(0) {}

    	bool write(uint8_t data);
    	bool read(uint8_t &data);
    	bool isEmpty() const;
};

#endif /* RINGBUFFER_RINGBUFFER_H_ */
