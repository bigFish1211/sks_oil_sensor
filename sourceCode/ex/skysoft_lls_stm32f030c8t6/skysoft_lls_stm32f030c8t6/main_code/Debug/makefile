################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := cs-rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Source/subdir.mk
-include Libraries/STM32F0xx_StdPeriph_Driver/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
skysoft_lls_stm32f030c8t6.hex \

SECONDARY_LIST += \
skysoft_lls_stm32f030c8t6.lst \

SECONDARY_SIZE += \
skysoft_lls_stm32f030c8t6.siz \


# All Target
all: skysoft_lls_stm32f030c8t6.elf secondary-outputs

# Tool invocations
skysoft_lls_stm32f030c8t6.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Sourcery Windows GCC C Linker'
	arm-none-eabi-gcc -T"C:\Working\CURRENT_PROJECT\SKYBOX\skysoft_lls_stm32f030c8t6\main_code\stm32_flash.ld" -Xlinker --gc-sections -Wl,-Map,skysoft_lls_stm32f030c8t6.map -mcpu=cortex-m0 -mthumb -g3 -gdwarf-2 -o "skysoft_lls_stm32f030c8t6.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

skysoft_lls_stm32f030c8t6.hex: skysoft_lls_stm32f030c8t6.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex skysoft_lls_stm32f030c8t6.elf  "skysoft_lls_stm32f030c8t6.hex"
	@echo 'Finished building: $@'
	@echo ' '

skysoft_lls_stm32f030c8t6.lst: skysoft_lls_stm32f030c8t6.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Create Listing'
	arm-none-eabi-objdump -h -S skysoft_lls_stm32f030c8t6.elf > "skysoft_lls_stm32f030c8t6.lst"
	@echo 'Finished building: $@'
	@echo ' '

skysoft_lls_stm32f030c8t6.siz: skysoft_lls_stm32f030c8t6.elf
	@echo 'Invoking: ARM Sourcery Windows GNU Print Size'
	arm-none-eabi-size  --format=berkeley -x skysoft_lls_stm32f030c8t6.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) skysoft_lls_stm32f030c8t6.elf
	-@echo ' '

post-build:
	-@echo 'Create Binary'
	-arm-none-eabi-objcopy -S  -O binary "skysoft_lls_stm32f030c8t6.elf" "skysoft_lls_stm32f030c8t6.bin"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
