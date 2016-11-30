ifeq ($(MCU_TYPE), MCU_TYPE_STM32F0XX)
    FREE_RTOS_CORE_PORT = ARM_CM0
endif

ifeq ($(MCU_TYPE), MCU_TYPE_STM32F4XX)
    FREE_RTOS_CORE_PORT = ARM_CM4
endif


INC_DIRS := $(INC_DIRS) hal/os/FreeRTOS/include
SRCS_DIRS := $(SRCS_DIRS) hal/os/FreeRTOS hal/os/FreeRTOS_port

ifeq ($(FREE_RTOS_CORE_PORT), ARM_CM0)	
    SRCS_DIRS := $(SRCS_DIRS) hal/os/FreeRTOS/portable/GCC/ARM_CM0 hal/os/FreeRTOS/portable/MemMang
    INC_DIRS := $(INC_DIRS) hal/os/FreeRTOS/portable/GCC/ARM_CM0     
endif
ifeq ($(FREE_RTOS_CORE_PORT), ARM_CM4)
    SRCS_DIRS := $(SRCS_DIRS) hal/os/FreeRTOS/portable/GCC/ARM_CM4F hal/os/FreeRTOS/portable/MemMang
    INC_DIRS := $(INC_DIRS) hal/os/FreeRTOS/portable/GCC/ARM_CM4F       
endif