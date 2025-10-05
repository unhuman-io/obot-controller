BUILD_DIR ?= build

ifneq ($(VERBOSE),)
$(info BUILD_DIR: $(shell realpath $(BUILD_DIR)))
$(info CURDIR: $(CURDIR))
$(info MAKEFILE_LIST: $(MAKEFILE_LIST))
endif
ifneq ($(shell realpath $(BUILD_DIR)), $(CURDIR))
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	echo "override BUILD_DIR := .\ninclude ../Makefile" > $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR)

.PHONY: clean $(BUILD_DIR)

clean:
	rm -r $(BUILD_DIR)
else

SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
.EXTRA_PREREQS := $(.EXTRA_PREREQS) Makefile $(lastword $(MAKEFILE_LIST))
include $(SELF_DIR)make/toolchain.mk
ifneq ($(VERBOSE),)
$(info EXTRA_PREREQS: $(.EXTRA_PREREQS))
endif

c++_headers_location := $(shell realpath `$(CXX) --print-sysroot`)/include/c++/$(shell $(CXX) -dumpversion)
$(info c++ headers location: $(c++_headers_location))
c++_header_modules := bit
c++_header_units := $(addprefix gcm.cache/.$(c++_headers_location)/, $(c++_header_modules:%=%.gcm))

$(info c++ header units: $(c++_header_units))

vpath %.S $(SELF_DIR)src
vpath %.ld $(SELF_DIR)src
vpath %.cppm $(SELF_DIR)src
vpath %.cpp $(SELF_DIR)src $(SELF_DIR)../motorlib/peripheral/stm32g4
vpath %.h $(SELF_DIR)src $(SELF_DIR)../motorlib/CMSIS/Include

main.bin: main.elf
	$(CP) -O binary main.elf main.bin


# main.elf: main.o stm32g474.ld
# 	$(CC) -specs=nosys.specs -o main.elf main.o stm32g474.o -Tstm32g474.ld

# manual dependencies for modules
main.o: stm32g474.o trace_board.o

trace_board.o: stm32g474.o

#stm32g474.o: $(c++_header_units) #gcm.cache$(c++_headers_location)/bit.gcm

main.elf: main.o STM32G474RETx_FLASH.ld startup_stm32g474xx.o
	@echo "  LD    $@"
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) -o main.elf main.o startup_stm32g474xx.o stm32g474.o trace_board.o

# deletes implicit rule
#%.o: %.cpp

%.o: %.cpp $(c++_header_units)
	@echo "  CXX   $<"
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) -MMD -Mno-modules $(<:.d=.cpp) -o $@

%.o: %.cppm  $(c++_header_units)
	@echo "  CXXM  $<"
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(CXXMFLAGS) -MMD -Mno-modules -x c++ $(<:.d=.cppm) -o $@

# %.d: %.cpp
# 	$(CC) -c $(CFLAGS) -MM -MG -Mno-modules $< -o $@

# %.o: %.S
# 	$(CC) -c -o $@ $<


# .d files list the dependency as gcm.cache/.$(c++_headers_location)/%.gcm
gcm.cache$(c++_headers_location)/%.gcm gcm.cache/.$(c++_headers_location)/%.gcm: $(c++_headers_location)/%
	@echo "  SGCM  $*"
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(CXXMFLAGS) -x c++-system-header -c $*

gcm.cache/%.h.gcm: %.h
	@echo "  GCM   $<"
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(CXXMFLAGS) -x c++-user-header -c $<

header_units: $(c++_header_units)

load: main.bin
	@echo "Loading main.bin via DFU..."
	dfu-util -a 0 -s 0x8000000:leave -D main.bin

clean:
	@echo "Cleaning up..."
	rm -rf *.o *.elf *.bin gcm.cache/ *.d

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

-include $(wildcard $(BUILD_DIR)/*.d)

endif # build_dir

$(VERBOSE).SILENT:
