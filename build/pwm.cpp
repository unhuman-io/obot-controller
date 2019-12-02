ARM GAS  /tmp/ccLa5IHU.s 			page 1


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
  13              		.file	"pwm.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.ccmram,"ax",%progbits
  18              		.align	1
  19              		.global	_ZN3PWM11set_voltageEPf
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN3PWM11set_voltageEPf:
  27              		.fnstart
  28              	.LFB409:
  29              		.file 1 "motorlib/pwm.cpp"
   1:motorlib/pwm.cpp **** 
   2:motorlib/pwm.cpp **** #include "pwm.h"
   3:motorlib/pwm.cpp **** #include "../st_device.h"
   4:motorlib/pwm.cpp **** #include "gpio.h"
   5:motorlib/pwm.cpp **** #include "main.h"
   6:motorlib/pwm.cpp **** 
   7:motorlib/pwm.cpp **** void PWM::set_voltage(float v_abc[3]) {
  30              		.loc 1 7 39
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
  46 0008 3960     		str	r1, [r7]
   8:motorlib/pwm.cpp ****     pwm_a_ = v_abc[0] * v_to_pwm_ + half_period_;
  47              		.loc 1 8 21
  48 000a 3B68     		ldr	r3, [r7]
  49 000c 93ED007A 		vldr.32	s14, [r3]
  50              		.loc 1 8 25
ARM GAS  /tmp/ccLa5IHU.s 			page 2


  51 0010 7B68     		ldr	r3, [r7, #4]
  52 0012 D3ED067A 		vldr.32	s15, [r3, #24]
  53              		.loc 1 8 23
  54 0016 27EE277A 		vmul.f32	s14, s14, s15
  55              		.loc 1 8 37
  56 001a 7B68     		ldr	r3, [r7, #4]
  57 001c 5B88     		ldrh	r3, [r3, #2]
  58 001e 07EE903A 		vmov	s15, r3	@ int
  59              		.loc 1 8 35
  60 0022 F8EEE77A 		vcvt.f32.s32	s15, s15
  61 0026 77EE277A 		vadd.f32	s15, s14, s15
  62              		.loc 1 8 5
  63 002a 7B68     		ldr	r3, [r7, #4]
  64 002c 5B68     		ldr	r3, [r3, #4]
  65              		.loc 1 8 12
  66 002e FCEEE77A 		vcvt.u32.f32	s15, s15
  67 0032 17EE902A 		vmov	r2, s15	@ int
  68 0036 1A60     		str	r2, [r3]
   9:motorlib/pwm.cpp ****     pwm_b_ = v_abc[1] * v_to_pwm_ + half_period_;
  69              		.loc 1 9 21
  70 0038 3B68     		ldr	r3, [r7]
  71 003a 0433     		adds	r3, r3, #4
  72 003c 93ED007A 		vldr.32	s14, [r3]
  73              		.loc 1 9 25
  74 0040 7B68     		ldr	r3, [r7, #4]
  75 0042 D3ED067A 		vldr.32	s15, [r3, #24]
  76              		.loc 1 9 23
  77 0046 27EE277A 		vmul.f32	s14, s14, s15
  78              		.loc 1 9 37
  79 004a 7B68     		ldr	r3, [r7, #4]
  80 004c 5B88     		ldrh	r3, [r3, #2]
  81 004e 07EE903A 		vmov	s15, r3	@ int
  82              		.loc 1 9 35
  83 0052 F8EEE77A 		vcvt.f32.s32	s15, s15
  84 0056 77EE277A 		vadd.f32	s15, s14, s15
  85              		.loc 1 9 5
  86 005a 7B68     		ldr	r3, [r7, #4]
  87 005c 9B68     		ldr	r3, [r3, #8]
  88              		.loc 1 9 12
  89 005e FCEEE77A 		vcvt.u32.f32	s15, s15
  90 0062 17EE902A 		vmov	r2, s15	@ int
  91 0066 1A60     		str	r2, [r3]
  10:motorlib/pwm.cpp ****     pwm_c_ = v_abc[2] * v_to_pwm_ + half_period_;
  92              		.loc 1 10 21
  93 0068 3B68     		ldr	r3, [r7]
  94 006a 0833     		adds	r3, r3, #8
  95 006c 93ED007A 		vldr.32	s14, [r3]
  96              		.loc 1 10 25
  97 0070 7B68     		ldr	r3, [r7, #4]
  98 0072 D3ED067A 		vldr.32	s15, [r3, #24]
  99              		.loc 1 10 23
 100 0076 27EE277A 		vmul.f32	s14, s14, s15
 101              		.loc 1 10 37
 102 007a 7B68     		ldr	r3, [r7, #4]
 103 007c 5B88     		ldrh	r3, [r3, #2]
 104 007e 07EE903A 		vmov	s15, r3	@ int
 105              		.loc 1 10 35
ARM GAS  /tmp/ccLa5IHU.s 			page 3


 106 0082 F8EEE77A 		vcvt.f32.s32	s15, s15
 107 0086 77EE277A 		vadd.f32	s15, s14, s15
 108              		.loc 1 10 5
 109 008a 7B68     		ldr	r3, [r7, #4]
 110 008c DB68     		ldr	r3, [r3, #12]
 111              		.loc 1 10 12
 112 008e FCEEE77A 		vcvt.u32.f32	s15, s15
 113 0092 17EE902A 		vmov	r2, s15	@ int
 114 0096 1A60     		str	r2, [r3]
  11:motorlib/pwm.cpp **** }
 115              		.loc 1 11 1
 116 0098 00BF     		nop
 117 009a 0C37     		adds	r7, r7, #12
 118              	.LCFI3:
 119              		.cfi_def_cfa_offset 4
 120 009c BD46     		mov	sp, r7
 121              	.LCFI4:
 122              		.cfi_def_cfa_register 13
 123              		@ sp needed
 124 009e 5DF8047B 		ldr	r7, [sp], #4
 125              	.LCFI5:
 126              		.cfi_restore 7
 127              		.cfi_def_cfa_offset 0
 128 00a2 7047     		bx	lr
 129              		.cfi_endproc
 130              	.LFE409:
 131              		.cantunwind
 132              		.fnend
 134              		.section	.text._ZN3PWM8set_vbusEf,"ax",%progbits
 135              		.align	1
 136              		.global	_ZN3PWM8set_vbusEf
 137              		.syntax unified
 138              		.thumb
 139              		.thumb_func
 140              		.fpu fpv4-sp-d16
 142              	_ZN3PWM8set_vbusEf:
 143              		.fnstart
 144              	.LFB410:
  12:motorlib/pwm.cpp **** 
  13:motorlib/pwm.cpp **** void PWM::set_vbus(float vbus) {
 145              		.loc 1 13 32
 146              		.cfi_startproc
 147              		@ args = 0, pretend = 0, frame = 8
 148              		@ frame_needed = 1, uses_anonymous_args = 0
 149              		@ link register save eliminated.
 150 0000 80B4     		push	{r7}
 151              	.LCFI6:
 152              		.cfi_def_cfa_offset 4
 153              		.cfi_offset 7, -4
 154 0002 83B0     		sub	sp, sp, #12
 155              	.LCFI7:
 156              		.cfi_def_cfa_offset 16
 157 0004 00AF     		add	r7, sp, #0
 158              	.LCFI8:
 159              		.cfi_def_cfa_register 7
 160 0006 7860     		str	r0, [r7, #4]
 161 0008 87ED000A 		vstr.32	s0, [r7]
ARM GAS  /tmp/ccLa5IHU.s 			page 4


  14:motorlib/pwm.cpp ****     v_to_pwm_ = period_/vbus;
 162              		.loc 1 14 17
 163 000c 7B68     		ldr	r3, [r7, #4]
 164 000e 1B88     		ldrh	r3, [r3]
 165 0010 07EE903A 		vmov	s15, r3	@ int
 166              		.loc 1 14 24
 167 0014 F8EEE76A 		vcvt.f32.s32	s13, s15
 168 0018 97ED007A 		vldr.32	s14, [r7]
 169 001c C6EE877A 		vdiv.f32	s15, s13, s14
 170              		.loc 1 14 15
 171 0020 7B68     		ldr	r3, [r7, #4]
 172 0022 C3ED067A 		vstr.32	s15, [r3, #24]
  15:motorlib/pwm.cpp **** }
 173              		.loc 1 15 1
 174 0026 00BF     		nop
 175 0028 0C37     		adds	r7, r7, #12
 176              	.LCFI9:
 177              		.cfi_def_cfa_offset 4
 178 002a BD46     		mov	sp, r7
 179              	.LCFI10:
 180              		.cfi_def_cfa_register 13
 181              		@ sp needed
 182 002c 5DF8047B 		ldr	r7, [sp], #4
 183              	.LCFI11:
 184              		.cfi_restore 7
 185              		.cfi_def_cfa_offset 0
 186 0030 7047     		bx	lr
 187              		.cfi_endproc
 188              	.LFE410:
 189              		.cantunwind
 190              		.fnend
 192              		.section	.text._ZN3PWM9open_modeEv,"ax",%progbits
 193              		.align	1
 194              		.global	_ZN3PWM9open_modeEv
 195              		.syntax unified
 196              		.thumb
 197              		.thumb_func
 198              		.fpu fpv4-sp-d16
 200              	_ZN3PWM9open_modeEv:
 201              		.fnstart
 202              	.LFB411:
  16:motorlib/pwm.cpp **** 
  17:motorlib/pwm.cpp **** void PWM::open_mode() {
 203              		.loc 1 17 23
 204              		.cfi_startproc
 205              		@ args = 0, pretend = 0, frame = 8
 206              		@ frame_needed = 1, uses_anonymous_args = 0
 207 0000 80B5     		push	{r7, lr}
 208              		.save {r7, lr}
 209              	.LCFI12:
 210              		.cfi_def_cfa_offset 8
 211              		.cfi_offset 7, -8
 212              		.cfi_offset 14, -4
 213              		.pad #8
 214 0002 82B0     		sub	sp, sp, #8
 215              	.LCFI13:
 216              		.cfi_def_cfa_offset 16
ARM GAS  /tmp/ccLa5IHU.s 			page 5


 217              		.setfp r7, sp, #0
 218 0004 00AF     		add	r7, sp, #0
 219              	.LCFI14:
 220              		.cfi_def_cfa_register 7
 221 0006 7860     		str	r0, [r7, #4]
  18:motorlib/pwm.cpp ****     enable_.clear();
 222              		.loc 1 18 5
 223 0008 7B68     		ldr	r3, [r7, #4]
 224 000a 5B69     		ldr	r3, [r3, #20]
 225              		.loc 1 18 18
 226 000c 1846     		mov	r0, r3
 227 000e FFF7FEFF 		bl	_ZN4GPIO5clearEv
  19:motorlib/pwm.cpp **** }
 228              		.loc 1 19 1
 229 0012 00BF     		nop
 230 0014 0837     		adds	r7, r7, #8
 231              	.LCFI15:
 232              		.cfi_def_cfa_offset 8
 233 0016 BD46     		mov	sp, r7
 234              	.LCFI16:
 235              		.cfi_def_cfa_register 13
 236              		@ sp needed
 237 0018 80BD     		pop	{r7, pc}
 238              		.cfi_endproc
 239              	.LFE411:
 240              		.fnend
 242              		.section	.text._ZN3PWM10brake_modeEv,"ax",%progbits
 243              		.align	1
 244              		.global	_ZN3PWM10brake_modeEv
 245              		.syntax unified
 246              		.thumb
 247              		.thumb_func
 248              		.fpu fpv4-sp-d16
 250              	_ZN3PWM10brake_modeEv:
 251              		.fnstart
 252              	.LFB412:
  20:motorlib/pwm.cpp **** 
  21:motorlib/pwm.cpp **** void PWM::brake_mode() {
 253              		.loc 1 21 24
 254              		.cfi_startproc
 255              		@ args = 0, pretend = 0, frame = 8
 256              		@ frame_needed = 1, uses_anonymous_args = 0
 257 0000 80B5     		push	{r7, lr}
 258              		.save {r7, lr}
 259              	.LCFI17:
 260              		.cfi_def_cfa_offset 8
 261              		.cfi_offset 7, -8
 262              		.cfi_offset 14, -4
 263              		.pad #8
 264 0002 82B0     		sub	sp, sp, #8
 265              	.LCFI18:
 266              		.cfi_def_cfa_offset 16
 267              		.setfp r7, sp, #0
 268 0004 00AF     		add	r7, sp, #0
 269              	.LCFI19:
 270              		.cfi_def_cfa_register 7
 271 0006 7860     		str	r0, [r7, #4]
ARM GAS  /tmp/ccLa5IHU.s 			page 6


  22:motorlib/pwm.cpp ****     enable_.set();
 272              		.loc 1 22 5
 273 0008 7B68     		ldr	r3, [r7, #4]
 274 000a 5B69     		ldr	r3, [r3, #20]
 275              		.loc 1 22 16
 276 000c 1846     		mov	r0, r3
 277 000e FFF7FEFF 		bl	_ZN4GPIO3setEv
  23:motorlib/pwm.cpp ****     //MOE = 0; // with OSSI=1 to force low
  24:motorlib/pwm.cpp ****     regs_.BDTR |= TIM_BDTR_OSSI;
 278              		.loc 1 24 5
 279 0012 7B68     		ldr	r3, [r7, #4]
 280 0014 1B69     		ldr	r3, [r3, #16]
 281              		.loc 1 24 16
 282 0016 5A6C     		ldr	r2, [r3, #68]
 283              		.loc 1 24 5
 284 0018 7B68     		ldr	r3, [r7, #4]
 285 001a 1B69     		ldr	r3, [r3, #16]
 286              		.loc 1 24 16
 287 001c 42F48062 		orr	r2, r2, #1024
 288 0020 5A64     		str	r2, [r3, #68]
  25:motorlib/pwm.cpp ****     regs_.BDTR &= ~TIM_BDTR_MOE;
 289              		.loc 1 25 5
 290 0022 7B68     		ldr	r3, [r7, #4]
 291 0024 1B69     		ldr	r3, [r3, #16]
 292              		.loc 1 25 16
 293 0026 5A6C     		ldr	r2, [r3, #68]
 294              		.loc 1 25 5
 295 0028 7B68     		ldr	r3, [r7, #4]
 296 002a 1B69     		ldr	r3, [r3, #16]
 297              		.loc 1 25 16
 298 002c 22F40042 		bic	r2, r2, #32768
 299 0030 5A64     		str	r2, [r3, #68]
  26:motorlib/pwm.cpp **** }
 300              		.loc 1 26 1
 301 0032 00BF     		nop
 302 0034 0837     		adds	r7, r7, #8
 303              	.LCFI20:
 304              		.cfi_def_cfa_offset 8
 305 0036 BD46     		mov	sp, r7
 306              	.LCFI21:
 307              		.cfi_def_cfa_register 13
 308              		@ sp needed
 309 0038 80BD     		pop	{r7, pc}
 310              		.cfi_endproc
 311              	.LFE412:
 312              		.fnend
 314              		.section	.text._ZN3PWM12voltage_modeEv,"ax",%progbits
 315              		.align	1
 316              		.global	_ZN3PWM12voltage_modeEv
 317              		.syntax unified
 318              		.thumb
 319              		.thumb_func
 320              		.fpu fpv4-sp-d16
 322              	_ZN3PWM12voltage_modeEv:
 323              		.fnstart
 324              	.LFB413:
  27:motorlib/pwm.cpp **** 
ARM GAS  /tmp/ccLa5IHU.s 			page 7


  28:motorlib/pwm.cpp **** void PWM::voltage_mode() {
 325              		.loc 1 28 26
 326              		.cfi_startproc
 327              		@ args = 0, pretend = 0, frame = 8
 328              		@ frame_needed = 1, uses_anonymous_args = 0
 329 0000 80B5     		push	{r7, lr}
 330              		.save {r7, lr}
 331              	.LCFI22:
 332              		.cfi_def_cfa_offset 8
 333              		.cfi_offset 7, -8
 334              		.cfi_offset 14, -4
 335              		.pad #8
 336 0002 82B0     		sub	sp, sp, #8
 337              	.LCFI23:
 338              		.cfi_def_cfa_offset 16
 339              		.setfp r7, sp, #0
 340 0004 00AF     		add	r7, sp, #0
 341              	.LCFI24:
 342              		.cfi_def_cfa_register 7
 343 0006 7860     		str	r0, [r7, #4]
  29:motorlib/pwm.cpp ****     enable_.set();
 344              		.loc 1 29 5
 345 0008 7B68     		ldr	r3, [r7, #4]
 346 000a 5B69     		ldr	r3, [r3, #20]
 347              		.loc 1 29 16
 348 000c 1846     		mov	r0, r3
 349 000e FFF7FEFF 		bl	_ZN4GPIO3setEv
  30:motorlib/pwm.cpp ****     regs_.BDTR |= TIM_BDTR_MOE;
 350              		.loc 1 30 5
 351 0012 7B68     		ldr	r3, [r7, #4]
 352 0014 1B69     		ldr	r3, [r3, #16]
 353              		.loc 1 30 16
 354 0016 5A6C     		ldr	r2, [r3, #68]
 355              		.loc 1 30 5
 356 0018 7B68     		ldr	r3, [r7, #4]
 357 001a 1B69     		ldr	r3, [r3, #16]
 358              		.loc 1 30 16
 359 001c 42F40042 		orr	r2, r2, #32768
 360 0020 5A64     		str	r2, [r3, #68]
  31:motorlib/pwm.cpp **** }
 361              		.loc 1 31 1
 362 0022 00BF     		nop
 363 0024 0837     		adds	r7, r7, #8
 364              	.LCFI25:
 365              		.cfi_def_cfa_offset 8
 366 0026 BD46     		mov	sp, r7
 367              	.LCFI26:
 368              		.cfi_def_cfa_register 13
 369              		@ sp needed
 370 0028 80BD     		pop	{r7, pc}
 371              		.cfi_endproc
 372              	.LFE413:
 373              		.fnend
 375              		.global	__aeabi_ui2d
 376              		.global	__aeabi_ddiv
 377              		.global	__aeabi_d2uiz
 378              		.section	.text._ZN3PWM16set_frequency_hzEm,"ax",%progbits
ARM GAS  /tmp/ccLa5IHU.s 			page 8


 379              		.align	1
 380              		.global	_ZN3PWM16set_frequency_hzEm
 381              		.syntax unified
 382              		.thumb
 383              		.thumb_func
 384              		.fpu fpv4-sp-d16
 386              	_ZN3PWM16set_frequency_hzEm:
 387              		.fnstart
 388              	.LFB414:
  32:motorlib/pwm.cpp **** 
  33:motorlib/pwm.cpp **** void PWM::set_frequency_hz(uint32_t frequency_hz) {
 389              		.loc 1 33 51
 390              		.cfi_startproc
 391              		@ args = 0, pretend = 0, frame = 8
 392              		@ frame_needed = 1, uses_anonymous_args = 0
 393 0000 90B5     		push	{r4, r7, lr}
 394              	.LCFI27:
 395              		.cfi_def_cfa_offset 12
 396              		.cfi_offset 4, -12
 397              		.cfi_offset 7, -8
 398              		.cfi_offset 14, -4
 399 0002 83B0     		sub	sp, sp, #12
 400              	.LCFI28:
 401              		.cfi_def_cfa_offset 24
 402 0004 00AF     		add	r7, sp, #0
 403              	.LCFI29:
 404              		.cfi_def_cfa_register 7
 405 0006 7860     		str	r0, [r7, #4]
 406 0008 3960     		str	r1, [r7]
  34:motorlib/pwm.cpp ****     regs_.ARR = 180e6/2/frequency_hz; // todo not enabled at startup
 407              		.loc 1 34 24
 408 000a 3868     		ldr	r0, [r7]
 409 000c FFF7FEFF 		bl	__aeabi_ui2d
 410              	.LVL0:
 411 0010 0346     		mov	r3, r0
 412 0012 0C46     		mov	r4, r1
 413 0014 1A46     		mov	r2, r3
 414 0016 2346     		mov	r3, r4
 415 0018 0FA1     		adr	r1, .L7
 416 001a D1E90001 		ldrd	r0, [r1]
 417 001e FFF7FEFF 		bl	__aeabi_ddiv
 418              	.LVL1:
 419 0022 0346     		mov	r3, r0
 420 0024 0C46     		mov	r4, r1
 421 0026 1946     		mov	r1, r3
 422 0028 2246     		mov	r2, r4
 423              		.loc 1 34 5
 424 002a 7B68     		ldr	r3, [r7, #4]
 425 002c 1C69     		ldr	r4, [r3, #16]
 426              		.loc 1 34 15
 427 002e 0846     		mov	r0, r1
 428 0030 1146     		mov	r1, r2
 429 0032 FFF7FEFF 		bl	__aeabi_d2uiz
 430              	.LVL2:
 431 0036 0346     		mov	r3, r0
 432 0038 E362     		str	r3, [r4, #44]
  35:motorlib/pwm.cpp ****     period_ = hrperiod;// 180e6/2/frequency_hz;
ARM GAS  /tmp/ccLa5IHU.s 			page 9


 433              		.loc 1 35 13
 434 003a 7B68     		ldr	r3, [r7, #4]
 435 003c 4FF69032 		movw	r2, #64400
 436 0040 1A80     		strh	r2, [r3]	@ movhi
  36:motorlib/pwm.cpp ****     half_period_ = period_/2; 
 437              		.loc 1 36 20
 438 0042 7B68     		ldr	r3, [r7, #4]
 439 0044 1B88     		ldrh	r3, [r3]
 440              		.loc 1 36 27
 441 0046 5B08     		lsrs	r3, r3, #1
 442 0048 9AB2     		uxth	r2, r3
 443              		.loc 1 36 18
 444 004a 7B68     		ldr	r3, [r7, #4]
 445 004c 5A80     		strh	r2, [r3, #2]	@ movhi
  37:motorlib/pwm.cpp **** }...
 446              		.loc 1 37 1
 447 004e 00BF     		nop
 448 0050 0C37     		adds	r7, r7, #12
 449              	.LCFI30:
 450              		.cfi_def_cfa_offset 12
 451 0052 BD46     		mov	sp, r7
 452              	.LCFI31:
 453              		.cfi_def_cfa_register 13
 454              		@ sp needed
 455 0054 90BD     		pop	{r4, r7, pc}
 456              	.L8:
 457 0056 00BF     		.align	3
 458              	.L7:
 459 0058 00000000 		.word	0
 460 005c 2A759541 		.word	1100313898
 461              		.cfi_endproc
 462              	.LFE414:
 463              		.cantunwind
 464              		.fnend
 466              		.text
 467              	.Letext0:
 468              		.file 2 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdint"
 469              		.file 3 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thu
 470              		.file 4 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 471              		.file 5 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 472              		.file 6 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdint.h"
 473              		.file 7 "Drivers/CMSIS/Include/core_cm4.h"
 474              		.file 8 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/system_stm32g4xx.h"
 475              		.file 9 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/stm32g474xx.h"
 476              		.file 10 "motorlib/pwm.h"
 477              		.file 11 "motorlib/gpio.h"
 478              		.file 12 "Drivers/STM32G4xx_HAL_Driver/Inc/stm32g4xx_hal_def.h"
 479              		.file 13 "Drivers/STM32G4xx_HAL_Driver/Inc/stm32g4xx_hal_flash.h"
 480              		.file 14 "Drivers/STM32G4xx_HAL_Driver/Inc/stm32g4xx_hal.h"
 481              		.file 15 "<built-in>"
ARM GAS  /tmp/ccLa5IHU.s 			page 10


DEFINED SYMBOLS
                            *ABS*:0000000000000000 pwm.cpp
     /tmp/ccLa5IHU.s:18     .ccmram:0000000000000000 $t
     /tmp/ccLa5IHU.s:26     .ccmram:0000000000000000 _ZN3PWM11set_voltageEPf
                .ARM.exidx.ccmram:0000000000000000 $d
     /tmp/ccLa5IHU.s:135    .text._ZN3PWM8set_vbusEf:0000000000000000 $t
     /tmp/ccLa5IHU.s:142    .text._ZN3PWM8set_vbusEf:0000000000000000 _ZN3PWM8set_vbusEf
.ARM.exidx.text._ZN3PWM8set_vbusEf:0000000000000000 $d
     /tmp/ccLa5IHU.s:193    .text._ZN3PWM9open_modeEv:0000000000000000 $t
     /tmp/ccLa5IHU.s:200    .text._ZN3PWM9open_modeEv:0000000000000000 _ZN3PWM9open_modeEv
.ARM.extab.text._ZN3PWM9open_modeEv:0000000000000000 $d
.ARM.exidx.text._ZN3PWM9open_modeEv:0000000000000000 $d
     /tmp/ccLa5IHU.s:243    .text._ZN3PWM10brake_modeEv:0000000000000000 $t
     /tmp/ccLa5IHU.s:250    .text._ZN3PWM10brake_modeEv:0000000000000000 _ZN3PWM10brake_modeEv
.ARM.extab.text._ZN3PWM10brake_modeEv:0000000000000000 $d
.ARM.exidx.text._ZN3PWM10brake_modeEv:0000000000000000 $d
     /tmp/ccLa5IHU.s:315    .text._ZN3PWM12voltage_modeEv:0000000000000000 $t
     /tmp/ccLa5IHU.s:322    .text._ZN3PWM12voltage_modeEv:0000000000000000 _ZN3PWM12voltage_modeEv
.ARM.extab.text._ZN3PWM12voltage_modeEv:0000000000000000 $d
.ARM.exidx.text._ZN3PWM12voltage_modeEv:0000000000000000 $d
     /tmp/ccLa5IHU.s:379    .text._ZN3PWM16set_frequency_hzEm:0000000000000000 $t
     /tmp/ccLa5IHU.s:386    .text._ZN3PWM16set_frequency_hzEm:0000000000000000 _ZN3PWM16set_frequency_hzEm
     /tmp/ccLa5IHU.s:459    .text._ZN3PWM16set_frequency_hzEm:0000000000000058 $d
.ARM.exidx.text._ZN3PWM16set_frequency_hzEm:0000000000000000 $d

UNDEFINED SYMBOLS
_ZN4GPIO5clearEv
__aeabi_unwind_cpp_pr1
_ZN4GPIO3setEv
__aeabi_ui2d
__aeabi_ddiv
__aeabi_d2uiz
