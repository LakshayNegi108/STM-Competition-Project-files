################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/MAX30102_Lib/Src/MAX30102_n.c \
../Libraries/MAX30102_Lib/Src/heartRate.c \
../Libraries/MAX30102_Lib/Src/ring_buffer.c 

OBJS += \
./Libraries/MAX30102_Lib/Src/MAX30102_n.o \
./Libraries/MAX30102_Lib/Src/heartRate.o \
./Libraries/MAX30102_Lib/Src/ring_buffer.o 

C_DEPS += \
./Libraries/MAX30102_Lib/Src/MAX30102_n.d \
./Libraries/MAX30102_Lib/Src/heartRate.d \
./Libraries/MAX30102_Lib/Src/ring_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/MAX30102_Lib/Src/%.o Libraries/MAX30102_Lib/Src/%.su: ../Libraries/MAX30102_Lib/Src/%.c Libraries/MAX30102_Lib/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"E:/STM codes/STM32F401 SPACE/MAX30102_OrgLib/Libraries/BML_DRIVERS/Inc" -I"E:/STM codes/STM32F401 SPACE/MAX30102_OrgLib/Libraries/MAX30102_Lib/Inc" -I"E:/STM codes/STM32F401 SPACE/MAX30102_OrgLib/Libraries/MLX90614_LIB/Inc" -I"E:/STM codes/STM32F401 SPACE/MAX30102_OrgLib/Libraries/tft_lib/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libraries-2f-MAX30102_Lib-2f-Src

clean-Libraries-2f-MAX30102_Lib-2f-Src:
	-$(RM) ./Libraries/MAX30102_Lib/Src/MAX30102_n.d ./Libraries/MAX30102_Lib/Src/MAX30102_n.o ./Libraries/MAX30102_Lib/Src/MAX30102_n.su ./Libraries/MAX30102_Lib/Src/heartRate.d ./Libraries/MAX30102_Lib/Src/heartRate.o ./Libraries/MAX30102_Lib/Src/heartRate.su ./Libraries/MAX30102_Lib/Src/ring_buffer.d ./Libraries/MAX30102_Lib/Src/ring_buffer.o ./Libraries/MAX30102_Lib/Src/ring_buffer.su

.PHONY: clean-Libraries-2f-MAX30102_Lib-2f-Src

