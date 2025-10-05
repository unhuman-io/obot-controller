ifneq ($(USE_LLVM),)
$(info LLVM)
	LLVM_PATH=../motorlib/llvm/bin/
	CXX = $(LLVM_PATH)clang++
	CP = $(LLVM_PATH)llvm-objcopy
	CPPFLAGS += --target=thumbv7em-none-eabi
	CXXFLAGS += -fprebuilt-module-path=.
	CXXMFLAGS += -fmodule-output -x c++-module -Wno-experimental-header-units
	LDFLAGS += -nostartfiles
else
$(info GCC)
	CC = $(GCC_PATH)arm-none-eabi-gcc
	CXX = $(GCC_PATH)arm-none-eabi-g++
	CP = $(GCC_PATH)arm-none-eabi-objcopy
	CXXFLAGS += -fmodules-ts
	LDFLAGS += -specs=nosys.specs
endif

C_INCLUDES += -I$(SELF_DIR)../motorlib/CMSIS/Include -I$(SELF_DIR)../motorlib/device/stm32g4/Include
CPPFLAGS += -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += $(C_INCLUDES) -g
CXXFLAGS += -std=c++20
LDFLAGS += -L$(SELF_DIR)src -TSTM32G474RETx_FLASH.ld

.EXTRA_PREREQS := $(.EXTRA_PREREQS) $(lastword $(MAKEFILE_LIST))