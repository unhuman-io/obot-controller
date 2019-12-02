ARM GAS  /tmp/ccmGrn2w.s 			page 1


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
  13              		.file	"usb.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.text._ZN4USBD4initEv,"ax",%progbits
  18              		.align	1
  19              		.global	_ZN4USBD4initEv
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN4USBD4initEv:
  27              		.fnstart
  28              	.LFB130:
  29              		.file 1 "Src/usb.cpp"
   1:Src/usb.cpp   **** #include "usb.h"
   2:Src/usb.cpp   **** 
   3:Src/usb.cpp   **** #include "stm32g474xx.h"
   4:Src/usb.cpp   **** 
   5:Src/usb.cpp   **** void USBD::init() {} 
  30              		.loc 1 5 19
  31              		.cfi_startproc
  32              		@ args = 0, pretend = 0, frame = 8
  33              		@ frame_needed = 1, uses_anonymous_args = 0
  34              		@ link register save eliminated.
  35 0000 80B4     		push	{r7}
  36              	.LCFI0:
  37              		.cfi_def_cfa_offset 4
  38              		.cfi_offset 7, -4
  39 0002 83B0     		sub	sp, sp, #12
  40              	.LCFI1:
  41              		.cfi_def_cfa_offset 16
  42 0004 00AF     		add	r7, sp, #0
  43              	.LCFI2:
  44              		.cfi_def_cfa_register 7
  45 0006 7860     		str	r0, [r7, #4]
  46              		.loc 1 5 20
  47 0008 00BF     		nop
  48 000a 0C37     		adds	r7, r7, #12
  49              	.LCFI3:
  50              		.cfi_def_cfa_offset 4
  51 000c BD46     		mov	sp, r7
  52              	.LCFI4:
  53              		.cfi_def_cfa_register 13
ARM GAS  /tmp/ccmGrn2w.s 			page 2


  54              		@ sp needed
  55 000e 5DF8047B 		ldr	r7, [sp], #4
  56              	.LCFI5:
  57              		.cfi_restore 7
  58              		.cfi_def_cfa_offset 0
  59 0012 7047     		bx	lr
  60              		.cfi_endproc
  61              	.LFE130:
  62              		.cantunwind
  63              		.fnend
  65              		.section	.text._ZN4USBD13usb_interruptEv,"ax",%progbits
  66              		.align	1
  67              		.global	_ZN4USBD13usb_interruptEv
  68              		.syntax unified
  69              		.thumb
  70              		.thumb_func
  71              		.fpu fpv4-sp-d16
  73              	_ZN4USBD13usb_interruptEv:
  74              		.fnstart
  75              	.LFB131:
   6:Src/usb.cpp   **** 
   7:Src/usb.cpp   **** void USBD::usb_interrupt() {
  76              		.loc 1 7 28
  77              		.cfi_startproc
  78              		@ args = 0, pretend = 0, frame = 24
  79              		@ frame_needed = 1, uses_anonymous_args = 0
  80              		@ link register save eliminated.
  81 0000 80B4     		push	{r7}
  82              	.LCFI6:
  83              		.cfi_def_cfa_offset 4
  84              		.cfi_offset 7, -4
  85 0002 87B0     		sub	sp, sp, #28
  86              	.LCFI7:
  87              		.cfi_def_cfa_offset 32
  88 0004 00AF     		add	r7, sp, #0
  89              	.LCFI8:
  90              		.cfi_def_cfa_register 7
  91 0006 7860     		str	r0, [r7, #4]
   8:Src/usb.cpp   ****     int a = 3;
  92              		.loc 1 8 9
  93 0008 0323     		movs	r3, #3
  94 000a 7B61     		str	r3, [r7, #20]
   9:Src/usb.cpp   ****     int b = a+3;
  95              		.loc 1 9 9
  96 000c 7B69     		ldr	r3, [r7, #20]
  97 000e 0333     		adds	r3, r3, #3
  98 0010 3B61     		str	r3, [r7, #16]
  10:Src/usb.cpp   ****     if (USB->ISTR & USB_ISTR_RESET) {
  99              		.loc 1 10 14
 100 0012 2D4B     		ldr	r3, .L8
 101 0014 B3F84430 		ldrh	r3, [r3, #68]	@ movhi
 102 0018 9BB2     		uxth	r3, r3
 103              		.loc 1 10 19
 104 001a 03F48063 		and	r3, r3, #1024
 105              		.loc 1 10 35
 106 001e 002B     		cmp	r3, #0
 107 0020 14BF     		ite	ne
ARM GAS  /tmp/ccmGrn2w.s 			page 3


 108 0022 0123     		movne	r3, #1
 109 0024 0023     		moveq	r3, #0
 110 0026 DBB2     		uxtb	r3, r3
 111              		.loc 1 10 5
 112 0028 002B     		cmp	r3, #0
 113 002a 11D0     		beq	.L3
  11:Src/usb.cpp   ****         USB->EP0R = USB_EP_CONTROL | USB_EP_CTR_RX | USB_EP_CTR_TX;
 114              		.loc 1 11 14
 115 002c 264B     		ldr	r3, .L8
 116              		.loc 1 11 19
 117 002e 48F28022 		movw	r2, #33408
 118 0032 1A80     		strh	r2, [r3]	@ movhi
  12:Src/usb.cpp   **** 
  13:Src/usb.cpp   ****         USB->DADDR = USB_DADDR_EF;
 119              		.loc 1 13 14
 120 0034 244B     		ldr	r3, .L8
 121              		.loc 1 13 20
 122 0036 8022     		movs	r2, #128
 123 0038 A3F84C20 		strh	r2, [r3, #76]	@ movhi
  14:Src/usb.cpp   ****         USB->ISTR &= ~USB_ISTR_RESET;
 124              		.loc 1 14 19
 125 003c 224B     		ldr	r3, .L8
 126 003e B3F84430 		ldrh	r3, [r3, #68]	@ movhi
 127 0042 9BB2     		uxth	r3, r3
 128 0044 204A     		ldr	r2, .L8
 129 0046 23F48063 		bic	r3, r3, #1024
 130 004a 9BB2     		uxth	r3, r3
 131 004c A2F84430 		strh	r3, [r2, #68]	@ movhi
 132              	.L3:
 133              	.LBB2:
  15:Src/usb.cpp   ****     }
  16:Src/usb.cpp   ****     if (USB->ISTR & USB_ISTR_CTR) { // correct transfer interrupt
 134              		.loc 1 16 14
 135 0050 1D4B     		ldr	r3, .L8
 136 0052 B3F84430 		ldrh	r3, [r3, #68]	@ movhi
 137 0056 9BB2     		uxth	r3, r3
 138              		.loc 1 16 33
 139 0058 1BB2     		sxth	r3, r3
 140 005a 9BB2     		uxth	r3, r3
 141 005c DB0B     		lsrs	r3, r3, #15
 142 005e DBB2     		uxtb	r3, r3
 143              		.loc 1 16 5
 144 0060 002B     		cmp	r3, #0
 145 0062 2AD0     		beq	.L7
 146              	.LBB3:
 147              	.LBB4:
  17:Src/usb.cpp   ****         if ((USB->ISTR & USB_ISTR_EP_ID) == 0) {    // EP0
 148              		.loc 1 17 19
 149 0064 184B     		ldr	r3, .L8
 150 0066 B3F84430 		ldrh	r3, [r3, #68]	@ movhi
 151 006a 9BB2     		uxth	r3, r3
 152              		.loc 1 17 24
 153 006c 03F00F03 		and	r3, r3, #15
 154              		.loc 1 17 42
 155 0070 002B     		cmp	r3, #0
 156 0072 0CBF     		ite	eq
 157 0074 0123     		moveq	r3, #1
ARM GAS  /tmp/ccmGrn2w.s 			page 4


 158 0076 0023     		movne	r3, #0
 159 0078 DBB2     		uxtb	r3, r3
 160              		.loc 1 17 9
 161 007a 002B     		cmp	r3, #0
 162 007c 1DD0     		beq	.L7
 163              	.LBB5:
 164              	.LBB6:
  18:Src/usb.cpp   ****             if ((USB->ISTR & USB_ISTR_DIR) == 0 ) { // 0->IN
 165              		.loc 1 18 23
 166 007e 124B     		ldr	r3, .L8
 167 0080 B3F84430 		ldrh	r3, [r3, #68]	@ movhi
 168 0084 9BB2     		uxth	r3, r3
 169              		.loc 1 18 28
 170 0086 03F01003 		and	r3, r3, #16
 171              		.loc 1 18 44
 172 008a 002B     		cmp	r3, #0
 173 008c 0CBF     		ite	eq
 174 008e 0123     		moveq	r3, #1
 175 0090 0023     		movne	r3, #0
 176 0092 DBB2     		uxtb	r3, r3
 177              		.loc 1 18 13
 178 0094 002B     		cmp	r3, #0
 179 0096 10D1     		bne	.L7
 180              	.LBB7:
 181              	.LBB8:
  19:Src/usb.cpp   ****                 
  20:Src/usb.cpp   ****             } else {    // OUT
  21:Src/usb.cpp   ****                 if (USB->EP0R & USB_EP_SETUP) {
 182              		.loc 1 21 26
 183 0098 0B4B     		ldr	r3, .L8
 184 009a 1B88     		ldrh	r3, [r3]	@ movhi
 185 009c 9BB2     		uxth	r3, r3
 186              		.loc 1 21 31
 187 009e 03F40063 		and	r3, r3, #2048
 188              		.loc 1 21 45
 189 00a2 002B     		cmp	r3, #0
 190 00a4 14BF     		ite	ne
 191 00a6 0123     		movne	r3, #1
 192 00a8 0023     		moveq	r3, #0
 193 00aa DBB2     		uxtb	r3, r3
 194              		.loc 1 21 17
 195 00ac 002B     		cmp	r3, #0
 196 00ae 04D0     		beq	.L7
 197              	.LBB9:
  22:Src/usb.cpp   ****                     int a = USB->BTABLE;
 198              		.loc 1 22 34
 199 00b0 054B     		ldr	r3, .L8
 200 00b2 B3F85030 		ldrh	r3, [r3, #80]	@ movhi
 201 00b6 9BB2     		uxth	r3, r3
 202 00b8 FB60     		str	r3, [r7, #12]
 203              	.L7:
 204              	.LBE9:
 205              	.LBE8:
 206              	.LBE7:
 207              	.LBE6:
 208              	.LBE5:
 209              	.LBE4:
ARM GAS  /tmp/ccmGrn2w.s 			page 5


 210              	.LBE3:
 211              	.LBE2:
  23:Src/usb.cpp   ****                 }
  24:Src/usb.cpp   ****             }
  25:Src/usb.cpp   ****         }
  26:Src/usb.cpp   ****     }
  27:Src/usb.cpp   **** }
 212              		.loc 1 27 1
 213 00ba 00BF     		nop
 214 00bc 1C37     		adds	r7, r7, #28
 215              	.LCFI9:
 216              		.cfi_def_cfa_offset 4
 217 00be BD46     		mov	sp, r7
 218              	.LCFI10:
 219              		.cfi_def_cfa_register 13
 220              		@ sp needed
 221 00c0 5DF8047B 		ldr	r7, [sp], #4
 222              	.LCFI11:
 223              		.cfi_restore 7
 224              		.cfi_def_cfa_offset 0
 225 00c4 7047     		bx	lr
 226              	.L9:
 227 00c6 00BF     		.align	2
 228              	.L8:
 229 00c8 005C0040 		.word	1073765376
 230              		.cfi_endproc
 231              	.LFE131:
 232              		.cantunwind
 233              		.fnend
 235              		.text
 236              	.Letext0:
 237              		.file 2 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 238              		.file 3 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 239              		.file 4 "Drivers/CMSIS/Include/core_cm4.h"
 240              		.file 5 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/system_stm32g4xx.h"
 241              		.file 6 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/stm32g474xx.h"
 242              		.file 7 "Src/usb.h"
ARM GAS  /tmp/ccmGrn2w.s 			page 6


DEFINED SYMBOLS
                            *ABS*:0000000000000000 usb.cpp
     /tmp/ccmGrn2w.s:18     .text._ZN4USBD4initEv:0000000000000000 $t
     /tmp/ccmGrn2w.s:26     .text._ZN4USBD4initEv:0000000000000000 _ZN4USBD4initEv
  .ARM.exidx.text._ZN4USBD4initEv:0000000000000000 $d
     /tmp/ccmGrn2w.s:66     .text._ZN4USBD13usb_interruptEv:0000000000000000 $t
     /tmp/ccmGrn2w.s:73     .text._ZN4USBD13usb_interruptEv:0000000000000000 _ZN4USBD13usb_interruptEv
     /tmp/ccmGrn2w.s:229    .text._ZN4USBD13usb_interruptEv:00000000000000c8 $d
.ARM.exidx.text._ZN4USBD13usb_interruptEv:0000000000000000 $d

NO UNDEFINED SYMBOLS
