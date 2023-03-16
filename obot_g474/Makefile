
######################################
# target
######################################

$(shell touch version.h)

CONFIG=h2
CONFIG_FILE = config/config_obot_g474_$(CONFIG).cpp
ifndef PARAM
PARAM=$(CONFIG)
endif
PARAM_FILE = param/param_obot_g474_$(PARAM).c


TARGET = $(CONFIG)
# Configure the system types.
TYPE_HEADER_DEFINE = -DINSTANCE_SYSTEM_TYPES_HEADER="\"../obot_g474/config/config_obot_g474_$(CONFIG)_types.h\""

ifeq ($(CONFIG), osa14)
override C_DEFS += -DSCOPE_DEBUG
endif

ifeq ($(CONFIG), h2)
override C_DEFS += -DUSE_HSI
endif

TARGET_MCU := stm32g474

# for submake
export

include ../motorlib/scripts/configure_gcc.mk
include ../motorlib/scripts/configure.mk
include ../motorlib/scripts/build.mk

