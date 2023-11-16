

srcs = $(wildcard *.c)

C_INCLUDES = \
-I../../motorlib

all:: $(srcs:c=bin)

%.bin: %.c
	$(CC) $(C_INCLUDES) -c $< $(<:c=o)
	$(CP) -O binary -S -j otp $(<:c=o) $@ 

TARGET_MCU := stm32g474
include ../../motorlib/make/configure_gcc.mk

#dfu-util -a2 -s 0x1FFF7000:leave -D motor_molex_mr1.bin
