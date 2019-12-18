PARAM = ../../motorlib/param_g474_boost_ninebot.c
#PARAM_GEN = 
#DFU = 

all: load clean

param: FORCE
	g++ -c param_gen.cpp
	gcc -c ../../motorlib/param.c $(PARAM)
	g++ param_gen.o param.o $(notdir $(PARAM:.c=.o)) -o param
	
param.bin: param
	./param $(PARAM_GEN)

.PHONY: load all FORCE clean

load: param.bin
	# can't seem to use :leave here
	-dfu-util -s 0x8060000 -a0 -D param.bin $(DFU)
	# :leave workaround
	-dfu-util -s 0x8000000:leave -a0 -U tmp

clean:
	rm *.o param param.bin tmp

FORCE:
