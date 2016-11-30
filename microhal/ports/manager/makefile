#=============================================================================#
# microHAL makefile
#
# author: Pawel Okas
# last change: 2014-09-29
#
#=============================================================================#
MAKEFILE_DIR := $(MAKEFILE_DIR)/ports

ifneq (,$(findstring STMICROELECTRONICS, $(VENDOR)))
	include $(MICROHAL_DIR)/ports/STMicroelectronics/makefile
endif

ifneq (,$(findstring LINUX, $(VENDOR)))
	include $(MICROHAL_DIR)$(MAKEFILE_DIR)/Linux/makefile
endif

ifneq (,$(findstring WINDOWS, $(VENDOR)))
	include $(MICROHAL_DIR)$(MAKEFILE_DIR)/Windows/makefile
endif