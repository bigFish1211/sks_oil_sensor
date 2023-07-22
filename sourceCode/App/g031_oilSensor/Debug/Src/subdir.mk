################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/drv_io.c \
../Src/drv_spi.c \
../Src/flash.c \
../Src/gpio.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_g031.c \
../Src/uart.c \
../Src/wdg.c \
../Src/xprintf.c 

OBJS += \
./Src/drv_io.o \
./Src/drv_spi.o \
./Src/flash.o \
./Src/gpio.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_g031.o \
./Src/uart.o \
./Src/wdg.o \
./Src/xprintf.o 

C_DEPS += \
./Src/drv_io.d \
./Src/drv_spi.d \
./Src/flash.d \
./Src/gpio.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_g031.d \
./Src/uart.d \
./Src/wdg.d \
./Src/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DSTM32G031xx -DSTM32 -DSTM32G0 -DSTM32G031C8Tx -c -I"D:/github/sks_oil_sensor/sourceCode/App/g031_oilSensor/Inc" -I"D:/github/sks_oil_sensor/sourceCode/App/g031_oilSensor/Drivers/CMSIS/Device/ST/STM32G0xx/Include" -I"D:/github/sks_oil_sensor/sourceCode/App/g031_oilSensor/Drivers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/drv_io.d ./Src/drv_io.o ./Src/drv_io.su ./Src/drv_spi.d ./Src/drv_spi.o ./Src/drv_spi.su ./Src/flash.d ./Src/flash.o ./Src/flash.su ./Src/gpio.d ./Src/gpio.o ./Src/gpio.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_g031.d ./Src/system_g031.o ./Src/system_g031.su ./Src/uart.d ./Src/uart.o ./Src/uart.su ./Src/wdg.d ./Src/wdg.o ./Src/wdg.su ./Src/xprintf.d ./Src/xprintf.o ./Src/xprintf.su

.PHONY: clean-Src

