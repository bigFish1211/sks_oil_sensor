################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/EMA.c \
../Core/Src/NTC.c \
../Core/Src/SimpleKalmanFilter.c \
../Core/Src/command.c \
../Core/Src/flash.c \
../Core/Src/main.c \
../Core/Src/mainTask.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g0xx.c 

OBJS += \
./Core/Src/EMA.o \
./Core/Src/NTC.o \
./Core/Src/SimpleKalmanFilter.o \
./Core/Src/command.o \
./Core/Src/flash.o \
./Core/Src/main.o \
./Core/Src/mainTask.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g0xx.o 

C_DEPS += \
./Core/Src/EMA.d \
./Core/Src/NTC.d \
./Core/Src/SimpleKalmanFilter.d \
./Core/Src/command.d \
./Core/Src/flash.d \
./Core/Src/main.d \
./Core/Src/mainTask.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g0xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/EMA.d ./Core/Src/EMA.o ./Core/Src/EMA.su ./Core/Src/NTC.d ./Core/Src/NTC.o ./Core/Src/NTC.su ./Core/Src/SimpleKalmanFilter.d ./Core/Src/SimpleKalmanFilter.o ./Core/Src/SimpleKalmanFilter.su ./Core/Src/command.d ./Core/Src/command.o ./Core/Src/command.su ./Core/Src/flash.d ./Core/Src/flash.o ./Core/Src/flash.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mainTask.d ./Core/Src/mainTask.o ./Core/Src/mainTask.su ./Core/Src/stm32g0xx_hal_msp.d ./Core/Src/stm32g0xx_hal_msp.o ./Core/Src/stm32g0xx_hal_msp.su ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/stm32g0xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g0xx.d ./Core/Src/system_stm32g0xx.o ./Core/Src/system_stm32g0xx.su

.PHONY: clean-Core-2f-Src

