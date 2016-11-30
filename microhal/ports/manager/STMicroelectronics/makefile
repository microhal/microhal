#=============================================================================#
# microHAL makefile
#
# author: Pawel Okas
# last change: 2014-09-29
#
#=============================================================================#
MAKEFILE_DIR := $(MAKEFILE_DIR)/STMicroelectronics

ifneq (,$(findstring STM32F0XX, $(MCU_TYPE)))
	include $(MICROHAL_DIR)/$(MAKEFILE_DIR)/STM32F0xx/makefile
endif

ifneq (,$(findstring STM32F4XX, $(MCU_TYPE)))
	include  $(MICROHAL_DIR)/$(MAKEFILE_DIR)/STM32F4xx/makefile
endif