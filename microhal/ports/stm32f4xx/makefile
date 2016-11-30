#=============================================================================#
# microHAL makefile
#
# author: Pawel Okas
# last change: 2014-09-29
#
#=============================================================================#
MAKEFILE_DIR := $(MAKEFILE_DIR)/STM32F4xx

# core type
CORE = cortex-m4

#=============================================================================#
# various compilation flags
#=============================================================================#

# core flags
CORE_FLAGS = -mcpu=$(CORE) -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -ffast-math 

# linker script
LD_SCRIPT =  $(MICROHAL_DIR)/$(MAKEFILE_DIR)/startup/STM32F4xxxG_rom.ld

#SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/$(MAKEFILE_DIR) $(MICROHAL_DIR)/$(MAKEFILE_DIR)/startup
SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)$(MAKEFILE_DIR)/startup

CXX_FILES := $(CXX_FILES) $(MICROHAL_DIR)$(MAKEFILE_DIR)/core_STM32F4xx.cpp $(MICROHAL_DIR)$(MAKEFILE_DIR)/gpio_stm32f4xx.cpp
C_FILES := $(C_FILES) $(MICROHAL_DIR)$(MAKEFILE_DIR)/syscalls.c
 
INC_DIRS := $(INC_DIRS) $(MICROHAL_DIR)/$(MAKEFILE_DIR) $(MICROHAL_DIR)/ports/common/CMSIS/Include

#ifneq (,$(findstring SPIDevice, $(COMPONENTS)))
ifneq (,$(filter $(COMPONENTS),SPI SPIDevice))
	SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/$(MAKEFILE_DIR)/spi	
endif

ifneq (,$(findstring SerialPort, $(COMPONENTS)))
	SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/$(MAKEFILE_DIR)/serialPort
	CXX_FILES := $(CXX_FILES) $(MICROHAL_DIR)$(MAKEFILE_DIR)/serialPort_stm32f4xx.cpp
endif

#ifneq (,$(findstring I2C, $(COMPONENTS)))
ifneq (,$(filter $(COMPONENTS),I2C I2CDevice))
	SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/$(MAKEFILE_DIR)/i2c
endif

ifneq (,$(findstring USB, $(COMPONENTS)))
	CXX_FILES := $(CXX_FILES) $(MICROHAL_DIR)/$(MAKEFILE_DIR)/usb_stm32f4xx.cpp
endif

ifneq (,$(findstring ADC, $(COMPONENTS)))
	CXX_FILES := $(CXX_FILES) $(MICROHAL_DIR)/$(MAKEFILE_DIR)/adc_stm32f4xx.cpp
endif

