################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DataUtil.c \
../Core/Src/EMA.c \
../Core/Src/KalmanFilter.c \
../Core/Src/LSM6DS3TR.c \
../Core/Src/SimpleKalmanFilter.c \
../Core/Src/Util.c \
../Core/Src/command.c \
../Core/Src/delay.c \
../Core/Src/drv_io.c \
../Core/Src/drv_spi.c \
../Core/Src/dvr_flash.c \
../Core/Src/dvr_uart.c \
../Core/Src/flash.c \
../Core/Src/main.c \
../Core/Src/printf-stdarg.c \
../Core/Src/spi.c \
../Core/Src/stm32g0xx_hal_msp.c \
../Core/Src/stm32g0xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_g031.c \
../Core/Src/xprintf.c 

OBJS += \
./Core/Src/DataUtil.o \
./Core/Src/EMA.o \
./Core/Src/KalmanFilter.o \
./Core/Src/LSM6DS3TR.o \
./Core/Src/SimpleKalmanFilter.o \
./Core/Src/Util.o \
./Core/Src/command.o \
./Core/Src/delay.o \
./Core/Src/drv_io.o \
./Core/Src/drv_spi.o \
./Core/Src/dvr_flash.o \
./Core/Src/dvr_uart.o \
./Core/Src/flash.o \
./Core/Src/main.o \
./Core/Src/printf-stdarg.o \
./Core/Src/spi.o \
./Core/Src/stm32g0xx_hal_msp.o \
./Core/Src/stm32g0xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_g031.o \
./Core/Src/xprintf.o 

C_DEPS += \
./Core/Src/DataUtil.d \
./Core/Src/EMA.d \
./Core/Src/KalmanFilter.d \
./Core/Src/LSM6DS3TR.d \
./Core/Src/SimpleKalmanFilter.d \
./Core/Src/Util.d \
./Core/Src/command.d \
./Core/Src/delay.d \
./Core/Src/drv_io.d \
./Core/Src/drv_spi.d \
./Core/Src/dvr_flash.d \
./Core/Src/dvr_uart.d \
./Core/Src/flash.d \
./Core/Src/main.d \
./Core/Src/printf-stdarg.d \
./Core/Src/spi.d \
./Core/Src/stm32g0xx_hal_msp.d \
./Core/Src/stm32g0xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_g031.d \
./Core/Src/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G031xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DataUtil.cyclo ./Core/Src/DataUtil.d ./Core/Src/DataUtil.o ./Core/Src/DataUtil.su ./Core/Src/EMA.cyclo ./Core/Src/EMA.d ./Core/Src/EMA.o ./Core/Src/EMA.su ./Core/Src/KalmanFilter.cyclo ./Core/Src/KalmanFilter.d ./Core/Src/KalmanFilter.o ./Core/Src/KalmanFilter.su ./Core/Src/LSM6DS3TR.cyclo ./Core/Src/LSM6DS3TR.d ./Core/Src/LSM6DS3TR.o ./Core/Src/LSM6DS3TR.su ./Core/Src/SimpleKalmanFilter.cyclo ./Core/Src/SimpleKalmanFilter.d ./Core/Src/SimpleKalmanFilter.o ./Core/Src/SimpleKalmanFilter.su ./Core/Src/Util.cyclo ./Core/Src/Util.d ./Core/Src/Util.o ./Core/Src/Util.su ./Core/Src/command.cyclo ./Core/Src/command.d ./Core/Src/command.o ./Core/Src/command.su ./Core/Src/delay.cyclo ./Core/Src/delay.d ./Core/Src/delay.o ./Core/Src/delay.su ./Core/Src/drv_io.cyclo ./Core/Src/drv_io.d ./Core/Src/drv_io.o ./Core/Src/drv_io.su ./Core/Src/drv_spi.cyclo ./Core/Src/drv_spi.d ./Core/Src/drv_spi.o ./Core/Src/drv_spi.su ./Core/Src/dvr_flash.cyclo ./Core/Src/dvr_flash.d ./Core/Src/dvr_flash.o ./Core/Src/dvr_flash.su ./Core/Src/dvr_uart.cyclo ./Core/Src/dvr_uart.d ./Core/Src/dvr_uart.o ./Core/Src/dvr_uart.su ./Core/Src/flash.cyclo ./Core/Src/flash.d ./Core/Src/flash.o ./Core/Src/flash.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/printf-stdarg.cyclo ./Core/Src/printf-stdarg.d ./Core/Src/printf-stdarg.o ./Core/Src/printf-stdarg.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32g0xx_hal_msp.cyclo ./Core/Src/stm32g0xx_hal_msp.d ./Core/Src/stm32g0xx_hal_msp.o ./Core/Src/stm32g0xx_hal_msp.su ./Core/Src/stm32g0xx_it.cyclo ./Core/Src/stm32g0xx_it.d ./Core/Src/stm32g0xx_it.o ./Core/Src/stm32g0xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_g031.cyclo ./Core/Src/system_g031.d ./Core/Src/system_g031.o ./Core/Src/system_g031.su ./Core/Src/xprintf.cyclo ./Core/Src/xprintf.d ./Core/Src/xprintf.o ./Core/Src/xprintf.su

.PHONY: clean-Core-2f-Src

