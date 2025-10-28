ifeq ($(USE_LLVM),1)
BUILD_DIR ?= build_llvm
else
BUILD_DIR ?= build
endif

ifneq ($(VERBOSE),)
$(info BUILD_DIR: $(shell realpath $(BUILD_DIR)))
$(info CURDIR: $(CURDIR))
$(info MAKEFILE_LIST: $(MAKEFILE_LIST))
endif
ifneq ($(shell realpath $(BUILD_DIR)), $(CURDIR))
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	echo "override BUILD_DIR := .\nUSE_LLVM := $(USE_LLVM)\ninclude ../Makefile" > $(BUILD_DIR)/Makefile
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

c++_header_modules := bit cstdint string

$(info c++ header units: $(c++_header_units))

# deletes implicit rule
#%.o: %.cpp

obot_std.o: $(SELF_DIR)obot_std.cppm $(c++_header_units)
	echo "  CXXM  $<"
	$(CXX) -c $(C_INCLUDES) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(CXXMFLAGS) $(obot_std_flags) -MMD $(<:.d=.cppm) -o $@ 

%.o: %.cpp obot_std.o
	@echo "  CXX   $<"
	$(CXX) -c $(C_INCLUDES) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) -MMD $(<:.d=.cpp) -o $@

gcm.cache/%.gcm %.pcm %.o: %.cppm obot_std.o
	@echo "  CXXM  $<"
	$(CXX) -c $(C_INCLUDES) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(CXXMFLAGS) -MMD $(<:.d=.cppm) -o $@

%.o: %.S
	@echo "  AS    $<"
	$(CXX) $(CPPFLAGS) -c -o $@ $<

%.elf: %.o
	@echo "  LD    $@"
	$(CXX) $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $(filter-out %.ld,$^)

%.bin: %.elf
	@echo "  CP    $@"
	$(CP) -O binary $< $@
	$(SZ) $<

# .d files list the dependency as gcm.cache/.$(c++_headers_location)/%.gcm
# gcc c++20 system header unit modules
gcm.cache$(c++_headers_location)/%.gcm gcm.cache/.$(c++_headers_location)/%.gcm: $(c++_headers_location)/%
	@echo "  SGCM  $*"
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(CXXMFLAGS) -x c++-system-header -c $*

# gcc c++20 user head unit modules
gcm.cache/%.h.gcm: %.h
	@echo "  GCM   $<"
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(CXXMFLAGS) -x c++-user-header -c $<

# clang c++20 system header unit modules
%.pcm: $(c++_headers_location)/%
	@echo "  SPCM  $*"
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $(CXXFLAGS) $(CXXMFLAGS) -Wno-user-defined-literals -x c++-system-header -c $*

header_units: $(c++_header_units)

clean:
	@echo "Cleaning up..."
	rm -rf *.o *.elf *.bin gcm.cache/ *.d *.pcm

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


.PHONY: clean help header_units

include $(SELF_DIR)src/Makefile
-include $(wildcard $(BUILD_DIR)/*.d)

endif # build_dir

$(VERBOSE).SILENT:
