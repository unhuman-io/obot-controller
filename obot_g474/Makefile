
######################################
# target
######################################

$(shell touch version.h)

CONFIG=h2
CONFIG_FILE = config/config_obot_g474_$(CONFIG).cpp
PARAM=$(CONFIG)
PARAM_FILE = param/param_obot_g474_$(PARAM).c


TARGET = $(CONFIG)

ifeq ($(CONFIG), osa14)
override C_DEFS += -DSCOPE_DEBUG
endif

ifeq ($(CONFIG), h2)
override C_DEFS += -DUSE_HSI
endif

TARGET_MCU := stm32g474

include ../motorlib/make/main.mk

