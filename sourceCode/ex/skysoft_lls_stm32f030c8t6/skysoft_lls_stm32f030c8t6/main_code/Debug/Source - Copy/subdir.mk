################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source\ -\ Copy/DataUtil.c \
../Source\ -\ Copy/DateUtil.c \
../Source\ -\ Copy/EMA.c \
../Source\ -\ Copy/KalmanFilter.c \
../Source\ -\ Copy/LSM6DS3.c \
../Source\ -\ Copy/Util.c \
../Source\ -\ Copy/alarm.c \
../Source\ -\ Copy/command.c \
../Source\ -\ Copy/delay.c \
../Source\ -\ Copy/flash.c \
../Source\ -\ Copy/gpio.c \
../Source\ -\ Copy/internal_temperature.c \
../Source\ -\ Copy/main.c \
../Source\ -\ Copy/printf-stdarg.c \
../Source\ -\ Copy/spi.c \
../Source\ -\ Copy/stm32f0xx_it.c \
../Source\ -\ Copy/syscalls.c \
../Source\ -\ Copy/system_stm32f0xx.c \
../Source\ -\ Copy/timer.c \
../Source\ -\ Copy/uart.c \
../Source\ -\ Copy/xprintf.c 

S_UPPER_SRCS += \
../Source\ -\ Copy/startup_stm32f030.S 

OBJS += \
./Source\ -\ Copy/DataUtil.o \
./Source\ -\ Copy/DateUtil.o \
./Source\ -\ Copy/EMA.o \
./Source\ -\ Copy/KalmanFilter.o \
./Source\ -\ Copy/LSM6DS3.o \
./Source\ -\ Copy/Util.o \
./Source\ -\ Copy/alarm.o \
./Source\ -\ Copy/command.o \
./Source\ -\ Copy/delay.o \
./Source\ -\ Copy/flash.o \
./Source\ -\ Copy/gpio.o \
./Source\ -\ Copy/internal_temperature.o \
./Source\ -\ Copy/main.o \
./Source\ -\ Copy/printf-stdarg.o \
./Source\ -\ Copy/spi.o \
./Source\ -\ Copy/startup_stm32f030.o \
./Source\ -\ Copy/stm32f0xx_it.o \
./Source\ -\ Copy/syscalls.o \
./Source\ -\ Copy/system_stm32f0xx.o \
./Source\ -\ Copy/timer.o \
./Source\ -\ Copy/uart.o \
./Source\ -\ Copy/xprintf.o 

S_UPPER_DEPS += \
./Source\ -\ Copy/startup_stm32f030.d 

C_DEPS += \
./Source\ -\ Copy/DataUtil.d \
./Source\ -\ Copy/DateUtil.d \
./Source\ -\ Copy/EMA.d \
./Source\ -\ Copy/KalmanFilter.d \
./Source\ -\ Copy/LSM6DS3.d \
./Source\ -\ Copy/Util.d \
./Source\ -\ Copy/alarm.d \
./Source\ -\ Copy/command.d \
./Source\ -\ Copy/delay.d \
./Source\ -\ Copy/flash.d \
./Source\ -\ Copy/gpio.d \
./Source\ -\ Copy/internal_temperature.d \
./Source\ -\ Copy/main.d \
./Source\ -\ Copy/printf-stdarg.d \
./Source\ -\ Copy/spi.d \
./Source\ -\ Copy/stm32f0xx_it.d \
./Source\ -\ Copy/syscalls.d \
./Source\ -\ Copy/system_stm32f0xx.d \
./Source\ -\ Copy/timer.d \
./Source\ -\ Copy/uart.d \
./Source\ -\ Copy/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
Source\ -\ Copy/DataUtil.o: ../Source\ -\ Copy/DataUtil.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/DataUtil.d" -MT"Source\ -\ Copy/DataUtil.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/DateUtil.o: ../Source\ -\ Copy/DateUtil.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/DateUtil.d" -MT"Source\ -\ Copy/DateUtil.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/EMA.o: ../Source\ -\ Copy/EMA.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/EMA.d" -MT"Source\ -\ Copy/EMA.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/KalmanFilter.o: ../Source\ -\ Copy/KalmanFilter.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/KalmanFilter.d" -MT"Source\ -\ Copy/KalmanFilter.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/LSM6DS3.o: ../Source\ -\ Copy/LSM6DS3.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/LSM6DS3.d" -MT"Source\ -\ Copy/LSM6DS3.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/Util.o: ../Source\ -\ Copy/Util.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/Util.d" -MT"Source\ -\ Copy/Util.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/alarm.o: ../Source\ -\ Copy/alarm.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/alarm.d" -MT"Source\ -\ Copy/alarm.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/command.o: ../Source\ -\ Copy/command.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/command.d" -MT"Source\ -\ Copy/command.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/delay.o: ../Source\ -\ Copy/delay.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/delay.d" -MT"Source\ -\ Copy/delay.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/flash.o: ../Source\ -\ Copy/flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/flash.d" -MT"Source\ -\ Copy/flash.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/gpio.o: ../Source\ -\ Copy/gpio.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/gpio.d" -MT"Source\ -\ Copy/gpio.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/internal_temperature.o: ../Source\ -\ Copy/internal_temperature.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/internal_temperature.d" -MT"Source\ -\ Copy/internal_temperature.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/main.o: ../Source\ -\ Copy/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/main.d" -MT"Source\ -\ Copy/main.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/printf-stdarg.o: ../Source\ -\ Copy/printf-stdarg.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/printf-stdarg.d" -MT"Source\ -\ Copy/printf-stdarg.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/spi.o: ../Source\ -\ Copy/spi.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/spi.d" -MT"Source\ -\ Copy/spi.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/startup_stm32f030.o: ../Source\ -\ Copy/startup_stm32f030.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\EclipseIndigo\CodeSourcery\Sourcery_CodeBench_Lite_for_ARM_EABI\arm-none-eabi\include" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/startup_stm32f030.d" -MT"Source\ -\ Copy/startup_stm32f030.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/stm32f0xx_it.o: ../Source\ -\ Copy/stm32f0xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/stm32f0xx_it.d" -MT"Source\ -\ Copy/stm32f0xx_it.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/syscalls.o: ../Source\ -\ Copy/syscalls.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/syscalls.d" -MT"Source\ -\ Copy/syscalls.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/system_stm32f0xx.o: ../Source\ -\ Copy/system_stm32f0xx.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/system_stm32f0xx.d" -MT"Source\ -\ Copy/system_stm32f0xx.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/timer.o: ../Source\ -\ Copy/timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/timer.d" -MT"Source\ -\ Copy/timer.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/uart.o: ../Source\ -\ Copy/uart.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/uart.d" -MT"Source\ -\ Copy/uart.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source\ -\ Copy/xprintf.o: ../Source\ -\ Copy/xprintf.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -D__GNUC__ -DSTM32F0XX -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\CMSIS\Device\ST\STM32F0xx\Include" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Libraries\STM32F0xx_StdPeriph_Driver\inc" -I"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\Source" -Os -ffunction-sections -fdata-sections -Wall -std=gnu99 -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"Source - Copy/xprintf.d" -MT"Source\ -\ Copy/xprintf.d" -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


