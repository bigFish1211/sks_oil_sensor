################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/common.c \
../Src/download.c \
../Src/drv_io.c \
../Src/dvr_crc.c \
../Src/dvr_flash.c \
../Src/dvr_uart.c \
../Src/main.c \
../Src/mainboot.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_g031.c \
../Src/upload.c \
../Src/xprintf.c \
../Src/ymodem.c 

OBJS += \
./Src/common.o \
./Src/download.o \
./Src/drv_io.o \
./Src/dvr_crc.o \
./Src/dvr_flash.o \
./Src/dvr_uart.o \
./Src/main.o \
./Src/mainboot.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_g031.o \
./Src/upload.o \
./Src/xprintf.o \
./Src/ymodem.o 

C_DEPS += \
./Src/common.d \
./Src/download.d \
./Src/drv_io.d \
./Src/dvr_crc.d \
./Src/dvr_flash.d \
./Src/dvr_uart.d \
./Src/main.d \
./Src/mainboot.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_g031.d \
./Src/upload.d \
./Src/xprintf.d \
./Src/ymodem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -DSTM32 -DSTM32G031xx -DSTM32G0 -DSTM32G031C8Tx -c -I../Inc -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/common.d ./Src/common.o ./Src/common.su ./Src/download.d ./Src/download.o ./Src/download.su ./Src/drv_io.d ./Src/drv_io.o ./Src/drv_io.su ./Src/dvr_crc.d ./Src/dvr_crc.o ./Src/dvr_crc.su ./Src/dvr_flash.d ./Src/dvr_flash.o ./Src/dvr_flash.su ./Src/dvr_uart.d ./Src/dvr_uart.o ./Src/dvr_uart.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/mainboot.d ./Src/mainboot.o ./Src/mainboot.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_g031.d ./Src/system_g031.o ./Src/system_g031.su ./Src/upload.d ./Src/upload.o ./Src/upload.su ./Src/xprintf.d ./Src/xprintf.o ./Src/xprintf.su ./Src/ymodem.d ./Src/ymodem.o ./Src/ymodem.su

.PHONY: clean-Src

