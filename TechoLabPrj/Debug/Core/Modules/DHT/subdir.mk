################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Modules/DHT/DHT.cpp 

OBJS += \
./Core/Modules/DHT/DHT.o 

CPP_DEPS += \
./Core/Modules/DHT/DHT.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Modules/DHT/%.o Core/Modules/DHT/%.su: ../Core/Modules/DHT/%.cpp Core/Modules/DHT/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Modules-2f-DHT

clean-Core-2f-Modules-2f-DHT:
	-$(RM) ./Core/Modules/DHT/DHT.d ./Core/Modules/DHT/DHT.o ./Core/Modules/DHT/DHT.su

.PHONY: clean-Core-2f-Modules-2f-DHT

