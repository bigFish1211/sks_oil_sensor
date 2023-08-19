################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/cirbuf.c \
../Src/drv_dma.c \
../Src/dvr_uart.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/system_g031.c \
../Src/xprintf.c 

OBJS += \
./Src/cirbuf.o \
./Src/drv_dma.o \
./Src/dvr_uart.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/system_g031.o \
./Src/xprintf.o 

C_DEPS += \
./Src/cirbuf.d \
./Src/drv_dma.d \
./Src/dvr_uart.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/system_g031.d \
./Src/xprintf.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32G0 -DSTM32G031C8Tx -c -I../Inc -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/cirbuf.d ./Src/cirbuf.o ./Src/cirbuf.su ./Src/drv_dma.d ./Src/drv_dma.o ./Src/drv_dma.su ./Src/dvr_uart.d ./Src/dvr_uart.o ./Src/dvr_uart.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/system_g031.d ./Src/system_g031.o ./Src/system_g031.su ./Src/xprintf.d ./Src/xprintf.o ./Src/xprintf.su

.PHONY: clean-Src

