ARM GAS  /tmp/ccpFMNt0.s 			page 1


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
  13              		.file	"foc_i.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.text._ZN8FastLoop10set_id_desEf,"axG",%progbits,_ZN8FastLoop10set_id_desEf,comdat
  18              		.align	1
  19              		.weak	_ZN8FastLoop10set_id_desEf
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN8FastLoop10set_id_desEf:
  27              		.fnstart
  28              	.LFB0:
  29              		.file 1 "motorlib/fast_loop.h"
   1:motorlib/fast_loop.h **** #ifndef FAST_LOOP_H
   2:motorlib/fast_loop.h **** #define FAST_LOOP_H
   3:motorlib/fast_loop.h **** 
   4:motorlib/fast_loop.h **** #include <cstdint>
   5:motorlib/fast_loop.h **** #include "messages.h"
   6:motorlib/fast_loop.h **** 
   7:motorlib/fast_loop.h **** class FOC;
   8:motorlib/fast_loop.h **** class PWM;
   9:motorlib/fast_loop.h **** class Encoder;
  10:motorlib/fast_loop.h **** 
  11:motorlib/fast_loop.h **** class FastLoop {
  12:motorlib/fast_loop.h ****  public:
  13:motorlib/fast_loop.h ****     FastLoop(PWM &pwm, Encoder &encoder); // TODO consider changing encoder to template
  14:motorlib/fast_loop.h ****     ~FastLoop();
  15:motorlib/fast_loop.h ****     void update()  __attribute__((section (".ccmram")));;
  16:motorlib/fast_loop.h ****     void maintenance();
  17:motorlib/fast_loop.h ****     void set_id_des(float id) { id_des = id; }
  30              		.loc 1 17 10
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
ARM GAS  /tmp/ccpFMNt0.s 			page 2


  42 0004 00AF     		add	r7, sp, #0
  43              	.LCFI2:
  44              		.cfi_def_cfa_register 7
  45 0006 7860     		str	r0, [r7, #4]
  46 0008 87ED000A 		vstr.32	s0, [r7]
  47              		.loc 1 17 40
  48 000c 7B68     		ldr	r3, [r7, #4]
  49 000e 03F58453 		add	r3, r3, #4224
  50 0012 1433     		adds	r3, r3, #20
  51 0014 3A68     		ldr	r2, [r7]	@ float
  52 0016 1A60     		str	r2, [r3]	@ float
  53              		.loc 1 17 46
  54 0018 00BF     		nop
  55 001a 0C37     		adds	r7, r7, #12
  56              	.LCFI3:
  57              		.cfi_def_cfa_offset 4
  58 001c BD46     		mov	sp, r7
  59              	.LCFI4:
  60              		.cfi_def_cfa_register 13
  61              		@ sp needed
  62 001e 5DF8047B 		ldr	r7, [sp], #4
  63              	.LCFI5:
  64              		.cfi_restore 7
  65              		.cfi_def_cfa_offset 0
  66 0022 7047     		bx	lr
  67              		.cfi_endproc
  68              	.LFE0:
  69              		.cantunwind
  70              		.fnend
  72              		.section	.text._ZN8FastLoop10set_iq_desEf,"axG",%progbits,_ZN8FastLoop10set_iq_desEf,comdat
  73              		.align	1
  74              		.weak	_ZN8FastLoop10set_iq_desEf
  75              		.syntax unified
  76              		.thumb
  77              		.thumb_func
  78              		.fpu fpv4-sp-d16
  80              	_ZN8FastLoop10set_iq_desEf:
  81              		.fnstart
  82              	.LFB1:
  18:motorlib/fast_loop.h ****     void set_iq_des(float iq) { if (mode_ == CURRENT_MODE) iq_des = iq; }
  83              		.loc 1 18 10
  84              		.cfi_startproc
  85              		@ args = 0, pretend = 0, frame = 8
  86              		@ frame_needed = 1, uses_anonymous_args = 0
  87              		@ link register save eliminated.
  88 0000 80B4     		push	{r7}
  89              	.LCFI6:
  90              		.cfi_def_cfa_offset 4
  91              		.cfi_offset 7, -4
  92 0002 83B0     		sub	sp, sp, #12
  93              	.LCFI7:
  94              		.cfi_def_cfa_offset 16
  95 0004 00AF     		add	r7, sp, #0
  96              	.LCFI8:
  97              		.cfi_def_cfa_register 7
  98 0006 7860     		str	r0, [r7, #4]
  99 0008 87ED000A 		vstr.32	s0, [r7]
ARM GAS  /tmp/ccpFMNt0.s 			page 3


 100              		.loc 1 18 37
 101 000c 7B68     		ldr	r3, [r7, #4]
 102 000e 03F58353 		add	r3, r3, #4192
 103 0012 0833     		adds	r3, r3, #8
 104 0014 1B78     		ldrb	r3, [r3]	@ zero_extendqisi2
 105              		.loc 1 18 33
 106 0016 022B     		cmp	r3, #2
 107 0018 05D1     		bne	.L4
 108              		.loc 1 18 67 discriminator 1
 109 001a 7B68     		ldr	r3, [r7, #4]
 110 001c 03F58453 		add	r3, r3, #4224
 111 0020 1033     		adds	r3, r3, #16
 112 0022 3A68     		ldr	r2, [r7]	@ float
 113 0024 1A60     		str	r2, [r3]	@ float
 114              	.L4:
 115              		.loc 1 18 73
 116 0026 00BF     		nop
 117 0028 0C37     		adds	r7, r7, #12
 118              	.LCFI9:
 119              		.cfi_def_cfa_offset 4
 120 002a BD46     		mov	sp, r7
 121              	.LCFI10:
 122              		.cfi_def_cfa_register 13
 123              		@ sp needed
 124 002c 5DF8047B 		ldr	r7, [sp], #4
 125              	.LCFI11:
 126              		.cfi_restore 7
 127              		.cfi_def_cfa_offset 0
 128 0030 7047     		bx	lr
 129              		.cfi_endproc
 130              	.LFE1:
 131              		.cantunwind
 132              		.fnend
 134              		.section	.text.system_init,"ax",%progbits
 135              		.align	1
 136              		.global	system_init
 137              		.syntax unified
 138              		.thumb
 139              		.thumb_func
 140              		.fpu fpv4-sp-d16
 142              	system_init:
 143              		.fnstart
 144              	.LFB230:
 145              		.file 2 "motorlib/foc_i.cpp"
   1:motorlib/foc_i.cpp **** #include "foc_i.h"
   2:motorlib/foc_i.cpp **** #include "config.h"
   3:motorlib/foc_i.cpp **** 
   4:motorlib/foc_i.cpp **** void system_init() {
 146              		.loc 2 4 20
 147              		.cfi_startproc
 148              		@ args = 0, pretend = 0, frame = 0
 149              		@ frame_needed = 1, uses_anonymous_args = 0
 150 0000 80B5     		push	{r7, lr}
 151              		.save {r7, lr}
 152              	.LCFI12:
 153              		.cfi_def_cfa_offset 8
 154              		.cfi_offset 7, -8
ARM GAS  /tmp/ccpFMNt0.s 			page 4


 155              		.cfi_offset 14, -4
 156              		.setfp r7, sp, #0
 157 0002 00AF     		add	r7, sp, #0
 158              	.LCFI13:
 159              		.cfi_def_cfa_register 7
   5:motorlib/foc_i.cpp ****     config.main_loop.init();
 160              		.loc 2 5 12
 161 0004 034B     		ldr	r3, .L6
 162 0006 5B68     		ldr	r3, [r3, #4]
 163              		.loc 2 5 26
 164 0008 1846     		mov	r0, r3
 165 000a FFF7FEFF 		bl	_ZN8MainLoop4initEv
   6:motorlib/foc_i.cpp **** }
 166              		.loc 2 6 1
 167 000e 00BF     		nop
 168 0010 80BD     		pop	{r7, pc}
 169              	.L7:
 170 0012 00BF     		.align	2
 171              	.L6:
 172 0014 00000000 		.word	config
 173              		.cfi_endproc
 174              	.LFE230:
 175              		.fnend
 177              		.section	.text.fast_loop_update,"ax",%progbits
 178              		.align	1
 179              		.global	fast_loop_update
 180              		.syntax unified
 181              		.thumb
 182              		.thumb_func
 183              		.fpu fpv4-sp-d16
 185              	fast_loop_update:
 186              		.fnstart
 187              	.LFB231:
   7:motorlib/foc_i.cpp **** 
   8:motorlib/foc_i.cpp **** void fast_loop_update() {
 188              		.loc 2 8 25
 189              		.cfi_startproc
 190              		@ args = 0, pretend = 0, frame = 0
 191              		@ frame_needed = 1, uses_anonymous_args = 0
 192 0000 80B5     		push	{r7, lr}
 193              		.save {r7, lr}
 194              	.LCFI14:
 195              		.cfi_def_cfa_offset 8
 196              		.cfi_offset 7, -8
 197              		.cfi_offset 14, -4
 198              		.setfp r7, sp, #0
 199 0002 00AF     		add	r7, sp, #0
 200              	.LCFI15:
 201              		.cfi_def_cfa_register 7
   9:motorlib/foc_i.cpp ****     config.fast_loop.update();
 202              		.loc 2 9 12
 203 0004 034B     		ldr	r3, .L9
 204 0006 1B68     		ldr	r3, [r3]
 205              		.loc 2 9 28
 206 0008 1846     		mov	r0, r3
 207 000a FFF7FEFF 		bl	_ZN8FastLoop6updateEv
  10:motorlib/foc_i.cpp **** }
ARM GAS  /tmp/ccpFMNt0.s 			page 5


 208              		.loc 2 10 1
 209 000e 00BF     		nop
 210 0010 80BD     		pop	{r7, pc}
 211              	.L10:
 212 0012 00BF     		.align	2
 213              	.L9:
 214 0014 00000000 		.word	config
 215              		.cfi_endproc
 216              	.LFE231:
 217              		.fnend
 219              		.section	.text.fast_loop_maintenance,"ax",%progbits
 220              		.align	1
 221              		.global	fast_loop_maintenance
 222              		.syntax unified
 223              		.thumb
 224              		.thumb_func
 225              		.fpu fpv4-sp-d16
 227              	fast_loop_maintenance:
 228              		.fnstart
 229              	.LFB232:
  11:motorlib/foc_i.cpp **** 
  12:motorlib/foc_i.cpp **** void fast_loop_maintenance() {
 230              		.loc 2 12 30
 231              		.cfi_startproc
 232              		@ args = 0, pretend = 0, frame = 0
 233              		@ frame_needed = 1, uses_anonymous_args = 0
 234 0000 80B5     		push	{r7, lr}
 235              		.save {r7, lr}
 236              	.LCFI16:
 237              		.cfi_def_cfa_offset 8
 238              		.cfi_offset 7, -8
 239              		.cfi_offset 14, -4
 240              		.setfp r7, sp, #0
 241 0002 00AF     		add	r7, sp, #0
 242              	.LCFI17:
 243              		.cfi_def_cfa_register 7
  13:motorlib/foc_i.cpp ****     config.fast_loop.maintenance();
 244              		.loc 2 13 12
 245 0004 034B     		ldr	r3, .L12
 246 0006 1B68     		ldr	r3, [r3]
 247              		.loc 2 13 33
 248 0008 1846     		mov	r0, r3
 249 000a FFF7FEFF 		bl	_ZN8FastLoop11maintenanceEv
  14:motorlib/foc_i.cpp **** }
 250              		.loc 2 14 1
 251 000e 00BF     		nop
 252 0010 80BD     		pop	{r7, pc}
 253              	.L13:
 254 0012 00BF     		.align	2
 255              	.L12:
 256 0014 00000000 		.word	config
 257              		.cfi_endproc
 258              	.LFE232:
 259              		.fnend
 261              		.section	.text.fast_loop_set_id_des,"ax",%progbits
 262              		.align	1
 263              		.global	fast_loop_set_id_des
ARM GAS  /tmp/ccpFMNt0.s 			page 6


 264              		.syntax unified
 265              		.thumb
 266              		.thumb_func
 267              		.fpu fpv4-sp-d16
 269              	fast_loop_set_id_des:
 270              		.fnstart
 271              	.LFB233:
  15:motorlib/foc_i.cpp **** 
  16:motorlib/foc_i.cpp **** void fast_loop_set_id_des(float id) {
 272              		.loc 2 16 37
 273              		.cfi_startproc
 274              		@ args = 0, pretend = 0, frame = 8
 275              		@ frame_needed = 1, uses_anonymous_args = 0
 276 0000 80B5     		push	{r7, lr}
 277              	.LCFI18:
 278              		.cfi_def_cfa_offset 8
 279              		.cfi_offset 7, -8
 280              		.cfi_offset 14, -4
 281 0002 82B0     		sub	sp, sp, #8
 282              	.LCFI19:
 283              		.cfi_def_cfa_offset 16
 284 0004 00AF     		add	r7, sp, #0
 285              	.LCFI20:
 286              		.cfi_def_cfa_register 7
 287 0006 87ED010A 		vstr.32	s0, [r7, #4]
  17:motorlib/foc_i.cpp ****     config.fast_loop.set_id_des(id);
 288              		.loc 2 17 12
 289 000a 054B     		ldr	r3, .L15
 290 000c 1B68     		ldr	r3, [r3]
 291              		.loc 2 17 32
 292 000e 97ED010A 		vldr.32	s0, [r7, #4]
 293 0012 1846     		mov	r0, r3
 294 0014 FFF7FEFF 		bl	_ZN8FastLoop10set_id_desEf
  18:motorlib/foc_i.cpp **** }
 295              		.loc 2 18 1
 296 0018 00BF     		nop
 297 001a 0837     		adds	r7, r7, #8
 298              	.LCFI21:
 299              		.cfi_def_cfa_offset 8
 300 001c BD46     		mov	sp, r7
 301              	.LCFI22:
 302              		.cfi_def_cfa_register 13
 303              		@ sp needed
 304 001e 80BD     		pop	{r7, pc}
 305              	.L16:
 306              		.align	2
 307              	.L15:
 308 0020 00000000 		.word	config
 309              		.cfi_endproc
 310              	.LFE233:
 311              		.cantunwind
 312              		.fnend
 314              		.section	.text.fast_loop_set_iq_des,"ax",%progbits
 315              		.align	1
 316              		.global	fast_loop_set_iq_des
 317              		.syntax unified
 318              		.thumb
ARM GAS  /tmp/ccpFMNt0.s 			page 7


 319              		.thumb_func
 320              		.fpu fpv4-sp-d16
 322              	fast_loop_set_iq_des:
 323              		.fnstart
 324              	.LFB234:
  19:motorlib/foc_i.cpp **** void fast_loop_set_iq_des(float iq) {
 325              		.loc 2 19 37
 326              		.cfi_startproc
 327              		@ args = 0, pretend = 0, frame = 8
 328              		@ frame_needed = 1, uses_anonymous_args = 0
 329 0000 80B5     		push	{r7, lr}
 330              	.LCFI23:
 331              		.cfi_def_cfa_offset 8
 332              		.cfi_offset 7, -8
 333              		.cfi_offset 14, -4
 334 0002 82B0     		sub	sp, sp, #8
 335              	.LCFI24:
 336              		.cfi_def_cfa_offset 16
 337 0004 00AF     		add	r7, sp, #0
 338              	.LCFI25:
 339              		.cfi_def_cfa_register 7
 340 0006 87ED010A 		vstr.32	s0, [r7, #4]
  20:motorlib/foc_i.cpp ****     config.fast_loop.set_iq_des(iq);
 341              		.loc 2 20 12
 342 000a 054B     		ldr	r3, .L18
 343 000c 1B68     		ldr	r3, [r3]
 344              		.loc 2 20 32
 345 000e 97ED010A 		vldr.32	s0, [r7, #4]
 346 0012 1846     		mov	r0, r3
 347 0014 FFF7FEFF 		bl	_ZN8FastLoop10set_iq_desEf
  21:motorlib/foc_i.cpp **** }
 348              		.loc 2 21 1
 349 0018 00BF     		nop
 350 001a 0837     		adds	r7, r7, #8
 351              	.LCFI26:
 352              		.cfi_def_cfa_offset 8
 353 001c BD46     		mov	sp, r7
 354              	.LCFI27:
 355              		.cfi_def_cfa_register 13
 356              		@ sp needed
 357 001e 80BD     		pop	{r7, pc}
 358              	.L19:
 359              		.align	2
 360              	.L18:
 361 0020 00000000 		.word	config
 362              		.cfi_endproc
 363              	.LFE234:
 364              		.cantunwind
 365              		.fnend
 367              		.section	.text.fast_loop_phase_lock_mode,"ax",%progbits
 368              		.align	1
 369              		.global	fast_loop_phase_lock_mode
 370              		.syntax unified
 371              		.thumb
 372              		.thumb_func
 373              		.fpu fpv4-sp-d16
 375              	fast_loop_phase_lock_mode:
ARM GAS  /tmp/ccpFMNt0.s 			page 8


 376              		.fnstart
 377              	.LFB235:
  22:motorlib/foc_i.cpp **** 
  23:motorlib/foc_i.cpp **** void fast_loop_phase_lock_mode(float id) {
 378              		.loc 2 23 42
 379              		.cfi_startproc
 380              		@ args = 0, pretend = 0, frame = 8
 381              		@ frame_needed = 1, uses_anonymous_args = 0
 382 0000 80B5     		push	{r7, lr}
 383              		.save {r7, lr}
 384              	.LCFI28:
 385              		.cfi_def_cfa_offset 8
 386              		.cfi_offset 7, -8
 387              		.cfi_offset 14, -4
 388              		.pad #8
 389 0002 82B0     		sub	sp, sp, #8
 390              	.LCFI29:
 391              		.cfi_def_cfa_offset 16
 392              		.setfp r7, sp, #0
 393 0004 00AF     		add	r7, sp, #0
 394              	.LCFI30:
 395              		.cfi_def_cfa_register 7
 396 0006 87ED010A 		vstr.32	s0, [r7, #4]
  24:motorlib/foc_i.cpp ****     config.fast_loop.phase_lock_mode(id);
 397              		.loc 2 24 12
 398 000a 054B     		ldr	r3, .L21
 399 000c 1B68     		ldr	r3, [r3]
 400              		.loc 2 24 37
 401 000e 97ED010A 		vldr.32	s0, [r7, #4]
 402 0012 1846     		mov	r0, r3
 403 0014 FFF7FEFF 		bl	_ZN8FastLoop15phase_lock_modeEf
  25:motorlib/foc_i.cpp **** }
 404              		.loc 2 25 1
 405 0018 00BF     		nop
 406 001a 0837     		adds	r7, r7, #8
 407              	.LCFI31:
 408              		.cfi_def_cfa_offset 8
 409 001c BD46     		mov	sp, r7
 410              	.LCFI32:
 411              		.cfi_def_cfa_register 13
 412              		@ sp needed
 413 001e 80BD     		pop	{r7, pc}
 414              	.L22:
 415              		.align	2
 416              	.L21:
 417 0020 00000000 		.word	config
 418              		.cfi_endproc
 419              	.LFE235:
 420              		.fnend
 422              		.section	.text.fast_loop_current_mode,"ax",%progbits
 423              		.align	1
 424              		.global	fast_loop_current_mode
 425              		.syntax unified
 426              		.thumb
 427              		.thumb_func
 428              		.fpu fpv4-sp-d16
 430              	fast_loop_current_mode:
ARM GAS  /tmp/ccpFMNt0.s 			page 9


 431              		.fnstart
 432              	.LFB236:
  26:motorlib/foc_i.cpp **** 
  27:motorlib/foc_i.cpp **** void fast_loop_current_mode() {
 433              		.loc 2 27 31
 434              		.cfi_startproc
 435              		@ args = 0, pretend = 0, frame = 0
 436              		@ frame_needed = 1, uses_anonymous_args = 0
 437 0000 80B5     		push	{r7, lr}
 438              		.save {r7, lr}
 439              	.LCFI33:
 440              		.cfi_def_cfa_offset 8
 441              		.cfi_offset 7, -8
 442              		.cfi_offset 14, -4
 443              		.setfp r7, sp, #0
 444 0002 00AF     		add	r7, sp, #0
 445              	.LCFI34:
 446              		.cfi_def_cfa_register 7
  28:motorlib/foc_i.cpp ****     config.fast_loop.current_mode();
 447              		.loc 2 28 12
 448 0004 034B     		ldr	r3, .L24
 449 0006 1B68     		ldr	r3, [r3]
 450              		.loc 2 28 34
 451 0008 1846     		mov	r0, r3
 452 000a FFF7FEFF 		bl	_ZN8FastLoop12current_modeEv
  29:motorlib/foc_i.cpp **** }
 453              		.loc 2 29 1
 454 000e 00BF     		nop
 455 0010 80BD     		pop	{r7, pc}
 456              	.L25:
 457 0012 00BF     		.align	2
 458              	.L24:
 459 0014 00000000 		.word	config
 460              		.cfi_endproc
 461              	.LFE236:
 462              		.fnend
 464              		.section	.text.fast_loop_brake_mode,"ax",%progbits
 465              		.align	1
 466              		.global	fast_loop_brake_mode
 467              		.syntax unified
 468              		.thumb
 469              		.thumb_func
 470              		.fpu fpv4-sp-d16
 472              	fast_loop_brake_mode:
 473              		.fnstart
 474              	.LFB237:
  30:motorlib/foc_i.cpp **** 
  31:motorlib/foc_i.cpp **** void fast_loop_brake_mode() {
 475              		.loc 2 31 29
 476              		.cfi_startproc
 477              		@ args = 0, pretend = 0, frame = 0
 478              		@ frame_needed = 1, uses_anonymous_args = 0
 479 0000 80B5     		push	{r7, lr}
 480              		.save {r7, lr}
 481              	.LCFI35:
 482              		.cfi_def_cfa_offset 8
 483              		.cfi_offset 7, -8
ARM GAS  /tmp/ccpFMNt0.s 			page 10


 484              		.cfi_offset 14, -4
 485              		.setfp r7, sp, #0
 486 0002 00AF     		add	r7, sp, #0
 487              	.LCFI36:
 488              		.cfi_def_cfa_register 7
  32:motorlib/foc_i.cpp ****     config.fast_loop.brake_mode();
 489              		.loc 2 32 12
 490 0004 034B     		ldr	r3, .L27
 491 0006 1B68     		ldr	r3, [r3]
 492              		.loc 2 32 32
 493 0008 1846     		mov	r0, r3
 494 000a FFF7FEFF 		bl	_ZN8FastLoop10brake_modeEv
  33:motorlib/foc_i.cpp **** }
 495              		.loc 2 33 1
 496 000e 00BF     		nop
 497 0010 80BD     		pop	{r7, pc}
 498              	.L28:
 499 0012 00BF     		.align	2
 500              	.L27:
 501 0014 00000000 		.word	config
 502              		.cfi_endproc
 503              	.LFE237:
 504              		.fnend
 506              		.section	.text.fast_loop_open_mode,"ax",%progbits
 507              		.align	1
 508              		.global	fast_loop_open_mode
 509              		.syntax unified
 510              		.thumb
 511              		.thumb_func
 512              		.fpu fpv4-sp-d16
 514              	fast_loop_open_mode:
 515              		.fnstart
 516              	.LFB238:
  34:motorlib/foc_i.cpp **** 
  35:motorlib/foc_i.cpp **** void fast_loop_open_mode() {
 517              		.loc 2 35 28
 518              		.cfi_startproc
 519              		@ args = 0, pretend = 0, frame = 0
 520              		@ frame_needed = 1, uses_anonymous_args = 0
 521 0000 80B5     		push	{r7, lr}
 522              		.save {r7, lr}
 523              	.LCFI37:
 524              		.cfi_def_cfa_offset 8
 525              		.cfi_offset 7, -8
 526              		.cfi_offset 14, -4
 527              		.setfp r7, sp, #0
 528 0002 00AF     		add	r7, sp, #0
 529              	.LCFI38:
 530              		.cfi_def_cfa_register 7
  36:motorlib/foc_i.cpp ****     config.fast_loop.open_mode();
 531              		.loc 2 36 12
 532 0004 034B     		ldr	r3, .L30
 533 0006 1B68     		ldr	r3, [r3]
 534              		.loc 2 36 31
 535 0008 1846     		mov	r0, r3
 536 000a FFF7FEFF 		bl	_ZN8FastLoop9open_modeEv
  37:motorlib/foc_i.cpp **** }
ARM GAS  /tmp/ccpFMNt0.s 			page 11


 537              		.loc 2 37 1
 538 000e 00BF     		nop
 539 0010 80BD     		pop	{r7, pc}
 540              	.L31:
 541 0012 00BF     		.align	2
 542              	.L30:
 543 0014 00000000 		.word	config
 544              		.cfi_endproc
 545              	.LFE238:
 546              		.fnend
 548              		.section	.text.fast_loop_set_param,"ax",%progbits
 549              		.align	1
 550              		.global	fast_loop_set_param
 551              		.syntax unified
 552              		.thumb
 553              		.thumb_func
 554              		.fpu fpv4-sp-d16
 556              	fast_loop_set_param:
 557              		.fnstart
 558              	.LFB239:
  38:motorlib/foc_i.cpp **** 
  39:motorlib/foc_i.cpp **** void fast_loop_set_param(const FastLoopParam *const fast_loop_param) {
 559              		.loc 2 39 70
 560              		.cfi_startproc
 561              		@ args = 0, pretend = 0, frame = 8
 562              		@ frame_needed = 1, uses_anonymous_args = 0
 563 0000 80B5     		push	{r7, lr}
 564              		.save {r7, lr}
 565              	.LCFI39:
 566              		.cfi_def_cfa_offset 8
 567              		.cfi_offset 7, -8
 568              		.cfi_offset 14, -4
 569              		.pad #8
 570 0002 82B0     		sub	sp, sp, #8
 571              	.LCFI40:
 572              		.cfi_def_cfa_offset 16
 573              		.setfp r7, sp, #0
 574 0004 00AF     		add	r7, sp, #0
 575              	.LCFI41:
 576              		.cfi_def_cfa_register 7
 577 0006 7860     		str	r0, [r7, #4]
  40:motorlib/foc_i.cpp ****     config.fast_loop.set_param(*fast_loop_param);
 578              		.loc 2 40 12
 579 0008 044B     		ldr	r3, .L33
 580 000a 1B68     		ldr	r3, [r3]
 581              		.loc 2 40 31
 582 000c 7968     		ldr	r1, [r7, #4]
 583 000e 1846     		mov	r0, r3
 584 0010 FFF7FEFF 		bl	_ZN8FastLoop9set_paramERK13FastLoopParam
  41:motorlib/foc_i.cpp **** }
 585              		.loc 2 41 1
 586 0014 00BF     		nop
 587 0016 0837     		adds	r7, r7, #8
 588              	.LCFI42:
 589              		.cfi_def_cfa_offset 8
 590 0018 BD46     		mov	sp, r7
 591              	.LCFI43:
ARM GAS  /tmp/ccpFMNt0.s 			page 12


 592              		.cfi_def_cfa_register 13
 593              		@ sp needed
 594 001a 80BD     		pop	{r7, pc}
 595              	.L34:
 596              		.align	2
 597              	.L33:
 598 001c 00000000 		.word	config
 599              		.cfi_endproc
 600              	.LFE239:
 601              		.fnend
 603              		.section	.text.fast_loop_get_status,"ax",%progbits
 604              		.align	1
 605              		.global	fast_loop_get_status
 606              		.syntax unified
 607              		.thumb
 608              		.thumb_func
 609              		.fpu fpv4-sp-d16
 611              	fast_loop_get_status:
 612              		.fnstart
 613              	.LFB240:
  42:motorlib/foc_i.cpp **** 
  43:motorlib/foc_i.cpp **** void fast_loop_get_status(FastLoopStatus * const fast_loop_status) {
 614              		.loc 2 43 68
 615              		.cfi_startproc
 616              		@ args = 0, pretend = 0, frame = 8
 617              		@ frame_needed = 1, uses_anonymous_args = 0
 618 0000 80B5     		push	{r7, lr}
 619              		.save {r7, lr}
 620              	.LCFI44:
 621              		.cfi_def_cfa_offset 8
 622              		.cfi_offset 7, -8
 623              		.cfi_offset 14, -4
 624              		.pad #8
 625 0002 82B0     		sub	sp, sp, #8
 626              	.LCFI45:
 627              		.cfi_def_cfa_offset 16
 628              		.setfp r7, sp, #0
 629 0004 00AF     		add	r7, sp, #0
 630              	.LCFI46:
 631              		.cfi_def_cfa_register 7
 632 0006 7860     		str	r0, [r7, #4]
  44:motorlib/foc_i.cpp ****     config.fast_loop.get_status(fast_loop_status);
 633              		.loc 2 44 12
 634 0008 044B     		ldr	r3, .L36
 635 000a 1B68     		ldr	r3, [r3]
 636              		.loc 2 44 32
 637 000c 7968     		ldr	r1, [r7, #4]
 638 000e 1846     		mov	r0, r3
 639 0010 FFF7FEFF 		bl	_ZN8FastLoop10get_statusEP14FastLoopStatus
  45:motorlib/foc_i.cpp **** }
 640              		.loc 2 45 1
 641 0014 00BF     		nop
 642 0016 0837     		adds	r7, r7, #8
 643              	.LCFI47:
 644              		.cfi_def_cfa_offset 8
 645 0018 BD46     		mov	sp, r7
 646              	.LCFI48:
ARM GAS  /tmp/ccpFMNt0.s 			page 13


 647              		.cfi_def_cfa_register 13
 648              		@ sp needed
 649 001a 80BD     		pop	{r7, pc}
 650              	.L37:
 651              		.align	2
 652              	.L36:
 653 001c 00000000 		.word	config
 654              		.cfi_endproc
 655              	.LFE240:
 656              		.fnend
 658              		.section	.text.fast_loop_voltage_mode,"ax",%progbits
 659              		.align	1
 660              		.global	fast_loop_voltage_mode
 661              		.syntax unified
 662              		.thumb
 663              		.thumb_func
 664              		.fpu fpv4-sp-d16
 666              	fast_loop_voltage_mode:
 667              		.fnstart
 668              	.LFB241:
  46:motorlib/foc_i.cpp **** 
  47:motorlib/foc_i.cpp **** void fast_loop_voltage_mode() {
 669              		.loc 2 47 31
 670              		.cfi_startproc
 671              		@ args = 0, pretend = 0, frame = 0
 672              		@ frame_needed = 1, uses_anonymous_args = 0
 673 0000 80B5     		push	{r7, lr}
 674              		.save {r7, lr}
 675              	.LCFI49:
 676              		.cfi_def_cfa_offset 8
 677              		.cfi_offset 7, -8
 678              		.cfi_offset 14, -4
 679              		.setfp r7, sp, #0
 680 0002 00AF     		add	r7, sp, #0
 681              	.LCFI50:
 682              		.cfi_def_cfa_register 7
  48:motorlib/foc_i.cpp ****     config.fast_loop.voltage_mode();
 683              		.loc 2 48 12
 684 0004 034B     		ldr	r3, .L39
 685 0006 1B68     		ldr	r3, [r3]
 686              		.loc 2 48 34
 687 0008 1846     		mov	r0, r3
 688 000a FFF7FEFF 		bl	_ZN8FastLoop12voltage_modeEv
  49:motorlib/foc_i.cpp **** }
 689              		.loc 2 49 1
 690 000e 00BF     		nop
 691 0010 80BD     		pop	{r7, pc}
 692              	.L40:
 693 0012 00BF     		.align	2
 694              	.L39:
 695 0014 00000000 		.word	config
 696              		.cfi_endproc
 697              	.LFE241:
 698              		.fnend
 700              		.section	.text.fast_loop_zero_current_sensors,"ax",%progbits
 701              		.align	1
 702              		.global	fast_loop_zero_current_sensors
ARM GAS  /tmp/ccpFMNt0.s 			page 14


 703              		.syntax unified
 704              		.thumb
 705              		.thumb_func
 706              		.fpu fpv4-sp-d16
 708              	fast_loop_zero_current_sensors:
 709              		.fnstart
 710              	.LFB242:
  50:motorlib/foc_i.cpp **** 
  51:motorlib/foc_i.cpp **** void fast_loop_zero_current_sensors() {
 711              		.loc 2 51 39
 712              		.cfi_startproc
 713              		@ args = 0, pretend = 0, frame = 0
 714              		@ frame_needed = 1, uses_anonymous_args = 0
 715 0000 80B5     		push	{r7, lr}
 716              		.save {r7, lr}
 717              	.LCFI51:
 718              		.cfi_def_cfa_offset 8
 719              		.cfi_offset 7, -8
 720              		.cfi_offset 14, -4
 721              		.setfp r7, sp, #0
 722 0002 00AF     		add	r7, sp, #0
 723              	.LCFI52:
 724              		.cfi_def_cfa_register 7
  52:motorlib/foc_i.cpp ****     config.fast_loop.zero_current_sensors();
 725              		.loc 2 52 12
 726 0004 034B     		ldr	r3, .L42
 727 0006 1B68     		ldr	r3, [r3]
 728              		.loc 2 52 42
 729 0008 1846     		mov	r0, r3
 730 000a FFF7FEFF 		bl	_ZN8FastLoop20zero_current_sensorsEv
  53:motorlib/foc_i.cpp **** }
 731              		.loc 2 53 1
 732 000e 00BF     		nop
 733 0010 80BD     		pop	{r7, pc}
 734              	.L43:
 735 0012 00BF     		.align	2
 736              	.L42:
 737 0014 00000000 		.word	config
 738              		.cfi_endproc
 739              	.LFE242:
 740              		.fnend
 742              		.section	.text.main_loop_update,"ax",%progbits
 743              		.align	1
 744              		.global	main_loop_update
 745              		.syntax unified
 746              		.thumb
 747              		.thumb_func
 748              		.fpu fpv4-sp-d16
 750              	main_loop_update:
 751              		.fnstart
 752              	.LFB243:
  54:motorlib/foc_i.cpp **** 
  55:motorlib/foc_i.cpp **** 
  56:motorlib/foc_i.cpp **** void main_loop_update() {
 753              		.loc 2 56 25
 754              		.cfi_startproc
 755              		@ args = 0, pretend = 0, frame = 0
ARM GAS  /tmp/ccpFMNt0.s 			page 15


 756              		@ frame_needed = 1, uses_anonymous_args = 0
 757 0000 80B5     		push	{r7, lr}
 758              		.save {r7, lr}
 759              	.LCFI53:
 760              		.cfi_def_cfa_offset 8
 761              		.cfi_offset 7, -8
 762              		.cfi_offset 14, -4
 763              		.setfp r7, sp, #0
 764 0002 00AF     		add	r7, sp, #0
 765              	.LCFI54:
 766              		.cfi_def_cfa_register 7
  57:motorlib/foc_i.cpp ****     config.main_loop.update();
 767              		.loc 2 57 12
 768 0004 034B     		ldr	r3, .L45
 769 0006 5B68     		ldr	r3, [r3, #4]
 770              		.loc 2 57 28
 771 0008 1846     		mov	r0, r3
 772 000a FFF7FEFF 		bl	_ZN8MainLoop6updateEv
  58:motorlib/foc_i.cpp **** }
 773              		.loc 2 58 1
 774 000e 00BF     		nop
 775 0010 80BD     		pop	{r7, pc}
 776              	.L46:
 777 0012 00BF     		.align	2
 778              	.L45:
 779 0014 00000000 		.word	config
 780              		.cfi_endproc
 781              	.LFE243:
 782              		.fnend
 784              		.section	.text.main_loop_set_param,"ax",%progbits
 785              		.align	1
 786              		.global	main_loop_set_param
 787              		.syntax unified
 788              		.thumb
 789              		.thumb_func
 790              		.fpu fpv4-sp-d16
 792              	main_loop_set_param:
 793              		.fnstart
 794              	.LFB244:
  59:motorlib/foc_i.cpp **** 
  60:motorlib/foc_i.cpp **** void main_loop_set_param(MainLoopParam * const main_loop_param) {
 795              		.loc 2 60 65
 796              		.cfi_startproc
 797              		@ args = 0, pretend = 0, frame = 8
 798              		@ frame_needed = 1, uses_anonymous_args = 0
 799 0000 80B5     		push	{r7, lr}
 800              		.save {r7, lr}
 801              	.LCFI55:
 802              		.cfi_def_cfa_offset 8
 803              		.cfi_offset 7, -8
 804              		.cfi_offset 14, -4
 805              		.pad #8
 806 0002 82B0     		sub	sp, sp, #8
 807              	.LCFI56:
 808              		.cfi_def_cfa_offset 16
 809              		.setfp r7, sp, #0
 810 0004 00AF     		add	r7, sp, #0
ARM GAS  /tmp/ccpFMNt0.s 			page 16


 811              	.LCFI57:
 812              		.cfi_def_cfa_register 7
 813 0006 7860     		str	r0, [r7, #4]
  61:motorlib/foc_i.cpp ****     config.main_loop.set_param(*main_loop_param);
 814              		.loc 2 61 12
 815 0008 044B     		ldr	r3, .L48
 816 000a 5B68     		ldr	r3, [r3, #4]
 817              		.loc 2 61 31
 818 000c 7968     		ldr	r1, [r7, #4]
 819 000e 1846     		mov	r0, r3
 820 0010 FFF7FEFF 		bl	_ZN8MainLoop9set_paramER13MainLoopParam
  62:motorlib/foc_i.cpp **** }
 821              		.loc 2 62 1
 822 0014 00BF     		nop
 823 0016 0837     		adds	r7, r7, #8
 824              	.LCFI58:
 825              		.cfi_def_cfa_offset 8
 826 0018 BD46     		mov	sp, r7
 827              	.LCFI59:
 828              		.cfi_def_cfa_register 13
 829              		@ sp needed
 830 001a 80BD     		pop	{r7, pc}
 831              	.L49:
 832              		.align	2
 833              	.L48:
 834 001c 00000000 		.word	config
 835              		.cfi_endproc
 836              	.LFE244:
 837              		.fnend
 839              		.section	.text.main_loop_get_status,"ax",%progbits
 840              		.align	1
 841              		.global	main_loop_get_status
 842              		.syntax unified
 843              		.thumb
 844              		.thumb_func
 845              		.fpu fpv4-sp-d16
 847              	main_loop_get_status:
 848              		.fnstart
 849              	.LFB245:
  63:motorlib/foc_i.cpp **** 
  64:motorlib/foc_i.cpp **** void main_loop_get_status(MainLoopStatus * const main_loop_status) {
 850              		.loc 2 64 68
 851              		.cfi_startproc
 852              		@ args = 0, pretend = 0, frame = 8
 853              		@ frame_needed = 1, uses_anonymous_args = 0
 854 0000 80B5     		push	{r7, lr}
 855              		.save {r7, lr}
 856              	.LCFI60:
 857              		.cfi_def_cfa_offset 8
 858              		.cfi_offset 7, -8
 859              		.cfi_offset 14, -4
 860              		.pad #8
 861 0002 82B0     		sub	sp, sp, #8
 862              	.LCFI61:
 863              		.cfi_def_cfa_offset 16
 864              		.setfp r7, sp, #0
 865 0004 00AF     		add	r7, sp, #0
ARM GAS  /tmp/ccpFMNt0.s 			page 17


 866              	.LCFI62:
 867              		.cfi_def_cfa_register 7
 868 0006 7860     		str	r0, [r7, #4]
  65:motorlib/foc_i.cpp ****     config.main_loop.get_status(main_loop_status);
 869              		.loc 2 65 12
 870 0008 044B     		ldr	r3, .L51
 871 000a 5B68     		ldr	r3, [r3, #4]
 872              		.loc 2 65 32
 873 000c 7968     		ldr	r1, [r7, #4]
 874 000e 1846     		mov	r0, r3
 875 0010 FFF7FEFF 		bl	_ZNK8MainLoop10get_statusEP14MainLoopStatus
  66:motorlib/foc_i.cpp **** }
 876              		.loc 2 66 1
 877 0014 00BF     		nop
 878 0016 0837     		adds	r7, r7, #8
 879              	.LCFI63:
 880              		.cfi_def_cfa_offset 8
 881 0018 BD46     		mov	sp, r7
 882              	.LCFI64:
 883              		.cfi_def_cfa_register 13
 884              		@ sp needed
 885 001a 80BD     		pop	{r7, pc}
 886              	.L52:
 887              		.align	2
 888              	.L51:
 889 001c 00000000 		.word	config
 890              		.cfi_endproc
 891              	.LFE245:
 892              		.fnend
 894              		.text
 895              	.Letext0:
 896              		.file 3 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 897              		.file 4 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 898              		.file 5 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdint.h"
 899              		.file 6 "motorlib/messages.h"
 900              		.file 7 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdint"
 901              		.file 8 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cmath"
 902              		.file 9 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thu
 903              		.file 10 "/opt/gcc-arm-none-eabi-8-2019-q3-update/lib/gcc/arm-none-eabi/8.3.1/include/stddef.h"
 904              		.file 11 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_types.h"
 905              		.file 12 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/reent.h"
 906              		.file 13 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/lock.h"
 907              		.file 14 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/math.h"
 908              		.file 15 "motorlib/main_loop.h"
 909              		.file 16 "motorlib/config.h"
 910              		.file 17 "<built-in>"
ARM GAS  /tmp/ccpFMNt0.s 			page 18


DEFINED SYMBOLS
                            *ABS*:0000000000000000 foc_i.cpp
     /tmp/ccpFMNt0.s:18     .text._ZN8FastLoop10set_id_desEf:0000000000000000 $t
     /tmp/ccpFMNt0.s:26     .text._ZN8FastLoop10set_id_desEf:0000000000000000 _ZN8FastLoop10set_id_desEf
.ARM.exidx.text._ZN8FastLoop10set_id_desEf:0000000000000000 $d
     /tmp/ccpFMNt0.s:73     .text._ZN8FastLoop10set_iq_desEf:0000000000000000 $t
     /tmp/ccpFMNt0.s:80     .text._ZN8FastLoop10set_iq_desEf:0000000000000000 _ZN8FastLoop10set_iq_desEf
.ARM.exidx.text._ZN8FastLoop10set_iq_desEf:0000000000000000 $d
     /tmp/ccpFMNt0.s:135    .text.system_init:0000000000000000 $t
     /tmp/ccpFMNt0.s:142    .text.system_init:0000000000000000 system_init
     /tmp/ccpFMNt0.s:172    .text.system_init:0000000000000014 $d
      .ARM.exidx.text.system_init:0000000000000000 $d
     /tmp/ccpFMNt0.s:178    .text.fast_loop_update:0000000000000000 $t
     /tmp/ccpFMNt0.s:185    .text.fast_loop_update:0000000000000000 fast_loop_update
     /tmp/ccpFMNt0.s:214    .text.fast_loop_update:0000000000000014 $d
 .ARM.exidx.text.fast_loop_update:0000000000000000 $d
     /tmp/ccpFMNt0.s:220    .text.fast_loop_maintenance:0000000000000000 $t
     /tmp/ccpFMNt0.s:227    .text.fast_loop_maintenance:0000000000000000 fast_loop_maintenance
     /tmp/ccpFMNt0.s:256    .text.fast_loop_maintenance:0000000000000014 $d
.ARM.exidx.text.fast_loop_maintenance:0000000000000000 $d
     /tmp/ccpFMNt0.s:262    .text.fast_loop_set_id_des:0000000000000000 $t
     /tmp/ccpFMNt0.s:269    .text.fast_loop_set_id_des:0000000000000000 fast_loop_set_id_des
     /tmp/ccpFMNt0.s:308    .text.fast_loop_set_id_des:0000000000000020 $d
.ARM.exidx.text.fast_loop_set_id_des:0000000000000000 $d
     /tmp/ccpFMNt0.s:315    .text.fast_loop_set_iq_des:0000000000000000 $t
     /tmp/ccpFMNt0.s:322    .text.fast_loop_set_iq_des:0000000000000000 fast_loop_set_iq_des
     /tmp/ccpFMNt0.s:361    .text.fast_loop_set_iq_des:0000000000000020 $d
.ARM.exidx.text.fast_loop_set_iq_des:0000000000000000 $d
     /tmp/ccpFMNt0.s:368    .text.fast_loop_phase_lock_mode:0000000000000000 $t
     /tmp/ccpFMNt0.s:375    .text.fast_loop_phase_lock_mode:0000000000000000 fast_loop_phase_lock_mode
     /tmp/ccpFMNt0.s:417    .text.fast_loop_phase_lock_mode:0000000000000020 $d
.ARM.extab.text.fast_loop_phase_lock_mode:0000000000000000 $d
.ARM.exidx.text.fast_loop_phase_lock_mode:0000000000000000 $d
     /tmp/ccpFMNt0.s:423    .text.fast_loop_current_mode:0000000000000000 $t
     /tmp/ccpFMNt0.s:430    .text.fast_loop_current_mode:0000000000000000 fast_loop_current_mode
     /tmp/ccpFMNt0.s:459    .text.fast_loop_current_mode:0000000000000014 $d
.ARM.exidx.text.fast_loop_current_mode:0000000000000000 $d
     /tmp/ccpFMNt0.s:465    .text.fast_loop_brake_mode:0000000000000000 $t
     /tmp/ccpFMNt0.s:472    .text.fast_loop_brake_mode:0000000000000000 fast_loop_brake_mode
     /tmp/ccpFMNt0.s:501    .text.fast_loop_brake_mode:0000000000000014 $d
.ARM.exidx.text.fast_loop_brake_mode:0000000000000000 $d
     /tmp/ccpFMNt0.s:507    .text.fast_loop_open_mode:0000000000000000 $t
     /tmp/ccpFMNt0.s:514    .text.fast_loop_open_mode:0000000000000000 fast_loop_open_mode
     /tmp/ccpFMNt0.s:543    .text.fast_loop_open_mode:0000000000000014 $d
.ARM.exidx.text.fast_loop_open_mode:0000000000000000 $d
     /tmp/ccpFMNt0.s:549    .text.fast_loop_set_param:0000000000000000 $t
     /tmp/ccpFMNt0.s:556    .text.fast_loop_set_param:0000000000000000 fast_loop_set_param
     /tmp/ccpFMNt0.s:598    .text.fast_loop_set_param:000000000000001c $d
.ARM.extab.text.fast_loop_set_param:0000000000000000 $d
.ARM.exidx.text.fast_loop_set_param:0000000000000000 $d
     /tmp/ccpFMNt0.s:604    .text.fast_loop_get_status:0000000000000000 $t
     /tmp/ccpFMNt0.s:611    .text.fast_loop_get_status:0000000000000000 fast_loop_get_status
     /tmp/ccpFMNt0.s:653    .text.fast_loop_get_status:000000000000001c $d
.ARM.extab.text.fast_loop_get_status:0000000000000000 $d
.ARM.exidx.text.fast_loop_get_status:0000000000000000 $d
     /tmp/ccpFMNt0.s:659    .text.fast_loop_voltage_mode:0000000000000000 $t
     /tmp/ccpFMNt0.s:666    .text.fast_loop_voltage_mode:0000000000000000 fast_loop_voltage_mode
ARM GAS  /tmp/ccpFMNt0.s 			page 19


     /tmp/ccpFMNt0.s:695    .text.fast_loop_voltage_mode:0000000000000014 $d
.ARM.exidx.text.fast_loop_voltage_mode:0000000000000000 $d
     /tmp/ccpFMNt0.s:701    .text.fast_loop_zero_current_sensors:0000000000000000 $t
     /tmp/ccpFMNt0.s:708    .text.fast_loop_zero_current_sensors:0000000000000000 fast_loop_zero_current_sensors
     /tmp/ccpFMNt0.s:737    .text.fast_loop_zero_current_sensors:0000000000000014 $d
.ARM.exidx.text.fast_loop_zero_current_sensors:0000000000000000 $d
     /tmp/ccpFMNt0.s:743    .text.main_loop_update:0000000000000000 $t
     /tmp/ccpFMNt0.s:750    .text.main_loop_update:0000000000000000 main_loop_update
     /tmp/ccpFMNt0.s:779    .text.main_loop_update:0000000000000014 $d
 .ARM.exidx.text.main_loop_update:0000000000000000 $d
     /tmp/ccpFMNt0.s:785    .text.main_loop_set_param:0000000000000000 $t
     /tmp/ccpFMNt0.s:792    .text.main_loop_set_param:0000000000000000 main_loop_set_param
     /tmp/ccpFMNt0.s:834    .text.main_loop_set_param:000000000000001c $d
.ARM.extab.text.main_loop_set_param:0000000000000000 $d
.ARM.exidx.text.main_loop_set_param:0000000000000000 $d
     /tmp/ccpFMNt0.s:840    .text.main_loop_get_status:0000000000000000 $t
     /tmp/ccpFMNt0.s:847    .text.main_loop_get_status:0000000000000000 main_loop_get_status
     /tmp/ccpFMNt0.s:889    .text.main_loop_get_status:000000000000001c $d
.ARM.extab.text.main_loop_get_status:0000000000000000 $d
.ARM.exidx.text.main_loop_get_status:0000000000000000 $d

UNDEFINED SYMBOLS
_ZN8MainLoop4initEv
config
__aeabi_unwind_cpp_pr0
_ZN8FastLoop6updateEv
_ZN8FastLoop11maintenanceEv
_ZN8FastLoop15phase_lock_modeEf
__aeabi_unwind_cpp_pr1
_ZN8FastLoop12current_modeEv
_ZN8FastLoop10brake_modeEv
_ZN8FastLoop9open_modeEv
_ZN8FastLoop9set_paramERK13FastLoopParam
_ZN8FastLoop10get_statusEP14FastLoopStatus
_ZN8FastLoop12voltage_modeEv
_ZN8FastLoop20zero_current_sensorsEv
_ZN8MainLoop6updateEv
_ZN8MainLoop9set_paramER13MainLoopParam
_ZNK8MainLoop10get_statusEP14MainLoopStatus
