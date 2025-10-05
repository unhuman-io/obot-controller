SELF_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
.EXTRA_PREREQS := $(.EXTRA_PREREQS) $(lastword $(MAKEFILE_LIST))

vpath %.S $(SELF_DIR)
vpath %.ld $(SELF_DIR)
vpath %.cppm $(SELF_DIR)
vpath %.cpp $(SELF_DIR)
vpath %.h $(SELF_DIR)

C_INCLUDES = -I$(SELF_DIR)../../motorlib/device/stm32g4/Include -I$(SELF_DIR)../../motorlib/CMSIS/Include
LDFLAGS += -L$(SELF_DIR)

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

load: main.bin
	@echo "Loading main.bin via DFU..."
	dfu-util -a 0 -s 0x8000000:leave -D main.bin
