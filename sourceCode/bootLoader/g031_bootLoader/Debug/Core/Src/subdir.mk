################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/common.c \
../Core/Src/download.c \
../Core/Src/drv_io.c \
../Core/Src/dvr_crc.c \
../Core/Src/dvr_flash.c \
../Core/Src/dvr_uart.c \
../Core/Src/main.c \
../Core/Src/mainTask.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_g031.c \
../Core/Src/upload.c \
../Core/Src/xprintf.c \
../Core/Src/ymodem.c 

OBJS += \
./Core/Src/common.o \
./Core/Src/download.o \
./Core/Src/drv_io.o \
./Core/Src/dvr_crc.o \
./Core/Src/dvr_flash.o \
./Core/Src/dvr_uart.o \
./Core/Src/main.o \
./Core/Src/mainTask.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_g031.o \
./Core/Src/upload.o \
./Core/Src/xprintf.o \
./Core/Src/ymodem.o 

C_DEPS += \
./Core/Src/common.d \
./Core/Src/download.d \
./Core/Src/drv_io.d \
./Core/Src/dvr_crc.d \
./Core/Src/dvr_flash.d \
./Core/Src/dvr_uart.d \
./Core/Src/main.d \
./Core/Src/mainTask.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_g031.d \
./Core/Src/upload.d \
./Core/Src/xprintf.d \
./Core/Src/ymodem.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DSTM32G031xx -c -I../Core/Inc -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/common.cyclo ./Core/Src/common.d ./Core/Src/common.o ./Core/Src/common.su ./Core/Src/download.cyclo ./Core/Src/download.d ./Core/Src/download.o ./Core/Src/download.su ./Core/Src/drv_io.cyclo ./Core/Src/drv_io.d ./Core/Src/drv_io.o ./Core/Src/drv_io.su ./Core/Src/dvr_crc.cyclo ./Core/Src/dvr_crc.d ./Core/Src/dvr_crc.o ./Core/Src/dvr_crc.su ./Core/Src/dvr_flash.cyclo ./Core/Src/dvr_flash.d ./Core/Src/dvr_flash.o ./Core/Src/dvr_flash.su ./Core/Src/dvr_uart.cyclo ./Core/Src/dvr_uart.d ./Core/Src/dvr_uart.o ./Core/Src/dvr_uart.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/mainTask.cyclo ./Core/Src/mainTask.d ./Core/Src/mainTask.o ./Core/Src/mainTask.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_g031.cyclo ./Core/Src/system_g031.d ./Core/Src/system_g031.o ./Core/Src/system_g031.su ./Core/Src/upload.cyclo ./Core/Src/upload.d ./Core/Src/upload.o ./Core/Src/upload.su ./Core/Src/xprintf.cyclo ./Core/Src/xprintf.d ./Core/Src/xprintf.o ./Core/Src/xprintf.su ./Core/Src/ymodem.cyclo ./Core/Src/ymodem.d ./Core/Src/ymodem.o ./Core/Src/ymodem.su

.PHONY: clean-Core-2f-Src

