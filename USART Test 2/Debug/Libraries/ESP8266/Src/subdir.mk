################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/ESP8266/Src/ESP8266.c 

OBJS += \
./Libraries/ESP8266/Src/ESP8266.o 

C_DEPS += \
./Libraries/ESP8266/Src/ESP8266.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/ESP8266/Src/%.o Libraries/ESP8266/Src/%.su: ../Libraries/ESP8266/Src/%.c Libraries/ESP8266/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"E:/STM codes/STM32F401 SPACE/USART Test 2/Libraries/ESP8266/Inc" -I"E:/STM codes/STM32F401 SPACE/USART Test 2/Libraries/ringbuffer/Inc" -I"E:/STM codes/STM32F401 SPACE/USART Test 2/Libraries/BML_DRIVERS/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libraries-2f-ESP8266-2f-Src

clean-Libraries-2f-ESP8266-2f-Src:
	-$(RM) ./Libraries/ESP8266/Src/ESP8266.d ./Libraries/ESP8266/Src/ESP8266.o ./Libraries/ESP8266/Src/ESP8266.su

.PHONY: clean-Libraries-2f-ESP8266-2f-Src
