################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/UartRingBuffer/Src/UartRingbuffer_multi.c 

OBJS += \
./Libraries/UartRingBuffer/Src/UartRingbuffer_multi.o 

C_DEPS += \
./Libraries/UartRingBuffer/Src/UartRingbuffer_multi.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/UartRingBuffer/Src/%.o Libraries/UartRingBuffer/Src/%.su: ../Libraries/UartRingBuffer/Src/%.c Libraries/UartRingBuffer/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_5/Libraries/BML_DRIVERS/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_5/Libraries/ESP8266_HAL/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_5/Libraries/HMS Functions/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_5/Libraries/MAX30100_LIB/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_5/Libraries/MLX90614_LIB/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_5/Libraries/tft_lib/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_5/Libraries/UartRingBuffer/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libraries-2f-UartRingBuffer-2f-Src

clean-Libraries-2f-UartRingBuffer-2f-Src:
	-$(RM) ./Libraries/UartRingBuffer/Src/UartRingbuffer_multi.d ./Libraries/UartRingBuffer/Src/UartRingbuffer_multi.o ./Libraries/UartRingBuffer/Src/UartRingbuffer_multi.su

.PHONY: clean-Libraries-2f-UartRingBuffer-2f-Src

