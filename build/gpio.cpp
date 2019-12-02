ARM GAS  /tmp/cc2YVys9.s 			page 1


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
  13              		.file	"gpio.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.text._ZN4GPIOC2ER12GPIO_TypeDefhNS_9DirectionE,"ax",%progbits
  18              		.align	1
  19              		.global	_ZN4GPIOC2ER12GPIO_TypeDefhNS_9DirectionE
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN4GPIOC2ER12GPIO_TypeDefhNS_9DirectionE:
  27              		.fnstart
  28              	.LFB131:
  29              		.file 1 "motorlib/gpio.cpp"
   1:motorlib/gpio.cpp **** #include "gpio.h"
   2:motorlib/gpio.cpp **** 
   3:motorlib/gpio.cpp **** GPIO::GPIO(GPIO_TypeDef &regs, uint8_t pin, Direction direction) :
  30              		.loc 1 3 1
  31              		.cfi_startproc
  32              		@ args = 0, pretend = 0, frame = 16
  33              		@ frame_needed = 1, uses_anonymous_args = 0
  34 0000 80B5     		push	{r7, lr}
  35              	.LCFI0:
  36              		.cfi_def_cfa_offset 8
  37              		.cfi_offset 7, -8
  38              		.cfi_offset 14, -4
  39 0002 84B0     		sub	sp, sp, #16
  40              	.LCFI1:
  41              		.cfi_def_cfa_offset 24
  42 0004 00AF     		add	r7, sp, #0
  43              	.LCFI2:
  44              		.cfi_def_cfa_register 7
  45 0006 F860     		str	r0, [r7, #12]
  46 0008 B960     		str	r1, [r7, #8]
  47 000a 1146     		mov	r1, r2
  48 000c 1A46     		mov	r2, r3
  49 000e 0B46     		mov	r3, r1
  50 0010 FB71     		strb	r3, [r7, #7]
  51 0012 1346     		mov	r3, r2
  52 0014 BB71     		strb	r3, [r7, #6]
  53              	.LBB2:
   4:motorlib/gpio.cpp ****     regs_(regs), mask_(1<<pin) {
  54              		.loc 1 4 30
ARM GAS  /tmp/cc2YVys9.s 			page 2


  55 0016 FB68     		ldr	r3, [r7, #12]
  56 0018 BA68     		ldr	r2, [r7, #8]
  57 001a 1A60     		str	r2, [r3]
  58              		.loc 1 4 25
  59 001c FB79     		ldrb	r3, [r7, #7]	@ zero_extendqisi2
  60 001e 0122     		movs	r2, #1
  61 0020 02FA03F3 		lsl	r3, r2, r3
  62 0024 1A46     		mov	r2, r3
  63              		.loc 1 4 30
  64 0026 FB68     		ldr	r3, [r7, #12]
  65 0028 5A60     		str	r2, [r3, #4]
   5:motorlib/gpio.cpp ****         set_direction(direction);
  66              		.loc 1 5 22
  67 002a BB79     		ldrb	r3, [r7, #6]	@ zero_extendqisi2
  68 002c 1946     		mov	r1, r3
  69 002e F868     		ldr	r0, [r7, #12]
  70 0030 FFF7FEFF 		bl	_ZN4GPIO13set_directionENS_9DirectionE
  71              	.LBE2:
   6:motorlib/gpio.cpp ****     }
  72              		.loc 1 6 5
  73 0034 FB68     		ldr	r3, [r7, #12]
  74 0036 1846     		mov	r0, r3
  75 0038 1037     		adds	r7, r7, #16
  76              	.LCFI3:
  77              		.cfi_def_cfa_offset 8
  78 003a BD46     		mov	sp, r7
  79              	.LCFI4:
  80              		.cfi_def_cfa_register 13
  81              		@ sp needed
  82 003c 80BD     		pop	{r7, pc}
  83              		.cfi_endproc
  84              	.LFE131:
  85              		.cantunwind
  86              		.fnend
  88              		.global	_ZN4GPIOC1ER12GPIO_TypeDefhNS_9DirectionE
  89              		.thumb_set _ZN4GPIOC1ER12GPIO_TypeDefhNS_9DirectionE,_ZN4GPIOC2ER12GPIO_TypeDefhNS_9DirectionE
  90              		.section	.text._ZN4GPIO3setEv,"ax",%progbits
  91              		.align	1
  92              		.global	_ZN4GPIO3setEv
  93              		.syntax unified
  94              		.thumb
  95              		.thumb_func
  96              		.fpu fpv4-sp-d16
  98              	_ZN4GPIO3setEv:
  99              		.fnstart
 100              	.LFB133:
   7:motorlib/gpio.cpp **** 
   8:motorlib/gpio.cpp **** void GPIO::set() {
 101              		.loc 1 8 18
 102              		.cfi_startproc
 103              		@ args = 0, pretend = 0, frame = 8
 104              		@ frame_needed = 1, uses_anonymous_args = 0
 105              		@ link register save eliminated.
 106 0000 80B4     		push	{r7}
 107              	.LCFI5:
 108              		.cfi_def_cfa_offset 4
 109              		.cfi_offset 7, -4
ARM GAS  /tmp/cc2YVys9.s 			page 3


 110 0002 83B0     		sub	sp, sp, #12
 111              	.LCFI6:
 112              		.cfi_def_cfa_offset 16
 113 0004 00AF     		add	r7, sp, #0
 114              	.LCFI7:
 115              		.cfi_def_cfa_register 7
 116 0006 7860     		str	r0, [r7, #4]
   9:motorlib/gpio.cpp ****     regs_.BSRR |= mask_;
 117              		.loc 1 9 5
 118 0008 7B68     		ldr	r3, [r7, #4]
 119 000a 1B68     		ldr	r3, [r3]
 120              		.loc 1 9 16
 121 000c 9969     		ldr	r1, [r3, #24]
 122              		.loc 1 9 19
 123 000e 7B68     		ldr	r3, [r7, #4]
 124 0010 5A68     		ldr	r2, [r3, #4]
 125              		.loc 1 9 5
 126 0012 7B68     		ldr	r3, [r7, #4]
 127 0014 1B68     		ldr	r3, [r3]
 128              		.loc 1 9 16
 129 0016 0A43     		orrs	r2, r2, r1
 130 0018 9A61     		str	r2, [r3, #24]
  10:motorlib/gpio.cpp **** }
 131              		.loc 1 10 1
 132 001a 00BF     		nop
 133 001c 0C37     		adds	r7, r7, #12
 134              	.LCFI8:
 135              		.cfi_def_cfa_offset 4
 136 001e BD46     		mov	sp, r7
 137              	.LCFI9:
 138              		.cfi_def_cfa_register 13
 139              		@ sp needed
 140 0020 5DF8047B 		ldr	r7, [sp], #4
 141              	.LCFI10:
 142              		.cfi_restore 7
 143              		.cfi_def_cfa_offset 0
 144 0024 7047     		bx	lr
 145              		.cfi_endproc
 146              	.LFE133:
 147              		.cantunwind
 148              		.fnend
 150              		.section	.text._ZN4GPIO5clearEv,"ax",%progbits
 151              		.align	1
 152              		.global	_ZN4GPIO5clearEv
 153              		.syntax unified
 154              		.thumb
 155              		.thumb_func
 156              		.fpu fpv4-sp-d16
 158              	_ZN4GPIO5clearEv:
 159              		.fnstart
 160              	.LFB134:
  11:motorlib/gpio.cpp **** 
  12:motorlib/gpio.cpp **** void GPIO::clear() {
 161              		.loc 1 12 20
 162              		.cfi_startproc
 163              		@ args = 0, pretend = 0, frame = 8
 164              		@ frame_needed = 1, uses_anonymous_args = 0
ARM GAS  /tmp/cc2YVys9.s 			page 4


 165              		@ link register save eliminated.
 166 0000 80B4     		push	{r7}
 167              	.LCFI11:
 168              		.cfi_def_cfa_offset 4
 169              		.cfi_offset 7, -4
 170 0002 83B0     		sub	sp, sp, #12
 171              	.LCFI12:
 172              		.cfi_def_cfa_offset 16
 173 0004 00AF     		add	r7, sp, #0
 174              	.LCFI13:
 175              		.cfi_def_cfa_register 7
 176 0006 7860     		str	r0, [r7, #4]
  13:motorlib/gpio.cpp ****     regs_.BSRR |= mask_ << 16;
 177              		.loc 1 13 5
 178 0008 7B68     		ldr	r3, [r7, #4]
 179 000a 1B68     		ldr	r3, [r3]
 180              		.loc 1 13 16
 181 000c 9969     		ldr	r1, [r3, #24]
 182              		.loc 1 13 19
 183 000e 7B68     		ldr	r3, [r7, #4]
 184 0010 5B68     		ldr	r3, [r3, #4]
 185              		.loc 1 13 25
 186 0012 1A04     		lsls	r2, r3, #16
 187              		.loc 1 13 5
 188 0014 7B68     		ldr	r3, [r7, #4]
 189 0016 1B68     		ldr	r3, [r3]
 190              		.loc 1 13 16
 191 0018 0A43     		orrs	r2, r2, r1
 192 001a 9A61     		str	r2, [r3, #24]
  14:motorlib/gpio.cpp **** }
 193              		.loc 1 14 1
 194 001c 00BF     		nop
 195 001e 0C37     		adds	r7, r7, #12
 196              	.LCFI14:
 197              		.cfi_def_cfa_offset 4
 198 0020 BD46     		mov	sp, r7
 199              	.LCFI15:
 200              		.cfi_def_cfa_register 13
 201              		@ sp needed
 202 0022 5DF8047B 		ldr	r7, [sp], #4
 203              	.LCFI16:
 204              		.cfi_restore 7
 205              		.cfi_def_cfa_offset 0
 206 0026 7047     		bx	lr
 207              		.cfi_endproc
 208              	.LFE134:
 209              		.cantunwind
 210              		.fnend
 212              		.section	.text._ZN4GPIO13set_directionENS_9DirectionE,"ax",%progbits
 213              		.align	1
 214              		.global	_ZN4GPIO13set_directionENS_9DirectionE
 215              		.syntax unified
 216              		.thumb
 217              		.thumb_func
 218              		.fpu fpv4-sp-d16
 220              	_ZN4GPIO13set_directionENS_9DirectionE:
 221              		.fnstart
ARM GAS  /tmp/cc2YVys9.s 			page 5


 222              	.LFB135:
  15:motorlib/gpio.cpp **** 
  16:motorlib/gpio.cpp **** 
  17:motorlib/gpio.cpp **** // TODO
  18:motorlib/gpio.cpp **** void GPIO::set_direction(Direction direction) {
 223              		.loc 1 18 47
 224              		.cfi_startproc
 225              		@ args = 0, pretend = 0, frame = 8
 226              		@ frame_needed = 1, uses_anonymous_args = 0
 227              		@ link register save eliminated.
 228 0000 80B4     		push	{r7}
 229              	.LCFI17:
 230              		.cfi_def_cfa_offset 4
 231              		.cfi_offset 7, -4
 232 0002 83B0     		sub	sp, sp, #12
 233              	.LCFI18:
 234              		.cfi_def_cfa_offset 16
 235 0004 00AF     		add	r7, sp, #0
 236              	.LCFI19:
 237              		.cfi_def_cfa_register 7
 238 0006 7860     		str	r0, [r7, #4]
 239 0008 0B46     		mov	r3, r1
 240 000a FB70     		strb	r3, [r7, #3]
  19:motorlib/gpio.cpp ****     switch (direction) {
 241              		.loc 1 19 5
 242 000c FB78     		ldrb	r3, [r7, #3]	@ zero_extendqisi2
 243 000e 012B     		cmp	r3, #1
  20:motorlib/gpio.cpp ****         default:
  21:motorlib/gpio.cpp ****         case INPUT:
  22:motorlib/gpio.cpp ****         break;
  23:motorlib/gpio.cpp ****         case OUTPUT:
  24:motorlib/gpio.cpp ****         break;
 244              		.loc 1 24 9
 245 0010 00BF     		nop
  25:motorlib/gpio.cpp ****     }
  26:motorlib/gpio.cpp **** }...
 246              		.loc 1 26 1
 247 0012 00BF     		nop
 248 0014 0C37     		adds	r7, r7, #12
 249              	.LCFI20:
 250              		.cfi_def_cfa_offset 4
 251 0016 BD46     		mov	sp, r7
 252              	.LCFI21:
 253              		.cfi_def_cfa_register 13
 254              		@ sp needed
 255 0018 5DF8047B 		ldr	r7, [sp], #4
 256              	.LCFI22:
 257              		.cfi_restore 7
 258              		.cfi_def_cfa_offset 0
 259 001c 7047     		bx	lr
 260              		.cfi_endproc
 261              	.LFE135:
 262              		.cantunwind
 263              		.fnend
 265              		.text
 266              	.Letext0:
 267              		.file 2 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdint"
ARM GAS  /tmp/cc2YVys9.s 			page 6


 268              		.file 3 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thu
 269              		.file 4 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 270              		.file 5 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 271              		.file 6 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdint.h"
 272              		.file 7 "Drivers/CMSIS/Include/core_cm4.h"
 273              		.file 8 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/system_stm32g4xx.h"
 274              		.file 9 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/stm32g474xx.h"
 275              		.file 10 "motorlib/gpio.h"
 276              		.file 11 "<built-in>"
ARM GAS  /tmp/cc2YVys9.s 			page 7


DEFINED SYMBOLS
                            *ABS*:0000000000000000 gpio.cpp
     /tmp/cc2YVys9.s:18     .text._ZN4GPIOC2ER12GPIO_TypeDefhNS_9DirectionE:0000000000000000 $t
     /tmp/cc2YVys9.s:26     .text._ZN4GPIOC2ER12GPIO_TypeDefhNS_9DirectionE:0000000000000000 _ZN4GPIOC2ER12GPIO_TypeDefhNS_9DirectionE
     /tmp/cc2YVys9.s:220    .text._ZN4GPIO13set_directionENS_9DirectionE:0000000000000000 _ZN4GPIO13set_directionENS_9DirectionE
.ARM.exidx.text._ZN4GPIOC2ER12GPIO_TypeDefhNS_9DirectionE:0000000000000000 $d
     /tmp/cc2YVys9.s:26     .text._ZN4GPIOC2ER12GPIO_TypeDefhNS_9DirectionE:0000000000000000 _ZN4GPIOC1ER12GPIO_TypeDefhNS_9DirectionE
     /tmp/cc2YVys9.s:91     .text._ZN4GPIO3setEv:0000000000000000 $t
     /tmp/cc2YVys9.s:98     .text._ZN4GPIO3setEv:0000000000000000 _ZN4GPIO3setEv
   .ARM.exidx.text._ZN4GPIO3setEv:0000000000000000 $d
     /tmp/cc2YVys9.s:151    .text._ZN4GPIO5clearEv:0000000000000000 $t
     /tmp/cc2YVys9.s:158    .text._ZN4GPIO5clearEv:0000000000000000 _ZN4GPIO5clearEv
 .ARM.exidx.text._ZN4GPIO5clearEv:0000000000000000 $d
     /tmp/cc2YVys9.s:213    .text._ZN4GPIO13set_directionENS_9DirectionE:0000000000000000 $t
.ARM.exidx.text._ZN4GPIO13set_directionENS_9DirectionE:0000000000000000 $d

NO UNDEFINED SYMBOLS
