
main.elf: main.cpp
	../motorlib/llvm/bin/clang++ -target thumbv7m-none-eabi -mcpu=cortex-m4 -Tpicolibcpp.ld -o main.elf main.cpp \
		--std=c++20 -fno-use-cxa-atexit -Wl,-gc-sections,--defsym=__boot_flash=0x8000000,--defsym=__flash=0x8001000,--defsym=__ram=0x20000000 -flto -O3 \
		-fno-rtti -fno-exceptions -g

.EXTRA_PREREQS = Makefile
