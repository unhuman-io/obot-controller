ARM GAS  /tmp/ccDFMiuv.s 			page 1


   1              		.cpu cortex-m4
   2              		.eabi_attribute 27, 1
   3              		.eabi_attribute 28, 1
   4              		.eabi_attribute 20, 1
   5              		.eabi_attribute 21, 1
   6              		.eabi_attribute 23, 3
   7              		.eabi_attribute 24, 1
   8              		.eabi_attribute 25, 1
   9              		.eabi_attribute 26, 1
  10              		.eabi_attribute 30, 6
  11              		.eabi_attribute 34, 1
  12              		.eabi_attribute 18, 4
  13              		.file	"spi_encoder.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.ccmram,"ax",%progbits
  18              		.align	1
  19              		.global	_ZN10SPIEncoder9get_valueEv
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN10SPIEncoder9get_valueEv:
  27              		.fnstart
  28              	.LFB139:
  29              		.file 1 "motorlib/spi_encoder.cpp"
   1:motorlib/spi_encoder.cpp **** #include "spi_encoder.h"
   2:motorlib/spi_encoder.cpp **** #include "gpio.h"
   3:motorlib/spi_encoder.cpp **** 
   4:motorlib/spi_encoder.cpp **** void SPIEncoder::trigger() {
   5:motorlib/spi_encoder.cpp ****     gpio_cs_.clear();
   6:motorlib/spi_encoder.cpp ****     asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
   7:motorlib/spi_encoder.cpp ****     // wait tdelay, 80 ns ma732 (14.4 cycles at 180 MHz)
   8:motorlib/spi_encoder.cpp ****     // 300 ns AEAT-8800
   9:motorlib/spi_encoder.cpp ****     regs_.DR = 0;
  10:motorlib/spi_encoder.cpp **** }
  11:motorlib/spi_encoder.cpp **** 
  12:motorlib/spi_encoder.cpp **** int32_t SPIEncoder::get_value() {
  30              		.loc 1 12 33
  31              		.cfi_startproc
  32              		@ args = 0, pretend = 0, frame = 16
  33              		@ frame_needed = 1, uses_anonymous_args = 0
  34 0000 80B5     		push	{r7, lr}
  35              		.save {r7, lr}
  36              	.LCFI0:
  37              		.cfi_def_cfa_offset 8
  38              		.cfi_offset 7, -8
  39              		.cfi_offset 14, -4
  40              		.pad #16
  41 0002 84B0     		sub	sp, sp, #16
  42              	.LCFI1:
  43              		.cfi_def_cfa_offset 24
  44              		.setfp r7, sp, #0
  45 0004 00AF     		add	r7, sp, #0
  46              	.LCFI2:
ARM GAS  /tmp/ccDFMiuv.s 			page 2


  47              		.cfi_def_cfa_register 7
  48 0006 7860     		str	r0, [r7, #4]
  49              	.L3:
  13:motorlib/spi_encoder.cpp ****     // wait until SPI_FLAG_RXNE, ma 732 max frequency 25 Mbps, 640 ns (115 cycles at 180 MHz)
  14:motorlib/spi_encoder.cpp ****     while(!(regs_.SR & SPI_SR_RXNE)); // RXNE: 1 -> data available
  50              		.loc 1 14 13
  51 0008 7B68     		ldr	r3, [r7, #4]
  52 000a 9B68     		ldr	r3, [r3, #8]
  53              		.loc 1 14 19
  54 000c 9B68     		ldr	r3, [r3, #8]
  55              		.loc 1 14 22
  56 000e 03F00103 		and	r3, r3, #1
  57              		.loc 1 14 11
  58 0012 002B     		cmp	r3, #0
  59 0014 0CBF     		ite	eq
  60 0016 0123     		moveq	r3, #1
  61 0018 0023     		movne	r3, #0
  62 001a DBB2     		uxtb	r3, r3
  63 001c 002B     		cmp	r3, #0
  64 001e 00D0     		beq	.L2
  65              		.loc 1 14 5
  66 0020 F2E7     		b	.L3
  67              	.L2:
  15:motorlib/spi_encoder.cpp ****     int16_t data = regs_.DR;
  68              		.loc 1 15 20
  69 0022 7B68     		ldr	r3, [r7, #4]
  70 0024 9B68     		ldr	r3, [r3, #8]
  71              		.loc 1 15 26
  72 0026 DB68     		ldr	r3, [r3, #12]
  73 0028 FB81     		strh	r3, [r7, #14]	@ movhi
  16:motorlib/spi_encoder.cpp ****     // wait tdelay, 25 ns ma732
  17:motorlib/spi_encoder.cpp ****     // 200 ns AEAT-8800
  18:motorlib/spi_encoder.cpp ****     asm("NOP; NOP; NOP; NOP;");
  74              		.loc 1 18 31
  75              		.syntax unified
  76              	@ 18 "motorlib/spi_encoder.cpp" 1
  77 002a 00BF00BF 		NOP; NOP; NOP; NOP;
  77      00BF00BF 
  78              	@ 0 "" 2
  19:motorlib/spi_encoder.cpp ****     gpio_cs_.set();
  79              		.loc 1 19 5
  80              		.thumb
  81              		.syntax unified
  82 0032 7B68     		ldr	r3, [r7, #4]
  83 0034 DB68     		ldr	r3, [r3, #12]
  84              		.loc 1 19 17
  85 0036 1846     		mov	r0, r3
  86 0038 FFF7FEFF 		bl	_ZN4GPIO3setEv
  20:motorlib/spi_encoder.cpp ****     return data;
  87              		.loc 1 20 12
  88 003c B7F90E30 		ldrsh	r3, [r7, #14]
  21:motorlib/spi_encoder.cpp **** }...
  89              		.loc 1 21 1
  90 0040 1846     		mov	r0, r3
  91 0042 1037     		adds	r7, r7, #16
  92              	.LCFI3:
  93              		.cfi_def_cfa_offset 8
ARM GAS  /tmp/ccDFMiuv.s 			page 3


  94 0044 BD46     		mov	sp, r7
  95              	.LCFI4:
  96              		.cfi_def_cfa_register 13
  97              		@ sp needed
  98 0046 80BD     		pop	{r7, pc}
  99              		.cfi_endproc
 100              	.LFE139:
 101              		.fnend
 103              		.align	1
 104              		.global	_ZN10SPIEncoder7triggerEv
 105              		.syntax unified
 106              		.thumb
 107              		.thumb_func
 108              		.fpu fpv4-sp-d16
 110              	_ZN10SPIEncoder7triggerEv:
 111              		.fnstart
 112              	.LFB138:
   4:motorlib/spi_encoder.cpp **** void SPIEncoder::trigger() {
 113              		.loc 1 4 28
 114              		.cfi_startproc
 115              		@ args = 0, pretend = 0, frame = 8
 116              		@ frame_needed = 1, uses_anonymous_args = 0
 117 0048 80B5     		push	{r7, lr}
 118              		.save {r7, lr}
 119              	.LCFI5:
 120              		.cfi_def_cfa_offset 8
 121              		.cfi_offset 7, -8
 122              		.cfi_offset 14, -4
 123              		.pad #8
 124 004a 82B0     		sub	sp, sp, #8
 125              	.LCFI6:
 126              		.cfi_def_cfa_offset 16
 127              		.setfp r7, sp, #0
 128 004c 00AF     		add	r7, sp, #0
 129              	.LCFI7:
 130              		.cfi_def_cfa_register 7
 131 004e 7860     		str	r0, [r7, #4]
   5:motorlib/spi_encoder.cpp ****     gpio_cs_.clear();
 132              		.loc 1 5 5
 133 0050 7B68     		ldr	r3, [r7, #4]
 134 0052 DB68     		ldr	r3, [r3, #12]
   5:motorlib/spi_encoder.cpp ****     gpio_cs_.clear();
 135              		.loc 1 5 19
 136 0054 1846     		mov	r0, r3
 137 0056 FFF7FEFF 		bl	_ZN4GPIO5clearEv
   6:motorlib/spi_encoder.cpp ****     asm("NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;");
 138              		.loc 1 6 76
 139              		.syntax unified
 140              	@ 6 "motorlib/spi_encoder.cpp" 1
 141 005a 00BF00BF 		NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP; NOP;
 141      00BF00BF 
 141      00BF00BF 
 141      00BF00BF 
 141      00BF00BF 
 142              	@ 0 "" 2
   9:motorlib/spi_encoder.cpp ****     regs_.DR = 0;
 143              		.loc 1 9 5
ARM GAS  /tmp/ccDFMiuv.s 			page 4


 144              		.thumb
 145              		.syntax unified
 146 0074 7B68     		ldr	r3, [r7, #4]
 147 0076 9B68     		ldr	r3, [r3, #8]
   9:motorlib/spi_encoder.cpp ****     regs_.DR = 0;
 148              		.loc 1 9 14
 149 0078 0022     		movs	r2, #0
 150 007a DA60     		str	r2, [r3, #12]
  10:motorlib/spi_encoder.cpp **** }
 151              		.loc 1 10 1
 152 007c 00BF     		nop
 153 007e 0837     		adds	r7, r7, #8
 154              	.LCFI8:
 155              		.cfi_def_cfa_offset 8
 156 0080 BD46     		mov	sp, r7
 157              	.LCFI9:
 158              		.cfi_def_cfa_register 13
 159              		@ sp needed
 160 0082 80BD     		pop	{r7, pc}
 161              		.cfi_endproc
 162              	.LFE138:
 163              		.fnend
 165              		.global	_ZTV10SPIEncoder
 166              		.section	.rodata._ZTV10SPIEncoder,"a"
 167              		.align	2
 170              	_ZTV10SPIEncoder:
 171 0000 00000000 		.word	0
 172 0004 00000000 		.word	_ZTI10SPIEncoder
 173 0008 00000000 		.word	_ZN10SPIEncoder9get_valueEv
 174 000c 00000000 		.word	_ZN10SPIEncoder7triggerEv
 175              		.global	_ZTI10SPIEncoder
 176              		.section	.rodata._ZTI10SPIEncoder,"a"
 177              		.align	2
 180              	_ZTI10SPIEncoder:
 181 0000 08000000 		.word	_ZTVN10__cxxabiv120__si_class_type_infoE+8
 182 0004 00000000 		.word	_ZTS10SPIEncoder
 183 0008 00000000 		.word	_ZTI7Encoder
 184              		.global	_ZTS10SPIEncoder
 185              		.section	.rodata._ZTS10SPIEncoder,"a"
 186              		.align	2
 189              	_ZTS10SPIEncoder:
 190 0000 31305350 		.ascii	"10SPIEncoder\000"
 190      49456E63 
 190      6F646572 
 190      00
 191              		.weak	_ZTI7Encoder
 192              		.section	.rodata._ZTI7Encoder,"aG",%progbits,_ZTI7Encoder,comdat
 193              		.align	2
 196              	_ZTI7Encoder:
 197 0000 08000000 		.word	_ZTVN10__cxxabiv117__class_type_infoE+8
 198 0004 00000000 		.word	_ZTS7Encoder
 199              		.weak	_ZTS7Encoder
 200              		.section	.rodata._ZTS7Encoder,"aG",%progbits,_ZTS7Encoder,comdat
 201              		.align	2
 204              	_ZTS7Encoder:
 205 0000 37456E63 		.ascii	"7Encoder\000"
 205      6F646572 
ARM GAS  /tmp/ccDFMiuv.s 			page 5


 205      00
 206              		.text
 207              	.Letext0:
 208              		.file 2 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdint"
 209              		.file 3 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thu
 210              		.file 4 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 211              		.file 5 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 212              		.file 6 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdint.h"
 213              		.file 7 "Drivers/CMSIS/Include/core_cm4.h"
 214              		.file 8 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/system_stm32g4xx.h"
 215              		.file 9 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/stm32g474xx.h"
 216              		.file 10 "motorlib/gpio.h"
 217              		.file 11 "motorlib/spi_encoder.h"
 218              		.file 12 "motorlib/encoder.h"
 219              		.file 13 "<built-in>"
ARM GAS  /tmp/ccDFMiuv.s 			page 6


DEFINED SYMBOLS
                            *ABS*:0000000000000000 spi_encoder.cpp
     /tmp/ccDFMiuv.s:18     .ccmram:0000000000000000 $t
     /tmp/ccDFMiuv.s:26     .ccmram:0000000000000000 _ZN10SPIEncoder9get_valueEv
                .ARM.extab.ccmram:0000000000000000 $d
                .ARM.exidx.ccmram:0000000000000000 $d
     /tmp/ccDFMiuv.s:110    .ccmram:0000000000000048 _ZN10SPIEncoder7triggerEv
     /tmp/ccDFMiuv.s:170    .rodata._ZTV10SPIEncoder:0000000000000000 _ZTV10SPIEncoder
     /tmp/ccDFMiuv.s:167    .rodata._ZTV10SPIEncoder:0000000000000000 $d
     /tmp/ccDFMiuv.s:180    .rodata._ZTI10SPIEncoder:0000000000000000 _ZTI10SPIEncoder
     /tmp/ccDFMiuv.s:177    .rodata._ZTI10SPIEncoder:0000000000000000 $d
     /tmp/ccDFMiuv.s:189    .rodata._ZTS10SPIEncoder:0000000000000000 _ZTS10SPIEncoder
     /tmp/ccDFMiuv.s:196    .rodata._ZTI7Encoder:0000000000000000 _ZTI7Encoder
     /tmp/ccDFMiuv.s:186    .rodata._ZTS10SPIEncoder:0000000000000000 $d
     /tmp/ccDFMiuv.s:193    .rodata._ZTI7Encoder:0000000000000000 $d
     /tmp/ccDFMiuv.s:204    .rodata._ZTS7Encoder:0000000000000000 _ZTS7Encoder
     /tmp/ccDFMiuv.s:201    .rodata._ZTS7Encoder:0000000000000000 $d

UNDEFINED SYMBOLS
_ZN4GPIO3setEv
__aeabi_unwind_cpp_pr1
_ZN4GPIO5clearEv
_ZTVN10__cxxabiv120__si_class_type_infoE
_ZTVN10__cxxabiv117__class_type_infoE
