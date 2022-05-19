
######################################
# target
######################################

$(shell touch version.h)

#PARAM_DIR = params_obot_g474

CONFIG=motor_wheel
CONFIG_FILE = config/config_obot_g474_$(CONFIG).cpp
PARAM=$(CONFIG)
PARAM_FILE = param/param_obot_g474_$(PARAM).c

TARGET = $(CONFIG)

TARGET_MCU := stm32g474

# for submake
export

include ../motorlib/scripts/configure_gcc.mk
include ../motorlib/scripts/configure.mk
include ../motorlib/scripts/build.mk

include ../motorlib/scripts/make_param.mk

# $(PARAM_DIR): $(BUILD_DIR)
# 	$(MAKE) -C $(PARAM_DIR)

# .PHONY: $(PARAM_DIR)


