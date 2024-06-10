# Тестовое задание на вакансию Embedded разработчик
## Видео работы устройства:
https://youtu.be/gYxEXtR9L0Y
## Ссылка на репозиторий с Python скриптом:
https://github.com/zhereloff/TechoLabQuestCLI
![TFBda2Bvp-U](https://github.com/zhereloff/TechnoLabQuest/assets/130227724/35837513-1366-4e9a-ae0c-eccf2652bfbf)

## Что было сделано:
- Реализация проекта под FreeRTOS
- Обмен данными между Windows хостом и отладочной платой STM32 NUCLEO-F446RE по UART
- Асинхронная передача с платы на хост событий от нажатия кнопки, подключенной к плате.
- Прием команд с хоста для управления яркостью светодиодов
- Передача данных о состоянии датчика температуры и влажности(DTH11) и потенциометра
## Задачи FreeRTOS
- **controlPWMLedTask**: Управление яркостью светодиодов с помощью PWM.
- **sensorHandlerTask**: Считывание данных с датчиков.
- **transmitDataTask**: Передача данных с датчиков по UART.
- **buttonHandlerTask**: Обработка нажатий кнопок.
- **buttonTransmitEventTask**: Передача событий кнопок по UART.
## Коллбэки
- HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)`: Обработка прерываний от кнопок.
- HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)`: Обработка завершения приема данных по UART.
## Подключение
- Датчик DHT подключен к порту GPIOB, PIN_7/GPIO_Input.
- Потенциометр подключен к порту PA1/ADC1_IN1
- Кнопка подключена к порту GPIOC, PIN_13/GPIO_EXTI13.(Использовал кнопку на самой отладочной плате)
- UART2(PA2-TX, PA3-RX) используется для передачи данных.
- PWM каналы настроены на TIM3_CH1, TIM3_CH2 и TIM4_CH1.
