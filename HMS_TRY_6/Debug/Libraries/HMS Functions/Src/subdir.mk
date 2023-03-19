################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/HMS\ Functions/Src/hms.c 

OBJS += \
./Libraries/HMS\ Functions/Src/hms.o 

C_DEPS += \
./Libraries/HMS\ Functions/Src/hms.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/HMS\ Functions/Src/hms.o: ../Libraries/HMS\ Functions/Src/hms.c Libraries/HMS\ Functions/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_6/Libraries/BML_DRIVERS/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_6/Libraries/ESP8266_HAL/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_6/Libraries/HMS Functions/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_6/Libraries/MAX30100_LIB/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_6/Libraries/MLX90614_LIB/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_6/Libraries/SDCardLib/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_6/Libraries/tft_lib/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_6/Libraries/UartRingBuffer/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Libraries/HMS Functions/Src/hms.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libraries-2f-HMS-20-Functions-2f-Src

clean-Libraries-2f-HMS-20-Functions-2f-Src:
	-$(RM) ./Libraries/HMS\ Functions/Src/hms.d ./Libraries/HMS\ Functions/Src/hms.o ./Libraries/HMS\ Functions/Src/hms.su

.PHONY: clean-Libraries-2f-HMS-20-Functions-2f-Src

