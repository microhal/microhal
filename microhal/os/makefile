ifdef HAL_RTOS
ifeq ($(HAL_RTOS),FreeRTOS)    
	include hal/os/FreeRTOS_port/makefile
endif
else
	SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/os/noRTOSport
endif

