/*
 * ringBuffer.h
 *
 *  Created on: 2 июн. 2024 г.
 *      Author: alexander
 */

#ifndef RINGBUFFER_RINGBUFFER_H_
#define RINGBUFFER_RINGBUFFER_H_

#define RECEIVE_PWM_PACKET_SIZE 9

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

    	uint8_t dma_rx_buffer[RECEIVE_PWM_PACKET_SIZE];

    	uint16_t getWriteIndex() const {
    	        return write_index;
    	}

    	uint16_t getReadIndex() const {
    	        return read_index;
    	}

    	uint8_t* getWritePointer() {
    	        return &buffer[write_index];
    	}

    	void advanceWriteIndex(uint16_t length) {
    	        write_index = (write_index + length) % BUFFER_SIZE;
    	}

    	bool write(uint8_t data);
    	bool read(uint8_t &data);
    	bool isEmpty() const;
};

#endif /* RINGBUFFER_RINGBUFFER_H_ */
