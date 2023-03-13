################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/BML_DRIVERS/Src/gpio.c \
../Libraries/BML_DRIVERS/Src/i2c.c \
../Libraries/BML_DRIVERS/Src/rcc.c \
../Libraries/BML_DRIVERS/Src/uart.c 

OBJS += \
./Libraries/BML_DRIVERS/Src/gpio.o \
./Libraries/BML_DRIVERS/Src/i2c.o \
./Libraries/BML_DRIVERS/Src/rcc.o \
./Libraries/BML_DRIVERS/Src/uart.o 

C_DEPS += \
./Libraries/BML_DRIVERS/Src/gpio.d \
./Libraries/BML_DRIVERS/Src/i2c.d \
./Libraries/BML_DRIVERS/Src/rcc.d \
./Libraries/BML_DRIVERS/Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/BML_DRIVERS/Src/%.o Libraries/BML_DRIVERS/Src/%.su: ../Libraries/BML_DRIVERS/Src/%.c Libraries/BML_DRIVERS/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_3/Libraries/BML_DRIVERS/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_3/Libraries/MAX30100_LIB/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_3/Libraries/MLX90614_LIB/Inc" -I"E:/STM codes/STM32F401 SPACE/HMS_TRY_3/Libraries/tft_lib/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libraries-2f-BML_DRIVERS-2f-Src

clean-Libraries-2f-BML_DRIVERS-2f-Src:
	-$(RM) ./Libraries/BML_DRIVERS/Src/gpio.d ./Libraries/BML_DRIVERS/Src/gpio.o ./Libraries/BML_DRIVERS/Src/gpio.su ./Libraries/BML_DRIVERS/Src/i2c.d ./Libraries/BML_DRIVERS/Src/i2c.o ./Libraries/BML_DRIVERS/Src/i2c.su ./Libraries/BML_DRIVERS/Src/rcc.d ./Libraries/BML_DRIVERS/Src/rcc.o ./Libraries/BML_DRIVERS/Src/rcc.su ./Libraries/BML_DRIVERS/Src/uart.d ./Libraries/BML_DRIVERS/Src/uart.o ./Libraries/BML_DRIVERS/Src/uart.su

.PHONY: clean-Libraries-2f-BML_DRIVERS-2f-Src

