/*
 * DefineAndVars.h
 *
 *  Created on: Jun 4, 2024
 *      Author: alexander
 */

#ifndef SRC_DEFINES_AND_VARS_H_
#define SRC_DEFINES_AND_VARS_H_

#include "main.h"

#define QUEUE_SIZE 10
#define NUM_OF_ADC 1
#define ANTI_BOUNCE_DELAY 100
#define ADC_MESS_BYTES 10
#define SENSOR_MESS_BYTES 50
#define RECEIVE_PWM_PACKET_SIZE 9
#define SENSOR_TASK_DELAY_MS 1000

osMessageQueueId_t sensorMessageQueue;
osMessageQueueId_t buttonInterruptStateQueue;
osMessageQueueId_t buttonEventMessadgeQueue;
osMessageQueueId_t uartPWMMessageQueue;

RingBuffer rxRingbBuffer;
osMutexId_t uartMutex;

typedef struct {

  char sensorData[SENSOR_MESS_BYTES];
  uint32_t adcValue[NUM_OF_ADC];
  char ADCData[ADC_MESS_BYTES];

} DataSensorPacket;

typedef struct {

  bool buttonState;
  const uint8_t* event;

} DataButtonPacket;

#endif /* SRC_DEFINES_AND_VARS_H_ */
