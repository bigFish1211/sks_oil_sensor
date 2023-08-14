################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/DataUtil.c \
../Source/DateUtil.c \
../Source/EMA.c \
../Source/KalmanFilter.c \
../Source/LSM6DS3.c \
../Source/Util.c \
../Source/alarm.c \
../Source/command.c \
../Source/delay.c \
../Source/flash.c \
../Source/gpio.c \
../Source/internal_temperature.c \
../Source/main.c \
../Source/printf-stdarg.c \
../Source/spi.c \
../Source/stm32f0xx_it.c \
../Source/syscalls.c \
../Source/system_stm32f0xx.c \
../Source/timer.c \
../Source/uart.c \
../Source/xprintf.c 

S_UPPER_SRCS += \
../Source/startup_stm32f030.S 

OBJS += \
./Source/DataUtil.o \
./Source/DateUtil.o \
./Source/EMA.o \
./Source/KalmanFilter.o \
./Source/LSM6DS3.o \
./Source/Util.o \
./Source/alarm.o \
./Source/command.o \
./Source/delay.o \
./Source/flash.o \
./Source/gpio.o \
./Source/internal_temperature.o \
./Source/main.o \
./Source/printf-stdarg.o \
./Source/spi.o \
./Source/startup_stm32f030.o \
./Source/stm32f0xx_it.o \
./Source/syscalls.o \
./Source/system_stm32f0xx.o \
./Source/timer.o \
./Source/uart.o \
./Source/xprintf.o 

S_UPPER_DEPS += \
./Source/startup_stm32f030.d 

C_DEPS += \
./Source/DataUtil.d \
./Source/DateUtil.d \
./Source/EMA.d \
./Source/KalmanFilter.d \
./Source/LSM6DS3.d \
./Source/Util.d \
./Source/alarm.d \
./Source/command.d \
./Source/delay.d \
./Source/flash.d \
./Source/gpio.d \
./Source/internal_temperature.d \
./Source/main.d \
./Source/printf-stdarg.d \
./Source/spi.d \
./Source/stm32f0xx_it.d \
./Source/syscalls.d \
./Source/system_stm32f0xx.d \
./Source/timer.d \
./Source/uart.d \
./Source/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/%.o: ../Source/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\EclipseIndigo\CodeSourcery\Sourcery_CodeBench_Lite_for_ARM_EABI\arm-none-eabi\include" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


