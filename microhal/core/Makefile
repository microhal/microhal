#=============================================================================#
# microHAL makefile
#
# author: Pawel Okas
# last change: 2014-09-29
#
#=============================================================================#
INC_DIRS := $(INC_DIRS) $(MICROHAL_DIR)

SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR) $(MICROHAL_DIR)/ioDeviceNull

ifneq (,$(findstring HostComm, $(COMPONENTS)))
	SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/hostComm
endif

ifneq (,$(findstring Diagnostic, $(COMPONENTS)))
	SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/diagnostic
endif

ifneq (,$(findstring CLI, $(COMPONENTS)))
	SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/cli
endif

ifneq (,$(findstring Button, $(COMPONENTS)))
	SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/button
endif

ifneq (,$(findstring I2CDevice, $(COMPONENTS)))
	SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/I2CDevice
endif

ifneq (,$(findstring SPIDevice, $(COMPONENTS)))
	SRCS_DIRS := $(SRCS_DIRS) $(MICROHAL_DIR)/SPIDevice
endif

include $(MICROHAL_DIR)\os\makefile
include $(MICROHAL_DIR)\ports\makefile