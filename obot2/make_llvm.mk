
LLVM_PATH=../motorlib/llvm/bin/
CXX = $(LLVM_PATH)clang++
CP = $(LLVM_PATH)llvm-objcopy

C_INCLUDES = -I../motorlib/CMSIS/Include -I../motorlib/device/stm32g4/Include
CPPFLAGS = --target=thumbv7em-none-eabi -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS = $(C_INCLUDES) $(CPPFLAGS)
CXXFLAGS = $(CFLAGS) -std=c++23 -g -fprebuilt-module-path=.
CXXMFLAGS = $(CXXFLAGS) -fmodule-output -x c++-module -Wno-experimental-header-units
LDFLAGS = -nostartfiles

c++_header_modules := bit
c++_header_units := $(c++_header_modules:%=%.pcm)

$(info c++ header units: $(c++_header_units))

vpath %.cpp ../motorlib/peripheral/stm32g4
vpath %.h ../motorlib/CMSIS/Include

main.bin: main.elf
	$(CP) -O binary main.elf main.bin



# manual dependencies for modules
main.o: stm32g474.o trace_board.o

trace_board.o: stm32g474.o


main.elf: main.o STM32G474RETx_FLASH.ld startup_stm32g474xx.o
	@echo "  LD    $@"
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o main.elf main.o -T../motorlib/peripheral/stm32g4/STM32G474RETx_FLASH.ld startup_stm32g474xx.o stm32g474.o trace_board.o


%.o: %.cpp $(c++_header_units)
	@echo "  CC    $<"
	$(CXX) -c $(CXXFLAGS) -MMD $(<:.d=.cpp) $(addprefix -fmodule-file=,$(c++_header_units)) -o $@

%.o: %.cppm $(c++_header_units)
	@echo "  CC    $<"
	$(CXX) -c $(CXXMFLAGS) -MMD $(<:.d=.cppm) $(addprefix -fmodule-file=,$(c++_header_units)) -o $@

%.o: %.S
	@echo "  AS    $<"
	$(CXX) $(CPPFLAGS) -c -o $@ $<

%.pcm:
	@echo "  SPCM  $*"
	$(CXX) -c $(CXXMFLAGS) -x c++-system-header -c $*

header_units: $(c++_header_units)

load: main.bin
	@echo "Loading main.bin via DFU..."
	dfu-util -a 0 -s 0x8000000:leave -D main.bin

clean:
	@echo "Cleaning up..."
	rm -rf *.o *.elf *.bin *.pcm *.d

help:
	@echo "Available targets:"
	@echo "  header_units - Generate C++ header units"
	@echo "  load    - Load the firmware onto the device via DFU"
	@echo "  clean   - Clean up generated files"
	@echo "  help    - Show this help message"
	@echo "  main.bin - Build the main binary (default target)"
	@echo "  main.elf - Build the main ELF file"
	@echo "Options:"
	@echo "  GCC_PATH - Path to the ARM GCC toolchain"
	@echo "  VERBOSE - Set to '1' to enable verbose output during build"


.PHONY: load clean help header_units

.EXTRA_PREREQS += make_llvm.mk

-include $(wildcard *.d)

$(VERBOSE).SILENT:
