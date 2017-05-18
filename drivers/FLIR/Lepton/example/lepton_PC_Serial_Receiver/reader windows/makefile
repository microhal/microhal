################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include microhal/microhal/ports/windows/General/tests/subdir.mk
-include microhal/microhal/ports/windows/General/subdir.mk
-include microhal/microhal/math/src/subdir.mk
-include microhal/microhal/core/signalSlot/subdir.mk
-include microhal/microhal/core/ioDeviceNull/subdir.mk
-include microhal/microhal/core/interfaces/subdir.mk
-include microhal/microhal/core/diagnostic/subdir.mk
-include microhal/microhal/core/crc/subdir.mk
-include microhal/microhal/core/subdir.mk
-include microhal/hostComm/source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
leptonPCSerialReceiver.hex \

SECONDARY_SIZE += \
leptonPCSerialReceiver.siz \


# All Target
all: leptonPCSerialReceiver.elf secondary-outputs

# Tool invocations
leptonPCSerialReceiver.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -Os -ffunction-sections -fdata-sections -fno-builtin -Wall -Wextra  -g3 -T STM32F4xxxG_rom.ld -Xlinker --gc-sections -Wl,-Map,"leptonPCSerialReceiver.map" -o "leptonPCSerialReceiver.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

leptonPCSerialReceiver.hex: leptonPCSerialReceiver.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "leptonPCSerialReceiver.elf"  "leptonPCSerialReceiver.hex"
	@echo 'Finished building: $@'
	@echo ' '

leptonPCSerialReceiver.siz: leptonPCSerialReceiver.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley $(OBJS) $(USER_OBJS) "leptonPCSerialReceiver.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) leptonPCSerialReceiver.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
