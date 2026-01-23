#################################################################
#
# $Id: EvaXPC560XB.mak 17608 2014-02-03 14:53:52Z tojo2507 $
#
# This makefile contains board-specific definitions and rules.
#
# Copyright 1998-2014 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.
#
#################################################################



#################################################################
# DEFINITIONS


CC_INCLUDE_PATH      += $(BOARD_PROJECT_PATH)
CPP_INCLUDE_PATH     += $(BOARD_PROJECT_PATH)
ASM_INCLUDE_PATH     += $(BOARD_PROJECT_PATH)

CC_FILES_TO_BUILD += $(BOARD_PROJECT_PATH)\board.c \
                     $(BOARD_PROJECT_PATH)\board_irq.c

ifeq ($(TOOLCHAIN),mwerks)
# in this folder will have, CW specific files
CC_FILES_TO_BUILD += $(BOARD_PROJECT_PATH)\mwerks\__ppc_eabi_init.c
CC_FILES_TO_BUILD += $(BOARD_PROJECT_PATH)\mwerks\__start.c

EXCLUDE_MAKE_DEPEND  += $(BOARD_PROJECT_PATH)\mwerks\cstartup.s \
                        $(BOARD_PROJECT_PATH)\mwerks\vector_vle_eb_cw.s
ASM_FILES_TO_BUILD +=  $(BOARD_PROJECT_PATH)\mwerks\cstartup.s \
                       $(BOARD_PROJECT_PATH)\mwerks\vector_vle_eb_cw.s

# this file is used to call compilers own functions in order
# to initialize the RAM
CC_INCLUDE_PATH      += $(BOARD_PROJECT_PATH)\mwerks
else
EXCLUDE_MAKE_DEPEND  += $(BOARD_PROJECT_PATH)\cstartup.s \
                        $(BOARD_PROJECT_PATH)\vector_vle_eb.s
ASM_FILES_TO_BUILD +=  $(BOARD_PROJECT_PATH)\cstartup.s \
                       $(BOARD_PROJECT_PATH)\vector_vle_eb.s
endif

PLATFORMS_TOOL := $(TOOLCHAIN)

######################## Defines for Linker file ############

ifeq ($(TOOLCHAIN),multi)
LOC_FILE_SUFFIX=ld
endif
ifeq ($(TOOLCHAIN),diab)
LOC_FILE_SUFFIX=dld
endif
ifeq ($(TOOLCHAIN),gnu)
LOC_FILE_SUFFIX=ldconfig
endif
ifeq ($(TOOLCHAIN),mwerks)
LOC_FILE_SUFFIX=lcf
endif
ifeq ($(TOOLCHAIN),dcc)
LOC_FILE_SUFFIX=dld
endif

LOC_FILE?=$(BOARD_PROJECT_PATH)/XPC560XB_flash.$(LOC_FILE_SUFFIX)

#################################################################
# TRACE32_PATH
# Path to the Trace32 Debugger
#
TRACE32_PATH ?= C:/tools/T32_PA_new
# old T32
#TRACE32_APP_PATH ?= $(TRACE32_PATH)
# new T32 runnning on windows and linx
TRACE32_APP_PATH ?= $(TRACE32_PATH)\bin\windows64
