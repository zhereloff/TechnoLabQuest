/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../Modules/DHT/DHT.h"
#include "../Modules/Button/button.h"
#include "../RingBuffer/ringBuffer.h"
#include "../PWMControl/PWMControl.h"
#include "defines_and_vars.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
IWDG_HandleTypeDef hiwdg;

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

//DMA_HandleTypeDef hdma_usart2_tx;
/* Definitions for controlPWMLed */
osThreadId_t controlPWMLedHandle;
const osThreadAttr_t controlPWMLed_attributes = {
  .name = "controlPWMLed",
  .stack_size = 128 * 16,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for sensorHandler */
osThreadId_t sensorHandlerHandle;
const osThreadAttr_t sensorHandler_attributes = {
  .name = "sensorHandler",
  .stack_size = 128 * 16,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for transmitData */
osThreadId_t transmitSensorDataHandle;
const osThreadAttr_t transmitData_attributes = {
  .name = "transmitSensorData",
  .stack_size = 128 * 16,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for buttonHandler */
osThreadId_t buttonHandlerHandle;
const osThreadAttr_t buttonHandler_attributes = {
  .name = "buttonHandler",
  .stack_size = 128 * 16,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for buttonTransmitEvent */
osThreadId_t buttonTransmitEventHandle;
const osThreadAttr_t buttonTransmitEvent_attributes = {
  .name = "buttonTransmitEvent",
  .stack_size = 128 * 16,
  .priority = (osPriority_t) osPriorityNormal,
};

/* USER CODE BEGIN PV */

/* USER CODE END PV */


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM4_Init(void);
void MX_IWDG_Init(void);

void controlPWMLedTask(void *argument);
void sensorHandlerTask(void *argument);
void transmitDataTask(void *argument);
void buttonHandlerTask(void *argument);
void buttonTransmitEventTask(void *argument);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	Button userButton;
	DataButtonPacket packet;

    userButton.last_interrupt_time = 0;
    userButton.interrupt_time = HAL_GetTick();

    if (GPIO_Pin == GPIO_PIN_13) {
        if (userButton.interrupt_time - userButton.last_interrupt_time > ANTI_BOUNCE_DELAY)
        {
        	packet.buttonState = !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
        	osMessageQueuePut(buttonInterruptStateQueue, &packet.buttonState, 0, 0);

            userButton.last_interrupt_time = userButton.interrupt_time;
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        for (uint8_t i = 0; i < RECEIVE_PWM_PACKET_SIZE; i++)
        {
            rxRingbBuffer.write(rxRingbBuffer.dma_rx_buffer[i]);
        }

        osMessageQueuePut(uartPWMMessageQueue, &rxRingbBuffer.dma_rx_buffer, 0, 0);
        HAL_UART_Receive_DMA(&huart2, rxRingbBuffer.dma_rx_buffer, RECEIVE_PWM_PACKET_SIZE);
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_IWDG_Init();
  /* USER CODE BEGIN 2 */

  HAL_UART_Receive_DMA(&huart2, rxRingbBuffer.dma_rx_buffer, RECEIVE_PWM_PACKET_SIZE);

  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */

  uartPWMMessageQueue = osMessageQueueNew(QUEUE_SIZE, RECEIVE_PWM_PACKET_SIZE * sizeof(uint8_t), NULL);
  sensorMessageQueue = osMessageQueueNew(QUEUE_SIZE, sizeof(DataSensorPacket), NULL);
  buttonInterruptStateQueue = osMessageQueueNew(QUEUE_SIZE, sizeof(DataButtonPacket), NULL);
  buttonEventMessadgeQueue = osMessageQueueNew(QUEUE_SIZE, sizeof(DataButtonPacket), NULL);

  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of controlPWMLed */
  controlPWMLedHandle = osThreadNew(controlPWMLedTask, NULL, &controlPWMLed_attributes);

  /* creation of sensorHandler */
  sensorHandlerHandle = osThreadNew(sensorHandlerTask, NULL, &sensorHandler_attributes);

  /* creation of transmitData */
  transmitSensorDataHandle = osThreadNew(transmitDataTask, NULL, &transmitData_attributes);

  /* creation of buttonHandler */
  buttonHandlerHandle = osThreadNew(buttonHandlerTask, NULL, &buttonHandler_attributes);

  /* creation of buttonTransmitEvent */
  buttonTransmitEventHandle = osThreadNew(buttonTransmitEventTask, NULL, &buttonTransmitEvent_attributes);
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

  static void MX_TIM3_Init(void)
  {

	  /* USER CODE BEGIN TIM3_Init 0 */

	  /* USER CODE END TIM3_Init 0 */

	  TIM_MasterConfigTypeDef sMasterConfig = {0};
	  TIM_OC_InitTypeDef sConfigOC = {0};

	  /* USER CODE BEGIN TIM3_Init 1 */

	  /* USER CODE END TIM3_Init 1 */
	  htim3.Instance = TIM3;
	  htim3.Init.Prescaler = 0;
	  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim3.Init.Period = 255;
	  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 0;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  /* USER CODE BEGIN TIM3_Init 2 */

	  /* USER CODE END TIM3_Init 2 */
	  HAL_TIM_MspPostInit(&htim3);

  }

  /**
    * @brief TIM4 Initialization Function
    * @param None
    * @retval None
    */
  static void MX_TIM4_Init(void)
  {

    /* USER CODE BEGIN TIM4_Init 0 */

    /* USER CODE END TIM4_Init 0 */

    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};

    /* USER CODE BEGIN TIM4_Init 1 */

    /* USER CODE END TIM4_Init 1 */
    htim4.Instance = TIM4;
    htim4.Init.Prescaler = 0;
    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim4.Init.Period = 255;
    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
    {
      Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
    {
      Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
      Error_Handler();
    }
    /* USER CODE BEGIN TIM4_Init 2 */

    /* USER CODE END TIM4_Init 2 */
    HAL_TIM_MspPostInit(&htim4);

  }
/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}


void MX_IWDG_Init(void)
{
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
  hiwdg.Init.Reload = 999;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
	Error_Handler();
  }

}
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_controlPWMLedTask */
/**
  * @brief  Function implementing the controlPWMLed thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_controlPWMLedTask */
void controlPWMLedTask(void *argument)
{
  /* USER CODE BEGIN 5 */
    uint8_t packet[RECEIVE_PWM_PACKET_SIZE];
    PWMControl LEDControl;
    int LEDData[3];
  /* Infinite loop */
  for(;;)
  {
      if (osMessageQueueGet(uartPWMMessageQueue, &packet, NULL, osWaitForever) == osOK)
      {
    	  LEDControl.unpacket(packet, LEDData);
    	  LEDControl.setPWMValue(&htim3, TIM_CHANNEL_1, LEDData[0]);
    	  LEDControl.setPWMValue(&htim3, TIM_CHANNEL_2, LEDData[1]);
    	  LEDControl.setPWMValue(&htim4, TIM_CHANNEL_1, LEDData[2]);
      }
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_sensorHandlerTask */
/**
* @brief Function implementing the sensorHandler thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_sensorHandlerTask */
void sensorHandlerTask(void *argument)
{
  /* USER CODE BEGIN sensorHandlerTask */

	static DHT_sensor livingRoom = {GPIOB, GPIO_PIN_7, DHT11, GPIO_NOPULL};
	DataSensorPacket packet;

  /* Infinite loop */
  for(;;)
  {
	  DHT_data d = DHT_getData(&livingRoom);
	  sprintf(packet.sensorData, "Temperature %d C, Humidity %d%%", static_cast<uint8_t> (d.temp), static_cast<uint8_t> (d.hum));

	  HAL_ADC_Start_DMA(&hadc1, packet.adcValue, 1);
	  sprintf(packet.ADCData, "ADC: %d\n", static_cast<uint16_t> (packet.adcValue[0]));

	  osMessageQueuePut(sensorMessageQueue, &packet, 0, osWaitForever);

	  HAL_IWDG_Refresh(&hiwdg);

	  osDelay(SENSOR_TASK_DELAY_MS);
  }
  /* USER CODE END sensorHandlerTask */
}

/* USER CODE BEGIN Header_transmitDataTask */
/**
* @brief Function implementing the transmitData thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_transmitDataTask */
void transmitDataTask(void *argument)
{
  /* USER CODE BEGIN transmitDataTask */

  DataSensorPacket packet;

  /* Infinite loop */
  for(;;)
  {
	  if (osMessageQueueGet(sensorMessageQueue, &packet, NULL, osWaitForever) == osOK)
	  {
		  osMutexAcquire(uartMutex, osWaitForever);
		  HAL_UART_Transmit(&huart2, (uint8_t*)packet.ADCData, strlen(packet.ADCData), 0xFF);
	      HAL_UART_Transmit(&huart2, (uint8_t*)packet.sensorData, strlen(packet.sensorData), 0xFF);
	      osMutexRelease(uartMutex);
	  }
  }
  /* USER CODE END transmitDataTask */
}

/* USER CODE BEGIN Header_buttonHandlerTask */
/**
* @brief Function implementing the buttonHandler thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_buttonHandlerTask */
void buttonHandlerTask(void *argument)
{
  /* USER CODE BEGIN buttonHandlerTask */

	DataButtonPacket packet;
	Button userButton;

  /* Infinite loop */
  for(;;)
  {
	 if(osMessageQueueGet(buttonInterruptStateQueue, &packet, NULL, 0) == osOK)
	 {
		 userButton.setCurrentState(packet.buttonState);
	 }

	 packet.event = userButton.getButtonEvent();
	 if(packet.event)
	 {
		 osMessageQueuePut(buttonEventMessadgeQueue, &packet, 0, osWaitForever);
	 }
	 userButton.setLastState(userButton.getCurrentState());
  }
  /* USER CODE END buttonHandlerTask */
}


/* USER CODE END Header_buttonTransmitEventTask */
void buttonTransmitEventTask(void *argument)
{
	/* USER CODE BEGIN buttonHandlerTask */

	DataButtonPacket packet;

	/* Infinite loop */
	for(;;)
	{
		if(osMessageQueueGet(buttonEventMessadgeQueue, &packet, NULL, osWaitForever) == osOK)
		{
			osMutexAcquire(uartMutex, osWaitForever);
			HAL_UART_Transmit(&huart2, packet.event, strlen((const char*)packet.event), 0xFF);
			osMutexRelease(uartMutex);
		}
	}

}
/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
