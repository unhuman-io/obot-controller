
usage := "USAGE: make NAME=name REV=rev"

ifndef NAME
$(error $(usage))
endif

ifndef REV
$(error $(usage))
endif

srcs = otp.cpp

C_INCLUDES = \
-I../../motorlib

C_DEFS = \
-DNAME=\"$(NAME)\" \
-DREV=\"$(REV)\"

clean_build::
	$(MAKE) clean
	$(MAKE) all
	
all: $(srcs:cpp=bin)

%.o: %.cpp
	$(CXX) $(C_INCLUDES) $(C_DEFS) -c $< $@

%.bin: %.o
	$(CP) -O binary -S -j otp $< $@ 

clean:
	rm -rf *.o *.d *.bin

program: clean_build
	dfu-util -a2 -s 0x1FFF7000:leave -D otp.bin

.PHONY: program clean_build all


TARGET_MCU := stm32g474
include ../../motorlib/make/configure_gcc.mk


#dfu-util -a2 -s 0x1FFF7000:leave -D motor_molex_mr1.bin
