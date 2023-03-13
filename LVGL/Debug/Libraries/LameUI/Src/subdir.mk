################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Libraries/LameUI/Src/lame_ui.c 

OBJS += \
./Libraries/LameUI/Src/lame_ui.o 

C_DEPS += \
./Libraries/LameUI/Src/lame_ui.d 


# Each subdirectory must supply rules for building sources it contributes
Libraries/LameUI/Src/%.o Libraries/LameUI/Src/%.su: ../Libraries/LameUI/Src/%.c Libraries/LameUI/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I"E:/STM codes/STM32F401 SPACE/LVGL/Libraries/BML_DRIVERS/Inc" -I"E:/STM codes/STM32F401 SPACE/LVGL/Libraries/LameUI/Inc" -I"E:/STM codes/STM32F401 SPACE/LVGL/Libraries/tft_lib/Inc" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Libraries-2f-LameUI-2f-Src

clean-Libraries-2f-LameUI-2f-Src:
	-$(RM) ./Libraries/LameUI/Src/lame_ui.d ./Libraries/LameUI/Src/lame_ui.o ./Libraries/LameUI/Src/lame_ui.su

.PHONY: clean-Libraries-2f-LameUI-2f-Src

