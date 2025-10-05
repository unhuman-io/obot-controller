ifneq ($(USE_LLVM),)
	LLVM_PATH=../../motorlib/llvm/bin/
	CXX = $(LLVM_PATH)clang++
	CP = $(LLVM_PATH)llvm-objcopy
	CPPFLAGS += --target=thumbv7em-none-eabi
	CXXFLAGS += -fprebuilt-module-path=. $(addprefix -fmodule-file=,$(c++_header_units))
	CXXMFLAGS += -fmodule-output -x c++-module -Wno-experimental-header-units
	LDFLAGS += -nostartfiles
	c++_header_units = $(c++_header_modules:%=%.pcm)
	c++_multi_dir = $(shell $(CXX) --print-multi-directory $(CPPFLAGS))
	c++_headers_location = $(LLVM_PATH)../lib/clang-runtimes/$(c++_multi_dir)/include/c++/v1
$(info clang: $(shell which $(CXX)) $(shell $(CXX) -dumpversion))
else
	CXX = $(GCC_PATH)arm-none-eabi-g++
	CP = $(GCC_PATH)arm-none-eabi-objcopy
	CXXFLAGS += -fmodules-ts -Mno-modules
	CXXMFLAGS += -x c++
	LDFLAGS += -specs=nosys.specs
	c++_headers_location := $(shell realpath `$(CXX) --print-sysroot`)/include/c++/$(shell $(CXX) -dumpversion)
$(info c++ headers location: $(c++_headers_location))
	c++_header_units = $(addprefix gcm.cache/.$(c++_headers_location)/, $(c++_header_modules:%=%.gcm))
$(info gcc: $(shell which $(CXX)) $(shell $(CXX) -dumpversion))
endif

C_INCLUDES += -I$(SELF_DIR)../motorlib/CMSIS/Include -I$(SELF_DIR)../motorlib/device/stm32g4/Include
CPPFLAGS += -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += $(C_INCLUDES) -g
CXXFLAGS += -std=c++20
LDFLAGS += -L$(SELF_DIR)src -TSTM32G474RETx_FLASH.ld

.EXTRA_PREREQS := $(.EXTRA_PREREQS) $(lastword $(MAKEFILE_LIST))
