################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../scr/KalmanFilter.c \
../scr/LSM6DS3.c \
../scr/Util.c \
../scr/alarm.c \
../scr/command.c \
../scr/delay.c \
../scr/gpio.c \
../scr/main.c \
../scr/mainboot.c \
../scr/spi.c \
../scr/stm32f0xx_it.c \
../scr/syscalls.c \
../scr/system_stm32f0xx.c \
../scr/timer.c \
../scr/uart.c \
../scr/xprintf.c 

OBJS += \
./scr/KalmanFilter.o \
./scr/LSM6DS3.o \
./scr/Util.o \
./scr/alarm.o \
./scr/command.o \
./scr/delay.o \
./scr/gpio.o \
./scr/main.o \
./scr/mainboot.o \
./scr/spi.o \
./scr/stm32f0xx_it.o \
./scr/syscalls.o \
./scr/system_stm32f0xx.o \
./scr/timer.o \
./scr/uart.o \
./scr/xprintf.o 

C_DEPS += \
./scr/KalmanFilter.d \
./scr/LSM6DS3.d \
./scr/Util.d \
./scr/alarm.d \
./scr/command.d \
./scr/delay.d \
./scr/gpio.d \
./scr/main.d \
./scr/mainboot.d \
./scr/spi.d \
./scr/stm32f0xx_it.d \
./scr/syscalls.d \
./scr/system_stm32f0xx.d \
./scr/timer.d \
./scr/uart.d \
./scr/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
scr/%.o: ../scr/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\scr" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


