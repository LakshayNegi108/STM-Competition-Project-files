################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/MLX90614_LIB/Src/MLX90614.c 

OBJS += \
./Libraries/MLX90614_LIB/Src/MLX90614.o 

C_DEPS += \
./Libraries/MLX90614_LIB/Src/MLX90614.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/MLX90614_LIB/Src/%.o Libraries/MLX90614_LIB/Src/%.su: ../Libraries/MLX90614_LIB/Src/%.c Libraries/MLX90614_LIB/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"E:/STM codes/STM32F401 SPACE/rename/Libraries/tft_lib/Inc" -I"E:/STM codes/STM32F401 SPACE/rename/Libraries/MLX90614_LIB/Inc" -I"E:/STM codes/STM32F401 SPACE/rename/Libraries/BML_DRIVERS/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libraries-2f-MLX90614_LIB-2f-Src

clean-Libraries-2f-MLX90614_LIB-2f-Src:
	-$(RM) ./Libraries/MLX90614_LIB/Src/MLX90614.d ./Libraries/MLX90614_LIB/Src/MLX90614.o ./Libraries/MLX90614_LIB/Src/MLX90614.su

.PHONY: clean-Libraries-2f-MLX90614_LIB-2f-Src

