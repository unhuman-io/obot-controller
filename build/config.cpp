ARM GAS  /tmp/ccMP84Nn.s 			page 1


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
  13              		.file	"config.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.text._ZN8MainLoopC2ER13PIDControllerR13CommunicationR3LED,"axG",%progbits,_ZN8MainLoopC5
  18              		.align	1
  19              		.weak	_ZN8MainLoopC2ER13PIDControllerR13CommunicationR3LED
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN8MainLoopC2ER13PIDControllerR13CommunicationR3LED:
  27              		.fnstart
  28              	.LFB227:
  29              		.file 1 "motorlib/main_loop.h"
   1:motorlib/main_loop.h **** 
   2:motorlib/main_loop.h **** #ifndef MAIN_LOOP_H
   3:motorlib/main_loop.h **** #define MAIN_LOOP_H
   4:motorlib/main_loop.h **** 
   5:motorlib/main_loop.h **** #include "messages.h"
   6:motorlib/main_loop.h **** class LED;
   7:motorlib/main_loop.h **** class PIDController;
   8:motorlib/main_loop.h **** class Communication;
   9:motorlib/main_loop.h **** #include <cmath>
  10:motorlib/main_loop.h **** 
  11:motorlib/main_loop.h **** class MainLoop {
  12:motorlib/main_loop.h ****  public:
  13:motorlib/main_loop.h ****     MainLoop(PIDController &controller, Communication &communication, LED &led) : 
  30              		.loc 1 13 5
  31              		.cfi_startproc
  32              		@ args = 0, pretend = 0, frame = 16
  33              		@ frame_needed = 1, uses_anonymous_args = 0
  34 0000 90B5     		push	{r4, r7, lr}
  35              	.LCFI0:
  36              		.cfi_def_cfa_offset 12
  37              		.cfi_offset 4, -12
  38              		.cfi_offset 7, -8
  39              		.cfi_offset 14, -4
  40 0002 85B0     		sub	sp, sp, #20
  41              	.LCFI1:
  42              		.cfi_def_cfa_offset 32
  43 0004 00AF     		add	r7, sp, #0
  44              	.LCFI2:
  45              		.cfi_def_cfa_register 7
ARM GAS  /tmp/ccMP84Nn.s 			page 2


  46 0006 F860     		str	r0, [r7, #12]
  47 0008 B960     		str	r1, [r7, #8]
  48 000a 7A60     		str	r2, [r7, #4]
  49 000c 3B60     		str	r3, [r7]
  50              	.LBB2:
  14:motorlib/main_loop.h ****         controller_(controller), communication_(communication), led_(led) {}
  51              		.loc 1 14 73
  52 000e FB68     		ldr	r3, [r7, #12]
  53 0010 BA68     		ldr	r2, [r7, #8]
  54 0012 9A62     		str	r2, [r3, #40]
  55 0014 FB68     		ldr	r3, [r7, #12]
  56 0016 7A68     		ldr	r2, [r7, #4]
  57 0018 DA62     		str	r2, [r3, #44]
  58 001a FB68     		ldr	r3, [r7, #12]
  59 001c 3A68     		ldr	r2, [r7]
  60 001e 1A63     		str	r2, [r3, #48]
  61 0020 FB68     		ldr	r3, [r7, #12]
  62 0022 3433     		adds	r3, r3, #52
  63 0024 0022     		movs	r2, #0
  64 0026 1A60     		str	r2, [r3]
  65 0028 5A60     		str	r2, [r3, #4]
  66 002a 9A60     		str	r2, [r3, #8]
  67 002c DA60     		str	r2, [r3, #12]
  68 002e FA68     		ldr	r2, [r7, #12]
  69 0030 4FF00003 		mov	r3, #0
  70 0034 4FF00004 		mov	r4, #0
  71 0038 C2E91234 		strd	r3, [r2, #72]
  72 003c FB68     		ldr	r3, [r7, #12]
  73 003e 5033     		adds	r3, r3, #80
  74 0040 5822     		movs	r2, #88
  75 0042 0021     		movs	r1, #0
  76 0044 1846     		mov	r0, r3
  77 0046 FFF7FEFF 		bl	memset
  78 004a FB68     		ldr	r3, [r7, #12]
  79 004c 0022     		movs	r2, #0
  80 004e 83F8A820 		strb	r2, [r3, #168]
  81              	.LBE2:
  82              		.loc 1 14 76
  83 0052 FB68     		ldr	r3, [r7, #12]
  84 0054 1846     		mov	r0, r3
  85 0056 1437     		adds	r7, r7, #20
  86              	.LCFI3:
  87              		.cfi_def_cfa_offset 12
  88 0058 BD46     		mov	sp, r7
  89              	.LCFI4:
  90              		.cfi_def_cfa_register 13
  91              		@ sp needed
  92 005a 90BD     		pop	{r4, r7, pc}
  93              		.cfi_endproc
  94              	.LFE227:
  95              		.cantunwind
  96              		.fnend
  98              		.weak	_ZN8MainLoopC1ER13PIDControllerR13CommunicationR3LED
  99              		.thumb_set _ZN8MainLoopC1ER13PIDControllerR13CommunicationR3LED,_ZN8MainLoopC2ER13PIDControllerR13
 100              		.section	.text._ZN3LEDC2EPtS0_S0_b,"axG",%progbits,_ZN3LEDC5EPtS0_S0_b,comdat
 101              		.align	1
 102              		.weak	_ZN3LEDC2EPtS0_S0_b
ARM GAS  /tmp/ccMP84Nn.s 			page 3


 103              		.syntax unified
 104              		.thumb
 105              		.thumb_func
 106              		.fpu fpv4-sp-d16
 108              	_ZN3LEDC2EPtS0_S0_b:
 109              		.fnstart
 110              	.LFB231:
 111              		.file 2 "motorlib/led.h"
   1:motorlib/led.h **** 
   2:motorlib/led.h **** 
   3:motorlib/led.h **** #ifndef LED_H
   4:motorlib/led.h **** #define LED_H
   5:motorlib/led.h **** 
   6:motorlib/led.h **** #include <cstdint>
   7:motorlib/led.h **** // A tricolor led for status, pwm compare registers required
   8:motorlib/led.h **** class LED {
   9:motorlib/led.h ****  public:
  10:motorlib/led.h ****     LED(uint16_t *const red_reg, uint16_t *const green_reg, uint16_t *const blue_reg, bool invert=f
 112              		.loc 2 10 5
 113              		.cfi_startproc
 114              		@ args = 4, pretend = 0, frame = 16
 115              		@ frame_needed = 1, uses_anonymous_args = 0
 116              		@ link register save eliminated.
 117 0000 80B4     		push	{r7}
 118              	.LCFI5:
 119              		.cfi_def_cfa_offset 4
 120              		.cfi_offset 7, -4
 121 0002 85B0     		sub	sp, sp, #20
 122              	.LCFI6:
 123              		.cfi_def_cfa_offset 24
 124 0004 00AF     		add	r7, sp, #0
 125              	.LCFI7:
 126              		.cfi_def_cfa_register 7
 127 0006 F860     		str	r0, [r7, #12]
 128 0008 B960     		str	r1, [r7, #8]
 129 000a 7A60     		str	r2, [r7, #4]
 130 000c 3B60     		str	r3, [r7]
 131              	.LBB3:
  11:motorlib/led.h ****         : red_reg_(red_reg), green_reg_(green_reg), blue_reg_(blue_reg) {
 132              		.loc 2 11 71
 133 000e FB68     		ldr	r3, [r7, #12]
 134 0010 0022     		movs	r2, #0
 135 0012 1A80     		strh	r2, [r3]	@ movhi
 136 0014 FB68     		ldr	r3, [r7, #12]
 137 0016 BA68     		ldr	r2, [r7, #8]
 138 0018 9A60     		str	r2, [r3, #8]
 139 001a FB68     		ldr	r3, [r7, #12]
 140 001c 7A68     		ldr	r2, [r7, #4]
 141 001e DA60     		str	r2, [r3, #12]
 142 0020 FB68     		ldr	r3, [r7, #12]
 143 0022 3A68     		ldr	r2, [r7]
 144 0024 1A61     		str	r2, [r3, #16]
  12:motorlib/led.h ****             invert_ = invert ? 1 : 0;
 145              		.loc 2 12 21
 146 0026 3B7E     		ldrb	r3, [r7, #24]	@ zero_extendqisi2
 147 0028 002B     		cmp	r3, #0
 148 002a 02D0     		beq	.L4
ARM GAS  /tmp/ccMP84Nn.s 			page 4


 149              		.loc 2 12 21 is_stmt 0 discriminator 1
 150 002c 4FF07E52 		mov	r2, #1065353216
 151 0030 01E0     		b	.L5
 152              	.L4:
 153              		.loc 2 12 21 discriminator 2
 154 0032 4FF00002 		mov	r2, #0
 155              	.L5:
 156              		.loc 2 12 21 discriminator 4
 157 0036 FB68     		ldr	r3, [r7, #12]
 158 0038 5A60     		str	r2, [r3, #4]	@ float
 159              	.LBE3:
  13:motorlib/led.h ****         }
 160              		.loc 2 13 9 is_stmt 1 discriminator 4
 161 003a FB68     		ldr	r3, [r7, #12]
 162 003c 1846     		mov	r0, r3
 163 003e 1437     		adds	r7, r7, #20
 164              	.LCFI8:
 165              		.cfi_def_cfa_offset 4
 166 0040 BD46     		mov	sp, r7
 167              	.LCFI9:
 168              		.cfi_def_cfa_register 13
 169              		@ sp needed
 170 0042 5DF8047B 		ldr	r7, [sp], #4
 171              	.LCFI10:
 172              		.cfi_restore 7
 173              		.cfi_def_cfa_offset 0
 174 0046 7047     		bx	lr
 175              		.cfi_endproc
 176              	.LFE231:
 177              		.cantunwind
 178              		.fnend
 180              		.weak	_ZN3LEDC1EPtS0_S0_b
 181              		.thumb_set _ZN3LEDC1EPtS0_S0_b,_ZN3LEDC2EPtS0_S0_b
 182              		.section	.text._ZN13PIDControllerD2Ev,"axG",%progbits,_ZN13PIDControllerD5Ev,comdat
 183              		.align	1
 184              		.weak	_ZN13PIDControllerD2Ev
 185              		.syntax unified
 186              		.thumb
 187              		.thumb_func
 188              		.fpu fpv4-sp-d16
 190              	_ZN13PIDControllerD2Ev:
 191              		.fnstart
 192              	.LFB241:
 193              		.file 3 "motorlib/control_fun.h"
   1:motorlib/control_fun.h **** #ifndef MOTOR_CONTROL_FUN_H
   2:motorlib/control_fun.h **** #define MOTOR_CONTROL_FUN_H
   3:motorlib/control_fun.h **** 
   4:motorlib/control_fun.h **** #include "messages.h"
   5:motorlib/control_fun.h **** 
   6:motorlib/control_fun.h **** class PIController {
   7:motorlib/control_fun.h **** public:
   8:motorlib/control_fun.h ****     ~PIController() {}
   9:motorlib/control_fun.h ****     float step(float desired, float measured)  __attribute__((section (".ccmram")));;
  10:motorlib/control_fun.h ****     void set_param(const PIParam &pi_param);
  11:motorlib/control_fun.h **** private:
  12:motorlib/control_fun.h ****     float kp_ = 0, ki_ = 0, ki_sum_ = 0, ki_limit_ = 0, command_max_ = 0;
  13:motorlib/control_fun.h **** 
ARM GAS  /tmp/ccMP84Nn.s 			page 5


  14:motorlib/control_fun.h **** };
  15:motorlib/control_fun.h **** 
  16:motorlib/control_fun.h **** 
  17:motorlib/control_fun.h **** class PIDController {
  18:motorlib/control_fun.h **** public:
  19:motorlib/control_fun.h ****     virtual ~PIDController() {}
 194              		.loc 3 19 13
 195              		.cfi_startproc
 196              		@ args = 0, pretend = 0, frame = 8
 197              		@ frame_needed = 1, uses_anonymous_args = 0
 198              		@ link register save eliminated.
 199 0000 80B4     		push	{r7}
 200              	.LCFI11:
 201              		.cfi_def_cfa_offset 4
 202              		.cfi_offset 7, -4
 203 0002 83B0     		sub	sp, sp, #12
 204              	.LCFI12:
 205              		.cfi_def_cfa_offset 16
 206 0004 00AF     		add	r7, sp, #0
 207              	.LCFI13:
 208              		.cfi_def_cfa_register 7
 209 0006 7860     		str	r0, [r7, #4]
 210              	.LBB4:
 211              		.loc 3 19 30
 212 0008 044A     		ldr	r2, .L9
 213 000a 7B68     		ldr	r3, [r7, #4]
 214 000c 1A60     		str	r2, [r3]
 215              	.LBE4:
 216              		.loc 3 19 31
 217 000e 7B68     		ldr	r3, [r7, #4]
 218 0010 1846     		mov	r0, r3
 219 0012 0C37     		adds	r7, r7, #12
 220              	.LCFI14:
 221              		.cfi_def_cfa_offset 4
 222 0014 BD46     		mov	sp, r7
 223              	.LCFI15:
 224              		.cfi_def_cfa_register 13
 225              		@ sp needed
 226 0016 5DF8047B 		ldr	r7, [sp], #4
 227              	.LCFI16:
 228              		.cfi_restore 7
 229              		.cfi_def_cfa_offset 0
 230 001a 7047     		bx	lr
 231              	.L10:
 232              		.align	2
 233              	.L9:
 234 001c 08000000 		.word	_ZTV13PIDController+8
 235              		.cfi_endproc
 236              	.LFE241:
 237              		.cantunwind
 238              		.fnend
 240              		.weak	_ZN13PIDControllerD1Ev
 241              		.thumb_set _ZN13PIDControllerD1Ev,_ZN13PIDControllerD2Ev
 242              		.section	.text._ZN13PIDControllerD0Ev,"axG",%progbits,_ZN13PIDControllerD5Ev,comdat
 243              		.align	1
 244              		.weak	_ZN13PIDControllerD0Ev
 245              		.syntax unified
ARM GAS  /tmp/ccMP84Nn.s 			page 6


 246              		.thumb
 247              		.thumb_func
 248              		.fpu fpv4-sp-d16
 250              	_ZN13PIDControllerD0Ev:
 251              		.fnstart
 252              	.LFB243:
 253              		.loc 3 19 13
 254              		.cfi_startproc
 255              		@ args = 0, pretend = 0, frame = 8
 256              		@ frame_needed = 1, uses_anonymous_args = 0
 257 0000 80B5     		push	{r7, lr}
 258              	.LCFI17:
 259              		.cfi_def_cfa_offset 8
 260              		.cfi_offset 7, -8
 261              		.cfi_offset 14, -4
 262 0002 82B0     		sub	sp, sp, #8
 263              	.LCFI18:
 264              		.cfi_def_cfa_offset 16
 265 0004 00AF     		add	r7, sp, #0
 266              	.LCFI19:
 267              		.cfi_def_cfa_register 7
 268 0006 7860     		str	r0, [r7, #4]
 269              		.loc 3 19 31
 270 0008 7868     		ldr	r0, [r7, #4]
 271 000a FFF7FEFF 		bl	_ZN13PIDControllerD1Ev
 272 000e 7868     		ldr	r0, [r7, #4]
 273 0010 FFF7FEFF 		bl	_ZdlPv
 274 0014 7B68     		ldr	r3, [r7, #4]
 275 0016 1846     		mov	r0, r3
 276 0018 0837     		adds	r7, r7, #8
 277              	.LCFI20:
 278              		.cfi_def_cfa_offset 8
 279 001a BD46     		mov	sp, r7
 280              	.LCFI21:
 281              		.cfi_def_cfa_register 13
 282              		@ sp needed
 283 001c 80BD     		pop	{r7, pc}
 284              		.cfi_endproc
 285              	.LFE243:
 286              		.cantunwind
 287              		.fnend
 289              		.section	.text._ZN7EncoderC2EPVl,"axG",%progbits,_ZN7EncoderC5EPVl,comdat
 290              		.align	1
 291              		.weak	_ZN7EncoderC2EPVl
 292              		.syntax unified
 293              		.thumb
 294              		.thumb_func
 295              		.fpu fpv4-sp-d16
 297              	_ZN7EncoderC2EPVl:
 298              		.fnstart
 299              	.LFB375:
 300              		.file 4 "motorlib/encoder.h"
   1:motorlib/encoder.h **** #ifndef ENCODER_H
   2:motorlib/encoder.h **** #define ENCODER_H
   3:motorlib/encoder.h **** 
   4:motorlib/encoder.h **** #include <cstdint>
   5:motorlib/encoder.h **** #include "../st_device.h"
ARM GAS  /tmp/ccMP84Nn.s 			page 7


   6:motorlib/encoder.h **** 
   7:motorlib/encoder.h **** class Encoder {
   8:motorlib/encoder.h ****  public:
   9:motorlib/encoder.h ****     Encoder(volatile int32_t *counter_reg) { counter_reg_ = counter_reg; }
 301              		.loc 4 9 5
 302              		.cfi_startproc
 303              		@ args = 0, pretend = 0, frame = 8
 304              		@ frame_needed = 1, uses_anonymous_args = 0
 305              		@ link register save eliminated.
 306 0000 80B4     		push	{r7}
 307              	.LCFI22:
 308              		.cfi_def_cfa_offset 4
 309              		.cfi_offset 7, -4
 310 0002 83B0     		sub	sp, sp, #12
 311              	.LCFI23:
 312              		.cfi_def_cfa_offset 16
 313 0004 00AF     		add	r7, sp, #0
 314              	.LCFI24:
 315              		.cfi_def_cfa_register 7
 316 0006 7860     		str	r0, [r7, #4]
 317 0008 3960     		str	r1, [r7]
 318              	.LBB5:
 319              		.loc 4 9 44
 320 000a 064A     		ldr	r2, .L15
 321 000c 7B68     		ldr	r3, [r7, #4]
 322 000e 1A60     		str	r2, [r3]
 323              		.loc 4 9 59
 324 0010 7B68     		ldr	r3, [r7, #4]
 325 0012 3A68     		ldr	r2, [r7]
 326 0014 5A60     		str	r2, [r3, #4]
 327              	.LBE5:
 328              		.loc 4 9 74
 329 0016 7B68     		ldr	r3, [r7, #4]
 330 0018 1846     		mov	r0, r3
 331 001a 0C37     		adds	r7, r7, #12
 332              	.LCFI25:
 333              		.cfi_def_cfa_offset 4
 334 001c BD46     		mov	sp, r7
 335              	.LCFI26:
 336              		.cfi_def_cfa_register 13
 337              		@ sp needed
 338 001e 5DF8047B 		ldr	r7, [sp], #4
 339              	.LCFI27:
 340              		.cfi_restore 7
 341              		.cfi_def_cfa_offset 0
 342 0022 7047     		bx	lr
 343              	.L16:
 344              		.align	2
 345              	.L15:
 346 0024 08000000 		.word	_ZTV7Encoder+8
 347              		.cfi_endproc
 348              	.LFE375:
 349              		.cantunwind
 350              		.fnend
 352              		.weak	_ZN7EncoderC1EPVl
 353              		.thumb_set _ZN7EncoderC1EPVl,_ZN7EncoderC2EPVl
 354              		.section	.text._ZN7Encoder9get_valueEv,"axG",%progbits,_ZN7Encoder9get_valueEv,comdat
ARM GAS  /tmp/ccMP84Nn.s 			page 8


 355              		.align	1
 356              		.weak	_ZN7Encoder9get_valueEv
 357              		.syntax unified
 358              		.thumb
 359              		.thumb_func
 360              		.fpu fpv4-sp-d16
 362              	_ZN7Encoder9get_valueEv:
 363              		.fnstart
 364              	.LFB377:
  10:motorlib/encoder.h ****     virtual int32_t get_value() { return *counter_reg_; }
 365              		.loc 4 10 21
 366              		.cfi_startproc
 367              		@ args = 0, pretend = 0, frame = 8
 368              		@ frame_needed = 1, uses_anonymous_args = 0
 369              		@ link register save eliminated.
 370 0000 80B4     		push	{r7}
 371              	.LCFI28:
 372              		.cfi_def_cfa_offset 4
 373              		.cfi_offset 7, -4
 374 0002 83B0     		sub	sp, sp, #12
 375              	.LCFI29:
 376              		.cfi_def_cfa_offset 16
 377 0004 00AF     		add	r7, sp, #0
 378              	.LCFI30:
 379              		.cfi_def_cfa_register 7
 380 0006 7860     		str	r0, [r7, #4]
 381              		.loc 4 10 43
 382 0008 7B68     		ldr	r3, [r7, #4]
 383 000a 5B68     		ldr	r3, [r3, #4]
 384 000c 1B68     		ldr	r3, [r3]
 385              		.loc 4 10 57
 386 000e 1846     		mov	r0, r3
 387 0010 0C37     		adds	r7, r7, #12
 388              	.LCFI31:
 389              		.cfi_def_cfa_offset 4
 390 0012 BD46     		mov	sp, r7
 391              	.LCFI32:
 392              		.cfi_def_cfa_register 13
 393              		@ sp needed
 394 0014 5DF8047B 		ldr	r7, [sp], #4
 395              	.LCFI33:
 396              		.cfi_restore 7
 397              		.cfi_def_cfa_offset 0
 398 0018 7047     		bx	lr
 399              		.cfi_endproc
 400              	.LFE377:
 401              		.cantunwind
 402              		.fnend
 404              		.section	.text._ZN7Encoder7triggerEv,"axG",%progbits,_ZN7Encoder7triggerEv,comdat
 405              		.align	1
 406              		.weak	_ZN7Encoder7triggerEv
 407              		.syntax unified
 408              		.thumb
 409              		.thumb_func
 410              		.fpu fpv4-sp-d16
 412              	_ZN7Encoder7triggerEv:
 413              		.fnstart
ARM GAS  /tmp/ccMP84Nn.s 			page 9


 414              	.LFB378:
  11:motorlib/encoder.h ****     virtual void trigger() {}
 415              		.loc 4 11 18
 416              		.cfi_startproc
 417              		@ args = 0, pretend = 0, frame = 8
 418              		@ frame_needed = 1, uses_anonymous_args = 0
 419              		@ link register save eliminated.
 420 0000 80B4     		push	{r7}
 421              	.LCFI34:
 422              		.cfi_def_cfa_offset 4
 423              		.cfi_offset 7, -4
 424 0002 83B0     		sub	sp, sp, #12
 425              	.LCFI35:
 426              		.cfi_def_cfa_offset 16
 427 0004 00AF     		add	r7, sp, #0
 428              	.LCFI36:
 429              		.cfi_def_cfa_register 7
 430 0006 7860     		str	r0, [r7, #4]
 431              		.loc 4 11 29
 432 0008 00BF     		nop
 433 000a 0C37     		adds	r7, r7, #12
 434              	.LCFI37:
 435              		.cfi_def_cfa_offset 4
 436 000c BD46     		mov	sp, r7
 437              	.LCFI38:
 438              		.cfi_def_cfa_register 13
 439              		@ sp needed
 440 000e 5DF8047B 		ldr	r7, [sp], #4
 441              	.LCFI39:
 442              		.cfi_restore 7
 443              		.cfi_def_cfa_offset 0
 444 0012 7047     		bx	lr
 445              		.cfi_endproc
 446              	.LFE378:
 447              		.cantunwind
 448              		.fnend
 450              		.section	.text._ZN3PWMC2EmRmS0_S0_R11TIM_TypeDefR4GPIO,"axG",%progbits,_ZN3PWMC5EmRmS0_S0_R11TIM_T
 451              		.align	1
 452              		.weak	_ZN3PWMC2EmRmS0_S0_R11TIM_TypeDefR4GPIO
 453              		.syntax unified
 454              		.thumb
 455              		.thumb_func
 456              		.fpu fpv4-sp-d16
 458              	_ZN3PWMC2EmRmS0_S0_R11TIM_TypeDefR4GPIO:
 459              		.fnstart
 460              	.LFB380:
 461              		.file 5 "motorlib/pwm.h"
   1:motorlib/pwm.h **** #ifndef PWM_H
   2:motorlib/pwm.h **** #define PWM_H
   3:motorlib/pwm.h **** 
   4:motorlib/pwm.h **** #include <cstdint>
   5:motorlib/pwm.h **** 
   6:motorlib/pwm.h **** #include "../st_device.h"
   7:motorlib/pwm.h **** 
   8:motorlib/pwm.h **** class GPIO;
   9:motorlib/pwm.h **** 
  10:motorlib/pwm.h **** class PWM {
ARM GAS  /tmp/ccMP84Nn.s 			page 10


  11:motorlib/pwm.h ****  public:
  12:motorlib/pwm.h ****     PWM(uint32_t frequency_hz, uint32_t &pwm_a, uint32_t &pwm_b, 
 462              		.loc 5 12 5
 463              		.cfi_startproc
 464              		@ args = 12, pretend = 0, frame = 16
 465              		@ frame_needed = 1, uses_anonymous_args = 0
 466 0000 80B5     		push	{r7, lr}
 467              		.save {r7, lr}
 468              	.LCFI40:
 469              		.cfi_def_cfa_offset 8
 470              		.cfi_offset 7, -8
 471              		.cfi_offset 14, -4
 472              		.pad #16
 473 0002 84B0     		sub	sp, sp, #16
 474              	.LCFI41:
 475              		.cfi_def_cfa_offset 24
 476              		.setfp r7, sp, #0
 477 0004 00AF     		add	r7, sp, #0
 478              	.LCFI42:
 479              		.cfi_def_cfa_register 7
 480 0006 F860     		str	r0, [r7, #12]
 481 0008 B960     		str	r1, [r7, #8]
 482 000a 7A60     		str	r2, [r7, #4]
 483 000c 3B60     		str	r3, [r7]
 484              	.LBB6:
  13:motorlib/pwm.h ****          uint32_t &pwm_c, TIM_TypeDef &regs, GPIO &enable) : 
  14:motorlib/pwm.h ****          pwm_a_(pwm_a), pwm_b_(pwm_b), pwm_c_(pwm_c),
  15:motorlib/pwm.h ****          regs_(regs), enable_(enable) {
 485              		.loc 5 15 37
 486 000e FB68     		ldr	r3, [r7, #12]
 487 0010 7A68     		ldr	r2, [r7, #4]
 488 0012 5A60     		str	r2, [r3, #4]
 489 0014 FB68     		ldr	r3, [r7, #12]
 490 0016 3A68     		ldr	r2, [r7]
 491 0018 9A60     		str	r2, [r3, #8]
 492 001a FB68     		ldr	r3, [r7, #12]
 493 001c BA69     		ldr	r2, [r7, #24]
 494 001e DA60     		str	r2, [r3, #12]
 495 0020 FB68     		ldr	r3, [r7, #12]
 496 0022 FA69     		ldr	r2, [r7, #28]
 497 0024 1A61     		str	r2, [r3, #16]
 498 0026 FB68     		ldr	r3, [r7, #12]
 499 0028 3A6A     		ldr	r2, [r7, #32]
 500 002a 5A61     		str	r2, [r3, #20]
  16:motorlib/pwm.h ****       set_frequency_hz(frequency_hz);
 501              		.loc 5 16 23
 502 002c B968     		ldr	r1, [r7, #8]
 503 002e F868     		ldr	r0, [r7, #12]
 504 0030 FFF7FEFF 		bl	_ZN3PWM16set_frequency_hzEm
  17:motorlib/pwm.h ****       set_vbus(12);
 505              		.loc 5 17 15
 506 0034 B2EE080A 		vmov.f32	s0, #1.2e+1
 507 0038 F868     		ldr	r0, [r7, #12]
 508 003a FFF7FEFF 		bl	_ZN3PWM8set_vbusEf
 509              	.LBE6:
  18:motorlib/pwm.h ****    } 
 510              		.loc 5 18 4
ARM GAS  /tmp/ccMP84Nn.s 			page 11


 511 003e FB68     		ldr	r3, [r7, #12]
 512 0040 1846     		mov	r0, r3
 513 0042 1037     		adds	r7, r7, #16
 514              	.LCFI43:
 515              		.cfi_def_cfa_offset 8
 516 0044 BD46     		mov	sp, r7
 517              	.LCFI44:
 518              		.cfi_def_cfa_register 13
 519              		@ sp needed
 520 0046 80BD     		pop	{r7, pc}
 521              		.cfi_endproc
 522              	.LFE380:
 523              		.fnend
 525              		.weak	_ZN3PWMC1EmRmS0_S0_R11TIM_TypeDefR4GPIO
 526              		.thumb_set _ZN3PWMC1EmRmS0_S0_R11TIM_TypeDefR4GPIO,_ZN3PWMC2EmRmS0_S0_R11TIM_TypeDefR4GPIO
 527              		.section	.text._ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO,"axG",%progbits,_ZN10SPIEncoderC5ER11SPI_Typ
 528              		.align	1
 529              		.weak	_ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO
 530              		.syntax unified
 531              		.thumb
 532              		.thumb_func
 533              		.fpu fpv4-sp-d16
 535              	_ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO:
 536              		.fnstart
 537              	.LFB383:
 538              		.file 6 "motorlib/spi_encoder.h"
   1:motorlib/spi_encoder.h **** #ifndef SPI_ENCODER
   2:motorlib/spi_encoder.h **** #define SPI_ENCODER
   3:motorlib/spi_encoder.h **** 
   4:motorlib/spi_encoder.h **** #include "encoder.h"
   5:motorlib/spi_encoder.h **** 
   6:motorlib/spi_encoder.h **** #ifndef SPI1
   7:motorlib/spi_encoder.h **** struct SPI_TypeDef;
   8:motorlib/spi_encoder.h **** #endif
   9:motorlib/spi_encoder.h **** 
  10:motorlib/spi_encoder.h **** class GPIO;
  11:motorlib/spi_encoder.h **** 
  12:motorlib/spi_encoder.h **** class SPIEncoder : public Encoder {
  13:motorlib/spi_encoder.h ****  public:
  14:motorlib/spi_encoder.h ****     SPIEncoder(SPI_TypeDef &regs, GPIO &gpio_cs) : Encoder(0), regs_(regs), gpio_cs_(gpio_cs) {} 
 539              		.loc 6 14 5
 540              		.cfi_startproc
 541              		@ args = 0, pretend = 0, frame = 16
 542              		@ frame_needed = 1, uses_anonymous_args = 0
 543 0000 80B5     		push	{r7, lr}
 544              	.LCFI45:
 545              		.cfi_def_cfa_offset 8
 546              		.cfi_offset 7, -8
 547              		.cfi_offset 14, -4
 548 0002 84B0     		sub	sp, sp, #16
 549              	.LCFI46:
 550              		.cfi_def_cfa_offset 24
 551 0004 00AF     		add	r7, sp, #0
 552              	.LCFI47:
 553              		.cfi_def_cfa_register 7
 554 0006 F860     		str	r0, [r7, #12]
 555 0008 B960     		str	r1, [r7, #8]
ARM GAS  /tmp/ccMP84Nn.s 			page 12


 556 000a 7A60     		str	r2, [r7, #4]
 557              	.LBB7:
 558              		.loc 6 14 93
 559 000c FB68     		ldr	r3, [r7, #12]
 560 000e 0021     		movs	r1, #0
 561 0010 1846     		mov	r0, r3
 562 0012 FFF7FEFF 		bl	_ZN7EncoderC2EPVl
 563 0016 074A     		ldr	r2, .L24
 564 0018 FB68     		ldr	r3, [r7, #12]
 565 001a 1A60     		str	r2, [r3]
 566 001c FB68     		ldr	r3, [r7, #12]
 567 001e BA68     		ldr	r2, [r7, #8]
 568 0020 9A60     		str	r2, [r3, #8]
 569 0022 FB68     		ldr	r3, [r7, #12]
 570 0024 7A68     		ldr	r2, [r7, #4]
 571 0026 DA60     		str	r2, [r3, #12]
 572              	.LBE7:
 573              		.loc 6 14 96
 574 0028 FB68     		ldr	r3, [r7, #12]
 575 002a 1846     		mov	r0, r3
 576 002c 1037     		adds	r7, r7, #16
 577              	.LCFI48:
 578              		.cfi_def_cfa_offset 8
 579 002e BD46     		mov	sp, r7
 580              	.LCFI49:
 581              		.cfi_def_cfa_register 13
 582              		@ sp needed
 583 0030 80BD     		pop	{r7, pc}
 584              	.L25:
 585 0032 00BF     		.align	2
 586              	.L24:
 587 0034 08000000 		.word	_ZTV10SPIEncoder+8
 588              		.cfi_endproc
 589              	.LFE383:
 590              		.cantunwind
 591              		.fnend
 593              		.weak	_ZN10SPIEncoderC1ER11SPI_TypeDefR4GPIO
 594              		.thumb_set _ZN10SPIEncoderC1ER11SPI_TypeDefR4GPIO,_ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO
 595              		.section	.text._ZN13Aksim2EncoderC2ER11SPI_TypeDefR4GPIO,"axG",%progbits,_ZN13Aksim2EncoderC5ER11S
 596              		.align	1
 597              		.weak	_ZN13Aksim2EncoderC2ER11SPI_TypeDefR4GPIO
 598              		.syntax unified
 599              		.thumb
 600              		.thumb_func
 601              		.fpu fpv4-sp-d16
 603              	_ZN13Aksim2EncoderC2ER11SPI_TypeDefR4GPIO:
 604              		.fnstart
 605              	.LFB386:
 606              		.file 7 "motorlib/aksim2_encoder.h"
   1:motorlib/aksim2_encoder.h **** #include "spi_encoder.h"
   2:motorlib/aksim2_encoder.h **** 
   3:motorlib/aksim2_encoder.h **** // max clock currently working is 180 MHz/128 = 1.4 MHz
   4:motorlib/aksim2_encoder.h **** class Aksim2Encoder : public SPIEncoder {
   5:motorlib/aksim2_encoder.h ****  public:
   6:motorlib/aksim2_encoder.h ****     Aksim2Encoder(SPI_TypeDef &regs, GPIO &gpio_cs) : SPIEncoder(regs, gpio_cs) {} 
 607              		.loc 7 6 5
 608              		.cfi_startproc
ARM GAS  /tmp/ccMP84Nn.s 			page 13


 609              		@ args = 0, pretend = 0, frame = 16
 610              		@ frame_needed = 1, uses_anonymous_args = 0
 611 0000 80B5     		push	{r7, lr}
 612              	.LCFI50:
 613              		.cfi_def_cfa_offset 8
 614              		.cfi_offset 7, -8
 615              		.cfi_offset 14, -4
 616 0002 84B0     		sub	sp, sp, #16
 617              	.LCFI51:
 618              		.cfi_def_cfa_offset 24
 619 0004 00AF     		add	r7, sp, #0
 620              	.LCFI52:
 621              		.cfi_def_cfa_register 7
 622 0006 F860     		str	r0, [r7, #12]
 623 0008 B960     		str	r1, [r7, #8]
 624 000a 7A60     		str	r2, [r7, #4]
 625              	.LBB8:
 626              		.loc 7 6 79
 627 000c FB68     		ldr	r3, [r7, #12]
 628 000e 7A68     		ldr	r2, [r7, #4]
 629 0010 B968     		ldr	r1, [r7, #8]
 630 0012 1846     		mov	r0, r3
 631 0014 FFF7FEFF 		bl	_ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO
 632 0018 104A     		ldr	r2, .L28
 633 001a FB68     		ldr	r3, [r7, #12]
 634 001c 1A60     		str	r2, [r3]
 635 001e FB68     		ldr	r3, [r7, #12]
 636 0020 0022     		movs	r2, #0
 637 0022 1A61     		str	r2, [r3, #16]
 638 0024 FB68     		ldr	r3, [r7, #12]
 639 0026 0022     		movs	r2, #0
 640 0028 5A61     		str	r2, [r3, #20]
 641 002a FB68     		ldr	r3, [r7, #12]
 642 002c 0022     		movs	r2, #0
 643 002e 9A61     		str	r2, [r3, #24]
 644 0030 FB68     		ldr	r3, [r7, #12]
 645 0032 0022     		movs	r2, #0
 646 0034 DA61     		str	r2, [r3, #28]
 647 0036 FB68     		ldr	r3, [r7, #12]
 648 0038 2033     		adds	r3, r3, #32
 649 003a 0022     		movs	r2, #0
 650 003c 1A60     		str	r2, [r3]
 651 003e 5A60     		str	r2, [r3, #4]
 652 0040 FB68     		ldr	r3, [r7, #12]
 653 0042 2833     		adds	r3, r3, #40
 654 0044 0022     		movs	r2, #0
 655 0046 1A60     		str	r2, [r3]
 656 0048 5A60     		str	r2, [r3, #4]
 657 004a FB68     		ldr	r3, [r7, #12]
 658 004c 0022     		movs	r2, #0
 659 004e 83F83020 		strb	r2, [r3, #48]
 660              	.LBE8:
 661              		.loc 7 6 82
 662 0052 FB68     		ldr	r3, [r7, #12]
 663 0054 1846     		mov	r0, r3
 664 0056 1037     		adds	r7, r7, #16
 665              	.LCFI53:
ARM GAS  /tmp/ccMP84Nn.s 			page 14


 666              		.cfi_def_cfa_offset 8
 667 0058 BD46     		mov	sp, r7
 668              	.LCFI54:
 669              		.cfi_def_cfa_register 13
 670              		@ sp needed
 671 005a 80BD     		pop	{r7, pc}
 672              	.L29:
 673              		.align	2
 674              	.L28:
 675 005c 08000000 		.word	_ZTV13Aksim2Encoder+8
 676              		.cfi_endproc
 677              	.LFE386:
 678              		.cantunwind
 679              		.fnend
 681              		.weak	_ZN13Aksim2EncoderC1ER11SPI_TypeDefR4GPIO
 682              		.thumb_set _ZN13Aksim2EncoderC1ER11SPI_TypeDefR4GPIO,_ZN13Aksim2EncoderC2ER11SPI_TypeDefR4GPIO
 683              		.section	.text._ZN13PIDControllerC2Ev,"axG",%progbits,_ZN13PIDControllerC5Ev,comdat
 684              		.align	1
 685              		.weak	_ZN13PIDControllerC2Ev
 686              		.syntax unified
 687              		.thumb
 688              		.thumb_func
 689              		.fpu fpv4-sp-d16
 691              	_ZN13PIDControllerC2Ev:
 692              		.fnstart
 693              	.LFB390:
  17:motorlib/control_fun.h **** public:
 694              		.loc 3 17 7
 695              		.cfi_startproc
 696              		@ args = 0, pretend = 0, frame = 8
 697              		@ frame_needed = 1, uses_anonymous_args = 0
 698              		@ link register save eliminated.
 699 0000 80B4     		push	{r7}
 700              	.LCFI55:
 701              		.cfi_def_cfa_offset 4
 702              		.cfi_offset 7, -4
 703 0002 83B0     		sub	sp, sp, #12
 704              	.LCFI56:
 705              		.cfi_def_cfa_offset 16
 706 0004 00AF     		add	r7, sp, #0
 707              	.LCFI57:
 708              		.cfi_def_cfa_register 7
 709 0006 7860     		str	r0, [r7, #4]
 710              	.LBB9:
  17:motorlib/control_fun.h **** public:
 711              		.loc 3 17 7
 712 0008 124A     		ldr	r2, .L32
 713 000a 7B68     		ldr	r3, [r7, #4]
 714 000c 1A60     		str	r2, [r3]
 715 000e 7B68     		ldr	r3, [r7, #4]
 716 0010 4FF00002 		mov	r2, #0
 717 0014 5A60     		str	r2, [r3, #4]	@ float
 718 0016 7B68     		ldr	r3, [r7, #4]
 719 0018 4FF00002 		mov	r2, #0
 720 001c 9A60     		str	r2, [r3, #8]	@ float
 721 001e 7B68     		ldr	r3, [r7, #4]
 722 0020 4FF00002 		mov	r2, #0
ARM GAS  /tmp/ccMP84Nn.s 			page 15


 723 0024 DA60     		str	r2, [r3, #12]	@ float
 724 0026 7B68     		ldr	r3, [r7, #4]
 725 0028 4FF00002 		mov	r2, #0
 726 002c 1A61     		str	r2, [r3, #16]	@ float
 727 002e 7B68     		ldr	r3, [r7, #4]
 728 0030 4FF00002 		mov	r2, #0
 729 0034 5A61     		str	r2, [r3, #20]	@ float
 730 0036 7B68     		ldr	r3, [r7, #4]
 731 0038 4FF00002 		mov	r2, #0
 732 003c 9A61     		str	r2, [r3, #24]	@ float
 733 003e 7B68     		ldr	r3, [r7, #4]
 734 0040 4FF00002 		mov	r2, #0
 735 0044 DA61     		str	r2, [r3, #28]	@ float
 736              	.LBE9:
 737 0046 7B68     		ldr	r3, [r7, #4]
 738 0048 1846     		mov	r0, r3
 739 004a 0C37     		adds	r7, r7, #12
 740              	.LCFI58:
 741              		.cfi_def_cfa_offset 4
 742 004c BD46     		mov	sp, r7
 743              	.LCFI59:
 744              		.cfi_def_cfa_register 13
 745              		@ sp needed
 746 004e 5DF8047B 		ldr	r7, [sp], #4
 747              	.LCFI60:
 748              		.cfi_restore 7
 749              		.cfi_def_cfa_offset 0
 750 0052 7047     		bx	lr
 751              	.L33:
 752              		.align	2
 753              	.L32:
 754 0054 08000000 		.word	_ZTV13PIDController+8
 755              		.cfi_endproc
 756              	.LFE390:
 757              		.cantunwind
 758              		.fnend
 760              		.weak	_ZN13PIDControllerC1Ev
 761              		.thumb_set _ZN13PIDControllerC1Ev,_ZN13PIDControllerC2Ev
 762              		.section	.text._ZN13CommunicationC2Ev,"axG",%progbits,_ZN13CommunicationC5Ev,comdat
 763              		.align	1
 764              		.weak	_ZN13CommunicationC2Ev
 765              		.syntax unified
 766              		.thumb
 767              		.thumb_func
 768              		.fpu fpv4-sp-d16
 770              	_ZN13CommunicationC2Ev:
 771              		.fnstart
 772              	.LFB394:
 773              		.file 8 "motorlib/communication.h"
   1:motorlib/communication.h **** #ifndef COMMUNICATION_H
   2:motorlib/communication.h **** #define COMMUNICATION_H
   3:motorlib/communication.h **** 
   4:motorlib/communication.h **** #include "messages.h"
   5:motorlib/communication.h **** 
   6:motorlib/communication.h **** class Communication {
 774              		.loc 8 6 7
 775              		.cfi_startproc
ARM GAS  /tmp/ccMP84Nn.s 			page 16


 776              		@ args = 0, pretend = 0, frame = 8
 777              		@ frame_needed = 1, uses_anonymous_args = 0
 778              		@ link register save eliminated.
 779 0000 80B4     		push	{r7}
 780              	.LCFI61:
 781              		.cfi_def_cfa_offset 4
 782              		.cfi_offset 7, -4
 783 0002 83B0     		sub	sp, sp, #12
 784              	.LCFI62:
 785              		.cfi_def_cfa_offset 16
 786 0004 00AF     		add	r7, sp, #0
 787              	.LCFI63:
 788              		.cfi_def_cfa_register 7
 789 0006 7860     		str	r0, [r7, #4]
 790              	.LBB10:
 791              		.loc 8 6 7
 792 0008 044A     		ldr	r2, .L36
 793 000a 7B68     		ldr	r3, [r7, #4]
 794 000c 1A60     		str	r2, [r3]
 795              	.LBE10:
 796 000e 7B68     		ldr	r3, [r7, #4]
 797 0010 1846     		mov	r0, r3
 798 0012 0C37     		adds	r7, r7, #12
 799              	.LCFI64:
 800              		.cfi_def_cfa_offset 4
 801 0014 BD46     		mov	sp, r7
 802              	.LCFI65:
 803              		.cfi_def_cfa_register 13
 804              		@ sp needed
 805 0016 5DF8047B 		ldr	r7, [sp], #4
 806              	.LCFI66:
 807              		.cfi_restore 7
 808              		.cfi_def_cfa_offset 0
 809 001a 7047     		bx	lr
 810              	.L37:
 811              		.align	2
 812              	.L36:
 813 001c 08000000 		.word	_ZTV13Communication+8
 814              		.cfi_endproc
 815              	.LFE394:
 816              		.cantunwind
 817              		.fnend
 819              		.weak	_ZN13CommunicationC1Ev
 820              		.thumb_set _ZN13CommunicationC1Ev,_ZN13CommunicationC2Ev
 821              		.section	.text._ZN16USBCommunicationC2Ev,"axG",%progbits,_ZN16USBCommunicationC5Ev,comdat
 822              		.align	1
 823              		.weak	_ZN16USBCommunicationC2Ev
 824              		.syntax unified
 825              		.thumb
 826              		.thumb_func
 827              		.fpu fpv4-sp-d16
 829              	_ZN16USBCommunicationC2Ev:
 830              		.fnstart
 831              	.LFB396:
 832              		.file 9 "motorlib/usb_communication.h"
   1:motorlib/usb_communication.h **** #ifndef USB_COMMUNICATION_H
   2:motorlib/usb_communication.h **** #define USB_COMMUNICATION_H
ARM GAS  /tmp/ccMP84Nn.s 			page 17


   3:motorlib/usb_communication.h **** 
   4:motorlib/usb_communication.h **** #include "communication.h"
   5:motorlib/usb_communication.h **** class USB1;
   6:motorlib/usb_communication.h **** 
   7:motorlib/usb_communication.h **** class USBCommunication : public Communication {
 833              		.loc 9 7 7
 834              		.cfi_startproc
 835              		@ args = 0, pretend = 0, frame = 8
 836              		@ frame_needed = 1, uses_anonymous_args = 0
 837 0000 80B5     		push	{r7, lr}
 838              	.LCFI67:
 839              		.cfi_def_cfa_offset 8
 840              		.cfi_offset 7, -8
 841              		.cfi_offset 14, -4
 842 0002 82B0     		sub	sp, sp, #8
 843              	.LCFI68:
 844              		.cfi_def_cfa_offset 16
 845 0004 00AF     		add	r7, sp, #0
 846              	.LCFI69:
 847              		.cfi_def_cfa_register 7
 848 0006 7860     		str	r0, [r7, #4]
 849              	.LBB11:
 850              		.loc 9 7 7
 851 0008 7B68     		ldr	r3, [r7, #4]
 852 000a 1846     		mov	r0, r3
 853 000c FFF7FEFF 		bl	_ZN13CommunicationC2Ev
 854 0010 034A     		ldr	r2, .L40
 855 0012 7B68     		ldr	r3, [r7, #4]
 856 0014 1A60     		str	r2, [r3]
 857              	.LBE11:
 858 0016 7B68     		ldr	r3, [r7, #4]
 859 0018 1846     		mov	r0, r3
 860 001a 0837     		adds	r7, r7, #8
 861              	.LCFI70:
 862              		.cfi_def_cfa_offset 8
 863 001c BD46     		mov	sp, r7
 864              	.LCFI71:
 865              		.cfi_def_cfa_register 13
 866              		@ sp needed
 867 001e 80BD     		pop	{r7, pc}
 868              	.L41:
 869              		.align	2
 870              	.L40:
 871 0020 08000000 		.word	_ZTV16USBCommunication+8
 872              		.cfi_endproc
 873              	.LFE396:
 874              		.cantunwind
 875              		.fnend
 877              		.weak	_ZN16USBCommunicationC1Ev
 878              		.thumb_set _ZN16USBCommunicationC1Ev,_ZN16USBCommunicationC2Ev
 879              		.section	.text._ZN5._149C2Ev,"ax",%progbits
 880              		.align	1
 881              		.syntax unified
 882              		.thumb
 883              		.thumb_func
 884              		.fpu fpv4-sp-d16
 886              	_ZN5._149C2Ev:
ARM GAS  /tmp/ccMP84Nn.s 			page 18


 887              		.fnstart
 888              	.LFB398:
 889              		.file 10 "motorlib/dev_00_aksim2.cpp"
   1:motorlib/dev_00_aksim2.cpp **** #include "aksim2_encoder.h"
   2:motorlib/dev_00_aksim2.cpp **** #include "usb_communication.h"
   3:motorlib/dev_00_aksim2.cpp **** #include "../st_device.h"
   4:motorlib/dev_00_aksim2.cpp **** 
   5:motorlib/dev_00_aksim2.cpp **** extern const volatile Param initial_param;
   6:motorlib/dev_00_aksim2.cpp **** 
   7:motorlib/dev_00_aksim2.cpp **** static struct {
 890              		.loc 10 7 15
 891              		.cfi_startproc
 892              		@ args = 0, pretend = 0, frame = 8
 893              		@ frame_needed = 1, uses_anonymous_args = 0
 894 0000 80B5     		push	{r7, lr}
 895              	.LCFI72:
 896              		.cfi_def_cfa_offset 8
 897              		.cfi_offset 7, -8
 898              		.cfi_offset 14, -4
 899 0002 86B0     		sub	sp, sp, #24
 900              	.LCFI73:
 901              		.cfi_def_cfa_offset 32
 902 0004 04AF     		add	r7, sp, #16
 903              	.LCFI74:
 904              		.cfi_def_cfa 7, 16
 905 0006 7860     		str	r0, [r7, #4]
 906              	.LBB12:
 907              		.loc 10 7 15
 908 0008 7868     		ldr	r0, [r7, #4]
 909 000a 0123     		movs	r3, #1
 910 000c 0F22     		movs	r2, #15
 911 000e 4FF09041 		mov	r1, #1207959552
 912 0012 FFF7FEFF 		bl	_ZN4GPIOC1ER12GPIO_TypeDefhNS_9DirectionE
 913 0016 7B68     		ldr	r3, [r7, #4]
 914 0018 0833     		adds	r3, r3, #8
 915 001a 7A68     		ldr	r2, [r7, #4]
 916 001c 2B49     		ldr	r1, .L44
 917 001e 1846     		mov	r0, r3
 918 0020 FFF7FEFF 		bl	_ZN13Aksim2EncoderC1ER11SPI_TypeDefR4GPIO
 919 0024 7B68     		ldr	r3, [r7, #4]
 920 0026 03F13C00 		add	r0, r3, #60
 921 002a 0123     		movs	r3, #1
 922 002c 0E22     		movs	r2, #14
 923 002e 2849     		ldr	r1, .L44+4
 924 0030 FFF7FEFF 		bl	_ZN4GPIOC1ER12GPIO_TypeDefhNS_9DirectionE
 925 0034 7B68     		ldr	r3, [r7, #4]
 926 0036 03F14400 		add	r0, r3, #68
 927 003a 264B     		ldr	r3, .L44+8
 928 003c 1B68     		ldr	r3, [r3]
 929 003e 1946     		mov	r1, r3
 930 0040 7B68     		ldr	r3, [r7, #4]
 931 0042 3C33     		adds	r3, r3, #60
 932 0044 0293     		str	r3, [sp, #8]
 933 0046 244B     		ldr	r3, .L44+12
 934 0048 0193     		str	r3, [sp, #4]
 935 004a 244B     		ldr	r3, .L44+16
 936 004c 0093     		str	r3, [sp]
ARM GAS  /tmp/ccMP84Nn.s 			page 19


 937 004e 244B     		ldr	r3, .L44+20
 938 0050 244A     		ldr	r2, .L44+24
 939 0052 FFF7FEFF 		bl	_ZN3PWMC1EmRmS0_S0_R11TIM_TypeDefR4GPIO
 940 0056 7B68     		ldr	r3, [r7, #4]
 941 0058 03F16000 		add	r0, r3, #96
 942 005c 7B68     		ldr	r3, [r7, #4]
 943 005e 03F14401 		add	r1, r3, #68
 944 0062 7B68     		ldr	r3, [r7, #4]
 945 0064 0833     		adds	r3, r3, #8
 946 0066 1A46     		mov	r2, r3
 947 0068 FFF7FEFF 		bl	_ZN8FastLoopC1ER3PWMR7Encoder
 948 006c 7B68     		ldr	r3, [r7, #4]
 949 006e 03F58A50 		add	r0, r3, #4416
 950 0072 0830     		adds	r0, r0, #8
 951 0074 0123     		movs	r3, #1
 952 0076 0093     		str	r3, [sp]
 953 0078 1B4B     		ldr	r3, .L44+28
 954 007a 1C4A     		ldr	r2, .L44+32
 955 007c 1C49     		ldr	r1, .L44+36
 956 007e FFF7FEFF 		bl	_ZN3LEDC1EPtS0_S0_b
 957 0082 7B68     		ldr	r3, [r7, #4]
 958 0084 03F58A53 		add	r3, r3, #4416
 959 0088 1C33     		adds	r3, r3, #28
 960 008a 1846     		mov	r0, r3
 961 008c FFF7FEFF 		bl	_ZN13PIDControllerC1Ev
 962 0090 7B68     		ldr	r3, [r7, #4]
 963 0092 03F58B53 		add	r3, r3, #4448
 964 0096 1C33     		adds	r3, r3, #28
 965 0098 1846     		mov	r0, r3
 966 009a FFF7FEFF 		bl	_ZN16USBCommunicationC1Ev
 967 009e 7B68     		ldr	r3, [r7, #4]
 968 00a0 03F58C50 		add	r0, r3, #4480
 969 00a4 0830     		adds	r0, r0, #8
 970 00a6 7B68     		ldr	r3, [r7, #4]
 971 00a8 03F58A51 		add	r1, r3, #4416
 972 00ac 1C31     		adds	r1, r1, #28
 973 00ae 7B68     		ldr	r3, [r7, #4]
 974 00b0 03F58B52 		add	r2, r3, #4448
 975 00b4 1C32     		adds	r2, r2, #28
 976 00b6 7B68     		ldr	r3, [r7, #4]
 977 00b8 03F58A53 		add	r3, r3, #4416
 978 00bc 0833     		adds	r3, r3, #8
 979 00be FFF7FEFF 		bl	_ZN8MainLoopC1ER13PIDControllerR13CommunicationR3LED
 980              	.LBE12:
 981 00c2 7B68     		ldr	r3, [r7, #4]
 982 00c4 1846     		mov	r0, r3
 983 00c6 0837     		adds	r7, r7, #8
 984              	.LCFI75:
 985              		.cfi_def_cfa_offset 8
 986 00c8 BD46     		mov	sp, r7
 987              	.LCFI76:
 988              		.cfi_def_cfa_register 13
 989              		@ sp needed
 990 00ca 80BD     		pop	{r7, pc}
 991              	.L45:
 992              		.align	2
 993              	.L44:
ARM GAS  /tmp/ccMP84Nn.s 			page 20


 994 00cc 003C0040 		.word	1073757184
 995 00d0 00080048 		.word	1207961600
 996 00d4 00000000 		.word	initial_param
 997 00d8 00340140 		.word	1073820672
 998 00dc 9C680140 		.word	1073834140
 999 00e0 9C6A0140 		.word	1073834652
 1000 00e4 1C6B0140 		.word	1073834780
 1001 00e8 40040040 		.word	1073742912
 1002 00ec 38040040 		.word	1073742904
 1003 00f0 34040040 		.word	1073742900
 1004              		.cfi_endproc
 1005              	.LFE398:
 1006              		.cantunwind
 1007              		.fnend
 1009              		.thumb_set _ZN5._149C1Ev,_ZN5._149C2Ev
 1010              		.section	.bss._ZL12config_items,"aw",%nobits
 1011              		.align	3
 1014              	_ZL12config_items:
 1015 0000 00000000 		.space	4664
 1015      00000000 
 1015      00000000 
 1015      00000000 
 1015      00000000 
 1016              		.global	config
 1017              		.section	.bss.config,"aw",%nobits
 1018              		.align	2
 1021              	config:
 1022 0000 00000000 		.space	8
 1022      00000000 
 1023              		.section	.text._ZN6ConfigC2Ev,"ax",%progbits
 1024              		.align	1
 1025              		.global	_ZN6ConfigC2Ev
 1026              		.syntax unified
 1027              		.thumb
 1028              		.thumb_func
 1029              		.fpu fpv4-sp-d16
 1031              	_ZN6ConfigC2Ev:
 1032              		.fnstart
 1033              	.LFB401:
 1034              		.file 11 "motorlib/config.cpp"
   1:motorlib/config.cpp **** #include "config.h"
   2:motorlib/config.cpp **** #include "led.h"
   3:motorlib/config.cpp **** #include "communication.h"
   4:motorlib/config.cpp **** #include "control_fun.h"
   5:motorlib/config.cpp **** #include "encoder.h"
   6:motorlib/config.cpp **** #include "pwm.h"
   7:motorlib/config.cpp **** #include "gpio.h"
   8:motorlib/config.cpp **** 
   9:motorlib/config.cpp **** #include "dev_00_aksim2.cpp"
  10:motorlib/config.cpp **** 
  11:motorlib/config.cpp **** const Config config;
  12:motorlib/config.cpp **** 
  13:motorlib/config.cpp **** Config::Config() :
 1035              		.loc 11 13 1
 1036              		.cfi_startproc
 1037              		@ args = 0, pretend = 0, frame = 8
 1038              		@ frame_needed = 1, uses_anonymous_args = 0
ARM GAS  /tmp/ccMP84Nn.s 			page 21


 1039              		@ link register save eliminated.
 1040 0000 80B4     		push	{r7}
 1041              	.LCFI77:
 1042              		.cfi_def_cfa_offset 4
 1043              		.cfi_offset 7, -4
 1044 0002 83B0     		sub	sp, sp, #12
 1045              	.LCFI78:
 1046              		.cfi_def_cfa_offset 16
 1047 0004 00AF     		add	r7, sp, #0
 1048              	.LCFI79:
 1049              		.cfi_def_cfa_register 7
 1050 0006 7860     		str	r0, [r7, #4]
 1051              	.LBB13:
  14:motorlib/config.cpp ****     fast_loop(config_items.fast_loop),
  15:motorlib/config.cpp ****     main_loop(config_items.main_loop)
 1052              		.loc 11 15 37
 1053 0008 7B68     		ldr	r3, [r7, #4]
 1054 000a 064A     		ldr	r2, .L48
 1055 000c 1A60     		str	r2, [r3]
 1056 000e 7B68     		ldr	r3, [r7, #4]
 1057 0010 054A     		ldr	r2, .L48+4
 1058 0012 5A60     		str	r2, [r3, #4]
 1059              	.LBE13:
  16:motorlib/config.cpp ****     {}...
 1060              		.loc 11 16 6
 1061 0014 7B68     		ldr	r3, [r7, #4]
 1062 0016 1846     		mov	r0, r3
 1063 0018 0C37     		adds	r7, r7, #12
 1064              	.LCFI80:
 1065              		.cfi_def_cfa_offset 4
 1066 001a BD46     		mov	sp, r7
 1067              	.LCFI81:
 1068              		.cfi_def_cfa_register 13
 1069              		@ sp needed
 1070 001c 5DF8047B 		ldr	r7, [sp], #4
 1071              	.LCFI82:
 1072              		.cfi_restore 7
 1073              		.cfi_def_cfa_offset 0
 1074 0020 7047     		bx	lr
 1075              	.L49:
 1076 0022 00BF     		.align	2
 1077              	.L48:
 1078 0024 60000000 		.word	_ZL12config_items+96
 1079 0028 88110000 		.word	_ZL12config_items+4488
 1080              		.cfi_endproc
 1081              	.LFE401:
 1082              		.cantunwind
 1083              		.fnend
 1085              		.global	_ZN6ConfigC1Ev
 1086              		.thumb_set _ZN6ConfigC1Ev,_ZN6ConfigC2Ev
 1087              		.weak	_ZTV7Encoder
 1088              		.section	.rodata._ZTV7Encoder,"aG",%progbits,_ZTV7Encoder,comdat
 1089              		.align	2
 1092              	_ZTV7Encoder:
 1093 0000 00000000 		.word	0
 1094 0004 00000000 		.word	_ZTI7Encoder
 1095 0008 00000000 		.word	_ZN7Encoder9get_valueEv
ARM GAS  /tmp/ccMP84Nn.s 			page 22


 1096 000c 00000000 		.word	_ZN7Encoder7triggerEv
 1097              		.weak	_ZTV13Communication
 1098              		.section	.rodata._ZTV13Communication,"aG",%progbits,_ZTV13Communication,comdat
 1099              		.align	2
 1102              	_ZTV13Communication:
 1103 0000 00000000 		.word	0
 1104 0004 00000000 		.word	_ZTI13Communication
 1105 0008 00000000 		.word	__cxa_pure_virtual
 1106 000c 00000000 		.word	__cxa_pure_virtual
 1107 0010 00000000 		.word	__cxa_pure_virtual
 1108              		.weak	_ZTI7Encoder
 1109              		.section	.rodata._ZTI7Encoder,"aG",%progbits,_ZTI7Encoder,comdat
 1110              		.align	2
 1113              	_ZTI7Encoder:
 1114 0000 08000000 		.word	_ZTVN10__cxxabiv117__class_type_infoE+8
 1115 0004 00000000 		.word	_ZTS7Encoder
 1116              		.weak	_ZTS7Encoder
 1117              		.section	.rodata._ZTS7Encoder,"aG",%progbits,_ZTS7Encoder,comdat
 1118              		.align	2
 1121              	_ZTS7Encoder:
 1122 0000 37456E63 		.ascii	"7Encoder\000"
 1122      6F646572 
 1122      00
 1123              		.weak	_ZTI13Communication
 1124              		.section	.rodata._ZTI13Communication,"aG",%progbits,_ZTI13Communication,comdat
 1125              		.align	2
 1128              	_ZTI13Communication:
 1129 0000 08000000 		.word	_ZTVN10__cxxabiv117__class_type_infoE+8
 1130 0004 00000000 		.word	_ZTS13Communication
 1131              		.weak	_ZTS13Communication
 1132              		.section	.rodata._ZTS13Communication,"aG",%progbits,_ZTS13Communication,comdat
 1133              		.align	2
 1136              	_ZTS13Communication:
 1137 0000 3133436F 		.ascii	"13Communication\000"
 1137      6D6D756E 
 1137      69636174 
 1137      696F6E00 
 1138              		.section	.text._ZN5._149D2Ev,"ax",%progbits
 1139              		.align	1
 1140              		.syntax unified
 1141              		.thumb
 1142              		.thumb_func
 1143              		.fpu fpv4-sp-d16
 1145              	_ZN5._149D2Ev:
 1146              		.fnstart
 1147              	.LFB405:
 1148              		.loc 10 7 15
 1149              		.cfi_startproc
 1150              		@ args = 0, pretend = 0, frame = 8
 1151              		@ frame_needed = 1, uses_anonymous_args = 0
 1152 0000 80B5     		push	{r7, lr}
 1153              	.LCFI83:
 1154              		.cfi_def_cfa_offset 8
 1155              		.cfi_offset 7, -8
 1156              		.cfi_offset 14, -4
 1157 0002 82B0     		sub	sp, sp, #8
 1158              	.LCFI84:
ARM GAS  /tmp/ccMP84Nn.s 			page 23


 1159              		.cfi_def_cfa_offset 16
 1160 0004 00AF     		add	r7, sp, #0
 1161              	.LCFI85:
 1162              		.cfi_def_cfa_register 7
 1163 0006 7860     		str	r0, [r7, #4]
 1164              	.LBB14:
 1165              		.loc 10 7 15
 1166 0008 7B68     		ldr	r3, [r7, #4]
 1167 000a 03F58A53 		add	r3, r3, #4416
 1168 000e 1C33     		adds	r3, r3, #28
 1169 0010 1846     		mov	r0, r3
 1170 0012 FFF7FEFF 		bl	_ZN13PIDControllerD1Ev
 1171 0016 7B68     		ldr	r3, [r7, #4]
 1172 0018 6033     		adds	r3, r3, #96
 1173 001a 1846     		mov	r0, r3
 1174 001c FFF7FEFF 		bl	_ZN8FastLoopD1Ev
 1175              	.LBE14:
 1176 0020 7B68     		ldr	r3, [r7, #4]
 1177 0022 1846     		mov	r0, r3
 1178 0024 0837     		adds	r7, r7, #8
 1179              	.LCFI86:
 1180              		.cfi_def_cfa_offset 8
 1181 0026 BD46     		mov	sp, r7
 1182              	.LCFI87:
 1183              		.cfi_def_cfa_register 13
 1184              		@ sp needed
 1185 0028 80BD     		pop	{r7, pc}
 1186              		.cfi_endproc
 1187              	.LFE405:
 1188              		.cantunwind
 1189              		.fnend
 1191              		.thumb_set _ZN5._149D1Ev,_ZN5._149D2Ev
 1192              		.section	.text._Z41__static_initialization_and_destruction_0ii,"ax",%progbits
 1193              		.align	1
 1194              		.syntax unified
 1195              		.thumb
 1196              		.thumb_func
 1197              		.fpu fpv4-sp-d16
 1199              	_Z41__static_initialization_and_destruction_0ii:
 1200              		.fnstart
 1201              	.LFB403:
 1202              		.loc 11 16 6
 1203              		.cfi_startproc
 1204              		@ args = 0, pretend = 0, frame = 8
 1205              		@ frame_needed = 1, uses_anonymous_args = 0
 1206 0000 80B5     		push	{r7, lr}
 1207              	.LCFI88:
 1208              		.cfi_def_cfa_offset 8
 1209              		.cfi_offset 7, -8
 1210              		.cfi_offset 14, -4
 1211 0002 82B0     		sub	sp, sp, #8
 1212              	.LCFI89:
 1213              		.cfi_def_cfa_offset 16
 1214 0004 00AF     		add	r7, sp, #0
 1215              	.LCFI90:
 1216              		.cfi_def_cfa_register 7
 1217 0006 7860     		str	r0, [r7, #4]
ARM GAS  /tmp/ccMP84Nn.s 			page 24


 1218 0008 3960     		str	r1, [r7]
 1219              		.loc 11 16 6
 1220 000a 7B68     		ldr	r3, [r7, #4]
 1221 000c 012B     		cmp	r3, #1
 1222 000e 0FD1     		bne	.L54
 1223              		.loc 11 16 6 is_stmt 0 discriminator 1
 1224 0010 3B68     		ldr	r3, [r7]
 1225 0012 4FF6FF72 		movw	r2, #65535
 1226 0016 9342     		cmp	r3, r2
 1227 0018 0AD1     		bne	.L54
   8:motorlib/dev_00_aksim2.cpp ****     GPIO motor_encoder_cs = {*GPIOA, 15, GPIO::OUTPUT};
   9:motorlib/dev_00_aksim2.cpp ****     Aksim2Encoder motor_encoder = {*SPI3, motor_encoder_cs};
  10:motorlib/dev_00_aksim2.cpp ****     GPIO enable = {*GPIOC, 14, GPIO::OUTPUT};
  11:motorlib/dev_00_aksim2.cpp ****     PWM motor_pwm = {initial_param.fast_loop_param.pwm_frequency, *const_cast<uint32_t*>(&HRTIM1_TI
  12:motorlib/dev_00_aksim2.cpp ****                           *const_cast<uint32_t*>(&HRTIM1_TIME->CMP1xR), 
  13:motorlib/dev_00_aksim2.cpp ****                           *const_cast<uint32_t*>(&HRTIM1_TIMA->CMP1xR),
  14:motorlib/dev_00_aksim2.cpp ****                           *TIM8, enable};
  15:motorlib/dev_00_aksim2.cpp ****     FastLoop fast_loop = {motor_pwm, motor_encoder};
  16:motorlib/dev_00_aksim2.cpp ****     LED led = {const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR1)), 
  17:motorlib/dev_00_aksim2.cpp ****                const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR2)),
  18:motorlib/dev_00_aksim2.cpp ****                const_cast<uint16_t*>(reinterpret_cast<volatile uint16_t *>(&TIM3->CCR4)), true};
  19:motorlib/dev_00_aksim2.cpp ****     PIDController controller;
  20:motorlib/dev_00_aksim2.cpp ****     USBCommunication communication;
  21:motorlib/dev_00_aksim2.cpp ****     MainLoop main_loop = {controller, communication, led};
  22:motorlib/dev_00_aksim2.cpp **** } config_items;
 1228              		.loc 10 22 3 is_stmt 1
 1229 001a 0748     		ldr	r0, .L55
 1230 001c FFF7FEFF 		bl	_ZN5._149C1Ev
 1231 0020 064A     		ldr	r2, .L55+4
 1232 0022 0749     		ldr	r1, .L55+8
 1233 0024 0448     		ldr	r0, .L55
 1234 0026 FFF7FEFF 		bl	__aeabi_atexit
  11:motorlib/config.cpp **** const Config config;
 1235              		.loc 11 11 14
 1236 002a 0648     		ldr	r0, .L55+12
 1237 002c FFF7FEFF 		bl	_ZN6ConfigC1Ev
 1238              	.L54:
 1239              		.loc 11 16 6
 1240 0030 00BF     		nop
 1241 0032 0837     		adds	r7, r7, #8
 1242              	.LCFI91:
 1243              		.cfi_def_cfa_offset 8
 1244 0034 BD46     		mov	sp, r7
 1245              	.LCFI92:
 1246              		.cfi_def_cfa_register 13
 1247              		@ sp needed
 1248 0036 80BD     		pop	{r7, pc}
 1249              	.L56:
 1250              		.align	2
 1251              	.L55:
 1252 0038 00000000 		.word	_ZL12config_items
 1253 003c 00000000 		.word	__dso_handle
 1254 0040 00000000 		.word	_ZN5._149D1Ev
 1255 0044 00000000 		.word	config
 1256              		.cfi_endproc
 1257              	.LFE403:
 1258              		.cantunwind
ARM GAS  /tmp/ccMP84Nn.s 			page 25


 1259              		.fnend
 1261              		.section	.text._GLOBAL__sub_I_config,"ax",%progbits
 1262              		.align	1
 1263              		.syntax unified
 1264              		.thumb
 1265              		.thumb_func
 1266              		.fpu fpv4-sp-d16
 1268              	_GLOBAL__sub_I_config:
 1269              		.fnstart
 1270              	.LFB407:
 1271              		.loc 11 16 6
 1272              		.cfi_startproc
 1273              		@ args = 0, pretend = 0, frame = 0
 1274              		@ frame_needed = 1, uses_anonymous_args = 0
 1275 0000 80B5     		push	{r7, lr}
 1276              	.LCFI93:
 1277              		.cfi_def_cfa_offset 8
 1278              		.cfi_offset 7, -8
 1279              		.cfi_offset 14, -4
 1280 0002 00AF     		add	r7, sp, #0
 1281              	.LCFI94:
 1282              		.cfi_def_cfa_register 7
 1283              		.loc 11 16 6
 1284 0004 4FF6FF71 		movw	r1, #65535
 1285 0008 0120     		movs	r0, #1
 1286 000a FFF7FEFF 		bl	_Z41__static_initialization_and_destruction_0ii
 1287 000e 80BD     		pop	{r7, pc}
 1288              		.cfi_endproc
 1289              	.LFE407:
 1290              		.cantunwind
 1291              		.fnend
 1293              		.section	.init_array,"aw",%init_array
 1294              		.align	2
 1295 0000 00000000 		.word	_GLOBAL__sub_I_config(target1)
 1296              		.text
 1297              	.Letext0:
 1298              		.file 12 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdint"
 1299              		.file 13 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/cpp_type_tr
 1300              		.file 14 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cmath"
 1301              		.file 15 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/th
 1302              		.file 16 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 1303              		.file 17 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 1304              		.file 18 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdint.h"
 1305              		.file 19 "motorlib/messages.h"
 1306              		.file 20 "motorlib/fast_loop.h"
 1307              		.file 21 "/opt/gcc-arm-none-eabi-8-2019-q3-update/lib/gcc/arm-none-eabi/8.3.1/include/stddef.h"
 1308              		.file 22 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_types.h"
 1309              		.file 23 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/reent.h"
 1310              		.file 24 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/lock.h"
 1311              		.file 25 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/math.h"
 1312              		.file 26 "motorlib/config.h"
 1313              		.file 27 "Drivers/CMSIS/Include/core_cm4.h"
 1314              		.file 28 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/system_stm32g4xx.h"
 1315              		.file 29 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/stm32g474xx.h"
 1316              		.file 30 "motorlib/gpio.h"
 1317              		.file 31 "<built-in>"
ARM GAS  /tmp/ccMP84Nn.s 			page 26


DEFINED SYMBOLS
                            *ABS*:0000000000000000 config.cpp
     /tmp/ccMP84Nn.s:18     .text._ZN8MainLoopC2ER13PIDControllerR13CommunicationR3LED:0000000000000000 $t
     /tmp/ccMP84Nn.s:26     .text._ZN8MainLoopC2ER13PIDControllerR13CommunicationR3LED:0000000000000000 _ZN8MainLoopC2ER13PIDControllerR13CommunicationR3LED
.ARM.exidx.text._ZN8MainLoopC2ER13PIDControllerR13CommunicationR3LED:0000000000000000 $d
     /tmp/ccMP84Nn.s:26     .text._ZN8MainLoopC2ER13PIDControllerR13CommunicationR3LED:0000000000000000 _ZN8MainLoopC1ER13PIDControllerR13CommunicationR3LED
     /tmp/ccMP84Nn.s:101    .text._ZN3LEDC2EPtS0_S0_b:0000000000000000 $t
     /tmp/ccMP84Nn.s:108    .text._ZN3LEDC2EPtS0_S0_b:0000000000000000 _ZN3LEDC2EPtS0_S0_b
.ARM.exidx.text._ZN3LEDC2EPtS0_S0_b:0000000000000000 $d
     /tmp/ccMP84Nn.s:108    .text._ZN3LEDC2EPtS0_S0_b:0000000000000000 _ZN3LEDC1EPtS0_S0_b
     /tmp/ccMP84Nn.s:183    .text._ZN13PIDControllerD2Ev:0000000000000000 $t
     /tmp/ccMP84Nn.s:190    .text._ZN13PIDControllerD2Ev:0000000000000000 _ZN13PIDControllerD2Ev
     /tmp/ccMP84Nn.s:234    .text._ZN13PIDControllerD2Ev:000000000000001c $d
.ARM.exidx.text._ZN13PIDControllerD2Ev:0000000000000000 $d
     /tmp/ccMP84Nn.s:190    .text._ZN13PIDControllerD2Ev:0000000000000000 _ZN13PIDControllerD1Ev
     /tmp/ccMP84Nn.s:243    .text._ZN13PIDControllerD0Ev:0000000000000000 $t
     /tmp/ccMP84Nn.s:250    .text._ZN13PIDControllerD0Ev:0000000000000000 _ZN13PIDControllerD0Ev
.ARM.exidx.text._ZN13PIDControllerD0Ev:0000000000000000 $d
     /tmp/ccMP84Nn.s:290    .text._ZN7EncoderC2EPVl:0000000000000000 $t
     /tmp/ccMP84Nn.s:297    .text._ZN7EncoderC2EPVl:0000000000000000 _ZN7EncoderC2EPVl
     /tmp/ccMP84Nn.s:346    .text._ZN7EncoderC2EPVl:0000000000000024 $d
     /tmp/ccMP84Nn.s:1092   .rodata._ZTV7Encoder:0000000000000000 _ZTV7Encoder
.ARM.exidx.text._ZN7EncoderC2EPVl:0000000000000000 $d
     /tmp/ccMP84Nn.s:297    .text._ZN7EncoderC2EPVl:0000000000000000 _ZN7EncoderC1EPVl
     /tmp/ccMP84Nn.s:355    .text._ZN7Encoder9get_valueEv:0000000000000000 $t
     /tmp/ccMP84Nn.s:362    .text._ZN7Encoder9get_valueEv:0000000000000000 _ZN7Encoder9get_valueEv
.ARM.exidx.text._ZN7Encoder9get_valueEv:0000000000000000 $d
     /tmp/ccMP84Nn.s:405    .text._ZN7Encoder7triggerEv:0000000000000000 $t
     /tmp/ccMP84Nn.s:412    .text._ZN7Encoder7triggerEv:0000000000000000 _ZN7Encoder7triggerEv
.ARM.exidx.text._ZN7Encoder7triggerEv:0000000000000000 $d
     /tmp/ccMP84Nn.s:451    .text._ZN3PWMC2EmRmS0_S0_R11TIM_TypeDefR4GPIO:0000000000000000 $t
     /tmp/ccMP84Nn.s:458    .text._ZN3PWMC2EmRmS0_S0_R11TIM_TypeDefR4GPIO:0000000000000000 _ZN3PWMC2EmRmS0_S0_R11TIM_TypeDefR4GPIO
.ARM.extab.text._ZN3PWMC2EmRmS0_S0_R11TIM_TypeDefR4GPIO:0000000000000000 $d
.ARM.exidx.text._ZN3PWMC2EmRmS0_S0_R11TIM_TypeDefR4GPIO:0000000000000000 $d
     /tmp/ccMP84Nn.s:458    .text._ZN3PWMC2EmRmS0_S0_R11TIM_TypeDefR4GPIO:0000000000000000 _ZN3PWMC1EmRmS0_S0_R11TIM_TypeDefR4GPIO
     /tmp/ccMP84Nn.s:528    .text._ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO:0000000000000000 $t
     /tmp/ccMP84Nn.s:535    .text._ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO:0000000000000000 _ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO
     /tmp/ccMP84Nn.s:587    .text._ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO:0000000000000034 $d
.ARM.exidx.text._ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO:0000000000000000 $d
     /tmp/ccMP84Nn.s:535    .text._ZN10SPIEncoderC2ER11SPI_TypeDefR4GPIO:0000000000000000 _ZN10SPIEncoderC1ER11SPI_TypeDefR4GPIO
     /tmp/ccMP84Nn.s:596    .text._ZN13Aksim2EncoderC2ER11SPI_TypeDefR4GPIO:0000000000000000 $t
     /tmp/ccMP84Nn.s:603    .text._ZN13Aksim2EncoderC2ER11SPI_TypeDefR4GPIO:0000000000000000 _ZN13Aksim2EncoderC2ER11SPI_TypeDefR4GPIO
     /tmp/ccMP84Nn.s:675    .text._ZN13Aksim2EncoderC2ER11SPI_TypeDefR4GPIO:000000000000005c $d
.ARM.exidx.text._ZN13Aksim2EncoderC2ER11SPI_TypeDefR4GPIO:0000000000000000 $d
     /tmp/ccMP84Nn.s:603    .text._ZN13Aksim2EncoderC2ER11SPI_TypeDefR4GPIO:0000000000000000 _ZN13Aksim2EncoderC1ER11SPI_TypeDefR4GPIO
     /tmp/ccMP84Nn.s:684    .text._ZN13PIDControllerC2Ev:0000000000000000 $t
     /tmp/ccMP84Nn.s:691    .text._ZN13PIDControllerC2Ev:0000000000000000 _ZN13PIDControllerC2Ev
     /tmp/ccMP84Nn.s:754    .text._ZN13PIDControllerC2Ev:0000000000000054 $d
.ARM.exidx.text._ZN13PIDControllerC2Ev:0000000000000000 $d
     /tmp/ccMP84Nn.s:691    .text._ZN13PIDControllerC2Ev:0000000000000000 _ZN13PIDControllerC1Ev
     /tmp/ccMP84Nn.s:763    .text._ZN13CommunicationC2Ev:0000000000000000 $t
     /tmp/ccMP84Nn.s:770    .text._ZN13CommunicationC2Ev:0000000000000000 _ZN13CommunicationC2Ev
     /tmp/ccMP84Nn.s:813    .text._ZN13CommunicationC2Ev:000000000000001c $d
     /tmp/ccMP84Nn.s:1102   .rodata._ZTV13Communication:0000000000000000 _ZTV13Communication
.ARM.exidx.text._ZN13CommunicationC2Ev:0000000000000000 $d
     /tmp/ccMP84Nn.s:770    .text._ZN13CommunicationC2Ev:0000000000000000 _ZN13CommunicationC1Ev
     /tmp/ccMP84Nn.s:822    .text._ZN16USBCommunicationC2Ev:0000000000000000 $t
ARM GAS  /tmp/ccMP84Nn.s 			page 27


     /tmp/ccMP84Nn.s:829    .text._ZN16USBCommunicationC2Ev:0000000000000000 _ZN16USBCommunicationC2Ev
     /tmp/ccMP84Nn.s:871    .text._ZN16USBCommunicationC2Ev:0000000000000020 $d
.ARM.exidx.text._ZN16USBCommunicationC2Ev:0000000000000000 $d
     /tmp/ccMP84Nn.s:829    .text._ZN16USBCommunicationC2Ev:0000000000000000 _ZN16USBCommunicationC1Ev
     /tmp/ccMP84Nn.s:880    .text._ZN5._149C2Ev:0000000000000000 $t
     /tmp/ccMP84Nn.s:886    .text._ZN5._149C2Ev:0000000000000000 _ZN5._149C2Ev
     /tmp/ccMP84Nn.s:994    .text._ZN5._149C2Ev:00000000000000cc $d
    .ARM.exidx.text._ZN5._149C2Ev:0000000000000000 $d
     /tmp/ccMP84Nn.s:886    .text._ZN5._149C2Ev:0000000000000000 _ZN5._149C1Ev
     /tmp/ccMP84Nn.s:1011   .bss._ZL12config_items:0000000000000000 $d
     /tmp/ccMP84Nn.s:1014   .bss._ZL12config_items:0000000000000000 _ZL12config_items
     /tmp/ccMP84Nn.s:1021   .bss.config:0000000000000000 config
     /tmp/ccMP84Nn.s:1018   .bss.config:0000000000000000 $d
     /tmp/ccMP84Nn.s:1024   .text._ZN6ConfigC2Ev:0000000000000000 $t
     /tmp/ccMP84Nn.s:1031   .text._ZN6ConfigC2Ev:0000000000000000 _ZN6ConfigC2Ev
     /tmp/ccMP84Nn.s:1078   .text._ZN6ConfigC2Ev:0000000000000024 $d
   .ARM.exidx.text._ZN6ConfigC2Ev:0000000000000000 $d
     /tmp/ccMP84Nn.s:1031   .text._ZN6ConfigC2Ev:0000000000000000 _ZN6ConfigC1Ev
     /tmp/ccMP84Nn.s:1089   .rodata._ZTV7Encoder:0000000000000000 $d
     /tmp/ccMP84Nn.s:1113   .rodata._ZTI7Encoder:0000000000000000 _ZTI7Encoder
     /tmp/ccMP84Nn.s:1099   .rodata._ZTV13Communication:0000000000000000 $d
     /tmp/ccMP84Nn.s:1128   .rodata._ZTI13Communication:0000000000000000 _ZTI13Communication
     /tmp/ccMP84Nn.s:1110   .rodata._ZTI7Encoder:0000000000000000 $d
     /tmp/ccMP84Nn.s:1121   .rodata._ZTS7Encoder:0000000000000000 _ZTS7Encoder
     /tmp/ccMP84Nn.s:1118   .rodata._ZTS7Encoder:0000000000000000 $d
     /tmp/ccMP84Nn.s:1125   .rodata._ZTI13Communication:0000000000000000 $d
     /tmp/ccMP84Nn.s:1136   .rodata._ZTS13Communication:0000000000000000 _ZTS13Communication
     /tmp/ccMP84Nn.s:1133   .rodata._ZTS13Communication:0000000000000000 $d
     /tmp/ccMP84Nn.s:1139   .text._ZN5._149D2Ev:0000000000000000 $t
     /tmp/ccMP84Nn.s:1145   .text._ZN5._149D2Ev:0000000000000000 _ZN5._149D2Ev
    .ARM.exidx.text._ZN5._149D2Ev:0000000000000000 $d
     /tmp/ccMP84Nn.s:1145   .text._ZN5._149D2Ev:0000000000000000 _ZN5._149D1Ev
     /tmp/ccMP84Nn.s:1193   .text._Z41__static_initialization_and_destruction_0ii:0000000000000000 $t
     /tmp/ccMP84Nn.s:1199   .text._Z41__static_initialization_and_destruction_0ii:0000000000000000 _Z41__static_initialization_and_destruction_0ii
     /tmp/ccMP84Nn.s:1252   .text._Z41__static_initialization_and_destruction_0ii:0000000000000038 $d
.ARM.exidx.text._Z41__static_initialization_and_destruction_0ii:0000000000000000 $d
     /tmp/ccMP84Nn.s:1262   .text._GLOBAL__sub_I_config:0000000000000000 $t
     /tmp/ccMP84Nn.s:1268   .text._GLOBAL__sub_I_config:0000000000000000 _GLOBAL__sub_I_config
.ARM.exidx.text._GLOBAL__sub_I_config:0000000000000000 $d
     /tmp/ccMP84Nn.s:1294   .init_array:0000000000000000 $d
                           .group:0000000000000000 _ZN8MainLoopC5ER13PIDControllerR13CommunicationR3LED
                           .group:0000000000000000 _ZN3LEDC5EPtS0_S0_b
                           .group:0000000000000000 _ZN13PIDControllerD5Ev
                           .group:0000000000000000 _ZN7EncoderC5EPVl
                           .group:0000000000000000 _ZN3PWMC5EmRmS0_S0_R11TIM_TypeDefR4GPIO
                           .group:0000000000000000 _ZN10SPIEncoderC5ER11SPI_TypeDefR4GPIO
                           .group:0000000000000000 _ZN13Aksim2EncoderC5ER11SPI_TypeDefR4GPIO
                           .group:0000000000000000 _ZN13PIDControllerC5Ev
                           .group:0000000000000000 _ZN13CommunicationC5Ev
                           .group:0000000000000000 _ZN16USBCommunicationC5Ev

UNDEFINED SYMBOLS
memset
_ZTV13PIDController
_ZdlPv
_ZN3PWM16set_frequency_hzEm
_ZN3PWM8set_vbusEf
ARM GAS  /tmp/ccMP84Nn.s 			page 28


__aeabi_unwind_cpp_pr1
_ZTV10SPIEncoder
_ZTV13Aksim2Encoder
_ZTV16USBCommunication
_ZN4GPIOC1ER12GPIO_TypeDefhNS_9DirectionE
_ZN8FastLoopC1ER3PWMR7Encoder
initial_param
__cxa_pure_virtual
_ZTVN10__cxxabiv117__class_type_infoE
_ZN8FastLoopD1Ev
__aeabi_atexit
__dso_handle
