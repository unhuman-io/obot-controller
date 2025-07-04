
GCC_PATH=~/gnu_tools/build-arm-none-eabi-armv7e-m+fp/install/bin/
CC = $(GCC_PATH)arm-none-eabi-g++
CP = $(GCC_PATH)arm-none-eabi-objcopy

C_INCLUDES = -I../motorlib/CMSIS/Include -I../motorlib/device/stm32g4/Include
CFLAGS = $(C_INCLUDES) -std=c++20 -fmodules-ts

headers_location := $(shell $(CC) --print-sysroot)/include/c++/$(shell $(CC) -dumpversion)
$(info Headers location: $(headers_location))
header_modules := bit
define add_header_unit_path
$(1:%=gcm.cache/.$(headers_location)/%.gcm)
endef

vpath %.cpp ../motorlib/peripheral/stm32g4
vpath %.h ../motorlib/CMSIS/Include

main.bin: main.elf
	$(CP) -O binary main.elf main.bin


# main.elf: main.o stm32g474.ld
# 	$(CC) -specs=nosys.specs -o main.elf main.o stm32g474.o -Tstm32g474.ld

# manual dependencies for modules
main.o: stm32g474.o trace_board.o

trace_board.o: stm32g474.o $(call add_header_unit_path,bit)

main.elf: main.o STM32G474RETx_FLASH.ld startup_stm32g474xx.o
	@echo "  LD    $@"
	$(CC) -specs=nosys.specs -o main.elf main.o -TSTM32G474RETx_FLASH.ld startup_stm32g474xx.o stm32g474.o trace_board.o

# some .cpps do not produce .gcm files, but this is ok
%.o gcm.cache/%.gcm: %.cpp
	@echo "  CC    $<"
	$(CC) -c $(CFLAGS) -MMD -Mno-modules $(<:.d=.cpp) -o $@

# %.d: %.cpp
# 	$(CC) -c $(CFLAGS) -MM -MG -Mno-modules $< -o $@

# %.o: %.S
# 	$(CC) -c -o $@ $<

# gcm.cache/%.gcm: %.cpp
# 	$(CC) -c $(CFLAGS) -MMD -Mno-modules $< -o $@

# gcm.cache/header_units:
# 	$(CC) -c $(CFLAGS) -fmodules-ts -x c++-system-header -c bit
# 	touch gcm.cache/header_units

gcm.cache/.$(headers_location)/%.gcm:
	@echo "  SGCM  $*"
	$(CC) -c $(CFLAGS) -fmodules-ts -x c++-system-header -c $*

gcm.cache/%.h.gcm: %.h
	@echo "  GCM   $<"
	$(CC) -c $(CFLAGS) -fmodules-ts -x c++-user-header -c $<

load: main.bin
	@echo "Loading main.bin via DFU..."
	dfu-util -a 0 -s 0x8000000:leave -D main.bin

clean:
	@echo "Cleaning up..."
	rm -rf *.o *.elf *.bin gcm.cache/ *.d

.PHONY: load clean

.EXTRA_PREREQS += Makefile

# # seems to be overriding the %.d: %.cpp rule with an implicit %.o: %.cpp rule
# MAKEFLAGS += --no-builtin-rules

-include $(wildcard *.d)

$(VERBOSE).SILENT:
