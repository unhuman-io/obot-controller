ARM GAS  /tmp/cc4qm6LU.s 			page 1


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
  13              		.file	"control_fun.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.ccmram,"ax",%progbits
  18              		.align	1
  19              		.global	_ZN12PIController4stepEff
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN12PIController4stepEff:
  27              		.fnstart
  28              	.LFB233:
  29              		.file 1 "motorlib/control_fun.cpp"
   1:motorlib/control_fun.cpp **** #include "control_fun.h"
   2:motorlib/control_fun.cpp **** //#include "hal_fun.h"
   3:motorlib/control_fun.cpp **** #include <cmath>
   4:motorlib/control_fun.cpp **** 
   5:motorlib/control_fun.cpp **** float fsat(float a, float sat) {
   6:motorlib/control_fun.cpp ****     // Slow version
   7:motorlib/control_fun.cpp ****     // if (a > sat) {
   8:motorlib/control_fun.cpp ****     //     return sat;
   9:motorlib/control_fun.cpp ****     // } else if (a < -sat) {
  10:motorlib/control_fun.cpp ****     //     return -sat;
  11:motorlib/control_fun.cpp ****     // } else {
  12:motorlib/control_fun.cpp ****     //     return a;
  13:motorlib/control_fun.cpp ****     // }
  14:motorlib/control_fun.cpp **** 
  15:motorlib/control_fun.cpp ****     // Doesn't completely optimize to branchless instructions
  16:motorlib/control_fun.cpp ****     // return a>sat ? sat : (a<-sat ? -sat : a);
  17:motorlib/control_fun.cpp **** 
  18:motorlib/control_fun.cpp ****     // Optimizes to branchless
  19:motorlib/control_fun.cpp ****     float b = a>sat ? sat : a;
  20:motorlib/control_fun.cpp ****     b = b<-sat ? -sat : b;
  21:motorlib/control_fun.cpp ****     return b;
  22:motorlib/control_fun.cpp **** 
  23:motorlib/control_fun.cpp ****     // Not so great
  24:motorlib/control_fun.cpp ****     // float s = a>0 ? sat : -sat;
  25:motorlib/control_fun.cpp ****     // return std::abs(a)-sat>0 ? s : a;
  26:motorlib/control_fun.cpp **** 
  27:motorlib/control_fun.cpp ****     // Assembly
  28:motorlib/control_fun.cpp ****     // asm("vcmpe.f32 %[a], %[sat]\n\t"
  29:motorlib/control_fun.cpp ****     //     "vmrs APSR_nzcv, fpscr\n\t"
ARM GAS  /tmp/cc4qm6LU.s 			page 2


  30:motorlib/control_fun.cpp ****     //     "it gt\n\t"
  31:motorlib/control_fun.cpp ****     //     "vmovgt.f32 %[a], %[sat]\n\t"
  32:motorlib/control_fun.cpp ****     //     "vneg.f32 %[sat], %[sat]\n\t"
  33:motorlib/control_fun.cpp ****     //     "vcmpe.f32 %[a], %[sat]\n\t"
  34:motorlib/control_fun.cpp ****     //     "vmrs APSR_nzcv, fpscr\n\t"
  35:motorlib/control_fun.cpp ****     //     "it lt\n\t"
  36:motorlib/control_fun.cpp ****     //     "vmovlt.f32 %[a], %[sat]\n\t"
  37:motorlib/control_fun.cpp ****     //     :
  38:motorlib/control_fun.cpp ****     //     : [a] "t" (a), [sat] "t" (sat));
  39:motorlib/control_fun.cpp ****     // return a;
  40:motorlib/control_fun.cpp **** }
  41:motorlib/control_fun.cpp **** 
  42:motorlib/control_fun.cpp **** void PIController::set_param(const PIParam &pi_param) {
  43:motorlib/control_fun.cpp ****     ki_ = pi_param.ki;
  44:motorlib/control_fun.cpp ****     kp_ = pi_param.kp;
  45:motorlib/control_fun.cpp ****     ki_limit_ = pi_param.ki_limit;
  46:motorlib/control_fun.cpp ****     command_max_ = pi_param.command_max;
  47:motorlib/control_fun.cpp **** }
  48:motorlib/control_fun.cpp **** 
  49:motorlib/control_fun.cpp **** float PIController::step(float desired, float measured) {
  30              		.loc 1 49 57
  31              		.cfi_startproc
  32              		@ args = 0, pretend = 0, frame = 24
  33              		@ frame_needed = 1, uses_anonymous_args = 0
  34 0000 80B5     		push	{r7, lr}
  35              	.LCFI0:
  36              		.cfi_def_cfa_offset 8
  37              		.cfi_offset 7, -8
  38              		.cfi_offset 14, -4
  39 0002 86B0     		sub	sp, sp, #24
  40              	.LCFI1:
  41              		.cfi_def_cfa_offset 32
  42 0004 00AF     		add	r7, sp, #0
  43              	.LCFI2:
  44              		.cfi_def_cfa_register 7
  45 0006 F860     		str	r0, [r7, #12]
  46 0008 87ED020A 		vstr.32	s0, [r7, #8]
  47 000c C7ED010A 		vstr.32	s1, [r7, #4]
  50:motorlib/control_fun.cpp ****     float error = desired - measured;
  48              		.loc 1 50 11
  49 0010 97ED027A 		vldr.32	s14, [r7, #8]
  50 0014 D7ED017A 		vldr.32	s15, [r7, #4]
  51 0018 77EE677A 		vsub.f32	s15, s14, s15
  52 001c C7ED057A 		vstr.32	s15, [r7, #20]
  51:motorlib/control_fun.cpp ****     ki_sum_ += ki_ * error;
  53              		.loc 1 51 13
  54 0020 FB68     		ldr	r3, [r7, #12]
  55 0022 93ED027A 		vldr.32	s14, [r3, #8]
  56              		.loc 1 51 16
  57 0026 FB68     		ldr	r3, [r7, #12]
  58 0028 D3ED016A 		vldr.32	s13, [r3, #4]
  59              		.loc 1 51 20
  60 002c D7ED057A 		vldr.32	s15, [r7, #20]
  61 0030 66EEA77A 		vmul.f32	s15, s13, s15
  62              		.loc 1 51 13
  63 0034 77EE277A 		vadd.f32	s15, s14, s15
  64 0038 FB68     		ldr	r3, [r7, #12]
ARM GAS  /tmp/cc4qm6LU.s 			page 3


  65 003a C3ED027A 		vstr.32	s15, [r3, #8]
  52:motorlib/control_fun.cpp ****     ki_sum_ = fsat(ki_sum_, ki_limit_);
  66              		.loc 1 52 19
  67 003e FB68     		ldr	r3, [r7, #12]
  68 0040 D3ED027A 		vldr.32	s15, [r3, #8]
  69 0044 FB68     		ldr	r3, [r7, #12]
  70 0046 93ED037A 		vldr.32	s14, [r3, #12]
  71 004a F0EE470A 		vmov.f32	s1, s14
  72 004e B0EE670A 		vmov.f32	s0, s15
  73 0052 FFF7FEFF 		bl	_Z4fsatff
  74 0056 F0EE407A 		vmov.f32	s15, s0
  75              		.loc 1 52 13
  76 005a FB68     		ldr	r3, [r7, #12]
  77 005c C3ED027A 		vstr.32	s15, [r3, #8]
  53:motorlib/control_fun.cpp ****     return fsat(kp_*error + ki_sum_, command_max_);
  78              		.loc 1 53 17
  79 0060 FB68     		ldr	r3, [r7, #12]
  80 0062 93ED007A 		vldr.32	s14, [r3]
  81              		.loc 1 53 20
  82 0066 D7ED057A 		vldr.32	s15, [r7, #20]
  83 006a 27EE277A 		vmul.f32	s14, s14, s15
  84              		.loc 1 53 29
  85 006e FB68     		ldr	r3, [r7, #12]
  86 0070 D3ED027A 		vldr.32	s15, [r3, #8]
  87              		.loc 1 53 16
  88 0074 77EE277A 		vadd.f32	s15, s14, s15
  89 0078 FB68     		ldr	r3, [r7, #12]
  90 007a 93ED047A 		vldr.32	s14, [r3, #16]
  91 007e F0EE470A 		vmov.f32	s1, s14
  92 0082 B0EE670A 		vmov.f32	s0, s15
  93 0086 FFF7FEFF 		bl	_Z4fsatff
  94 008a F0EE407A 		vmov.f32	s15, s0
  54:motorlib/control_fun.cpp **** }
  95              		.loc 1 54 1
  96 008e B0EE670A 		vmov.f32	s0, s15
  97 0092 1837     		adds	r7, r7, #24
  98              	.LCFI3:
  99              		.cfi_def_cfa_offset 8
 100 0094 BD46     		mov	sp, r7
 101              	.LCFI4:
 102              		.cfi_def_cfa_register 13
 103              		@ sp needed
 104 0096 80BD     		pop	{r7, pc}
 105              		.cfi_endproc
 106              	.LFE233:
 107              		.cantunwind
 108              		.fnend
 110              		.align	1
 111              		.global	_ZN13PIDController4stepEff
 112              		.syntax unified
 113              		.thumb
 114              		.thumb_func
 115              		.fpu fpv4-sp-d16
 117              	_ZN13PIDController4stepEff:
 118              		.fnstart
 119              	.LFB235:
  55:motorlib/control_fun.cpp **** 
ARM GAS  /tmp/cc4qm6LU.s 			page 4


  56:motorlib/control_fun.cpp **** void PIDController::set_param(const PIDParam &param) {
  57:motorlib/control_fun.cpp ****     ki_ = param.ki;
  58:motorlib/control_fun.cpp ****     kp_ = param.kp;
  59:motorlib/control_fun.cpp ****     ki_limit_ = param.ki_limit;
  60:motorlib/control_fun.cpp ****     kd_ = param.kd;
  61:motorlib/control_fun.cpp ****     command_max_ = param.command_max;
  62:motorlib/control_fun.cpp **** }
  63:motorlib/control_fun.cpp **** 
  64:motorlib/control_fun.cpp **** float PIDController::step(float desired, float measured) {
 120              		.loc 1 64 58
 121              		.cfi_startproc
 122              		@ args = 0, pretend = 0, frame = 24
 123              		@ frame_needed = 1, uses_anonymous_args = 0
 124 0098 80B5     		push	{r7, lr}
 125              	.LCFI5:
 126              		.cfi_def_cfa_offset 8
 127              		.cfi_offset 7, -8
 128              		.cfi_offset 14, -4
 129 009a 86B0     		sub	sp, sp, #24
 130              	.LCFI6:
 131              		.cfi_def_cfa_offset 32
 132 009c 00AF     		add	r7, sp, #0
 133              	.LCFI7:
 134              		.cfi_def_cfa_register 7
 135 009e F860     		str	r0, [r7, #12]
 136 00a0 87ED020A 		vstr.32	s0, [r7, #8]
 137 00a4 C7ED010A 		vstr.32	s1, [r7, #4]
  65:motorlib/control_fun.cpp ****     float error = desired - measured;
 138              		.loc 1 65 11
 139 00a8 97ED027A 		vldr.32	s14, [r7, #8]
 140 00ac D7ED017A 		vldr.32	s15, [r7, #4]
 141 00b0 77EE677A 		vsub.f32	s15, s14, s15
 142 00b4 C7ED057A 		vstr.32	s15, [r7, #20]
  66:motorlib/control_fun.cpp ****     float error_dot = error-error_last_;
 143              		.loc 1 66 29
 144 00b8 FB68     		ldr	r3, [r7, #12]
 145 00ba D3ED077A 		vldr.32	s15, [r3, #28]
 146              		.loc 1 66 11
 147 00be 97ED057A 		vldr.32	s14, [r7, #20]
 148 00c2 77EE677A 		vsub.f32	s15, s14, s15
 149 00c6 C7ED047A 		vstr.32	s15, [r7, #16]
  67:motorlib/control_fun.cpp ****     error_last_ = error;
 150              		.loc 1 67 17
 151 00ca FB68     		ldr	r3, [r7, #12]
 152 00cc 7A69     		ldr	r2, [r7, #20]	@ float
 153 00ce DA61     		str	r2, [r3, #28]	@ float
  68:motorlib/control_fun.cpp ****     ki_sum_ += ki_ * error;
 154              		.loc 1 68 13
 155 00d0 FB68     		ldr	r3, [r7, #12]
 156 00d2 93ED047A 		vldr.32	s14, [r3, #16]
 157              		.loc 1 68 16
 158 00d6 FB68     		ldr	r3, [r7, #12]
 159 00d8 D3ED036A 		vldr.32	s13, [r3, #12]
 160              		.loc 1 68 20
 161 00dc D7ED057A 		vldr.32	s15, [r7, #20]
 162 00e0 66EEA77A 		vmul.f32	s15, s13, s15
 163              		.loc 1 68 13
ARM GAS  /tmp/cc4qm6LU.s 			page 5


 164 00e4 77EE277A 		vadd.f32	s15, s14, s15
 165 00e8 FB68     		ldr	r3, [r7, #12]
 166 00ea C3ED047A 		vstr.32	s15, [r3, #16]
  69:motorlib/control_fun.cpp ****     ki_sum_ = fsat(ki_sum_, ki_limit_);
 167              		.loc 1 69 19
 168 00ee FB68     		ldr	r3, [r7, #12]
 169 00f0 D3ED047A 		vldr.32	s15, [r3, #16]
 170 00f4 FB68     		ldr	r3, [r7, #12]
 171 00f6 93ED057A 		vldr.32	s14, [r3, #20]
 172 00fa F0EE470A 		vmov.f32	s1, s14
 173 00fe B0EE670A 		vmov.f32	s0, s15
 174 0102 FFF7FEFF 		bl	_Z4fsatff
 175 0106 F0EE407A 		vmov.f32	s15, s0
 176              		.loc 1 69 13
 177 010a FB68     		ldr	r3, [r7, #12]
 178 010c C3ED047A 		vstr.32	s15, [r3, #16]
  70:motorlib/control_fun.cpp ****     return fsat(kp_*error + ki_sum_ + kd_*error_dot, command_max_);
 179              		.loc 1 70 17
 180 0110 FB68     		ldr	r3, [r7, #12]
 181 0112 93ED017A 		vldr.32	s14, [r3, #4]
 182              		.loc 1 70 20
 183 0116 D7ED057A 		vldr.32	s15, [r7, #20]
 184 011a 27EE277A 		vmul.f32	s14, s14, s15
 185              		.loc 1 70 29
 186 011e FB68     		ldr	r3, [r7, #12]
 187 0120 D3ED047A 		vldr.32	s15, [r3, #16]
 188              		.loc 1 70 27
 189 0124 37EE277A 		vadd.f32	s14, s14, s15
 190              		.loc 1 70 39
 191 0128 FB68     		ldr	r3, [r7, #12]
 192 012a D3ED026A 		vldr.32	s13, [r3, #8]
 193              		.loc 1 70 42
 194 012e D7ED047A 		vldr.32	s15, [r7, #16]
 195 0132 66EEA77A 		vmul.f32	s15, s13, s15
 196              		.loc 1 70 16
 197 0136 77EE277A 		vadd.f32	s15, s14, s15
 198 013a FB68     		ldr	r3, [r7, #12]
 199 013c 93ED067A 		vldr.32	s14, [r3, #24]
 200 0140 F0EE470A 		vmov.f32	s1, s14
 201 0144 B0EE670A 		vmov.f32	s0, s15
 202 0148 FFF7FEFF 		bl	_Z4fsatff
 203 014c F0EE407A 		vmov.f32	s15, s0
  71:motorlib/control_fun.cpp **** }...
 204              		.loc 1 71 1
 205 0150 B0EE670A 		vmov.f32	s0, s15
 206 0154 1837     		adds	r7, r7, #24
 207              	.LCFI8:
 208              		.cfi_def_cfa_offset 8
 209 0156 BD46     		mov	sp, r7
 210              	.LCFI9:
 211              		.cfi_def_cfa_register 13
 212              		@ sp needed
 213 0158 80BD     		pop	{r7, pc}
 214              		.cfi_endproc
 215              	.LFE235:
 216              		.cantunwind
 217              		.fnend
ARM GAS  /tmp/cc4qm6LU.s 			page 6


 219              		.section	.text._ZN13PIDControllerD2Ev,"axG",%progbits,_ZN13PIDControllerD5Ev,comdat
 220              		.align	1
 221              		.weak	_ZN13PIDControllerD2Ev
 222              		.syntax unified
 223              		.thumb
 224              		.thumb_func
 225              		.fpu fpv4-sp-d16
 227              	_ZN13PIDControllerD2Ev:
 228              		.fnstart
 229              	.LFB4:
 230              		.file 2 "motorlib/control_fun.h"
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
  14:motorlib/control_fun.h **** };
  15:motorlib/control_fun.h **** 
  16:motorlib/control_fun.h **** 
  17:motorlib/control_fun.h **** class PIDController {
  18:motorlib/control_fun.h **** public:
  19:motorlib/control_fun.h ****     virtual ~PIDController() {}
 231              		.loc 2 19 13
 232              		.cfi_startproc
 233              		@ args = 0, pretend = 0, frame = 8
 234              		@ frame_needed = 1, uses_anonymous_args = 0
 235              		@ link register save eliminated.
 236 0000 80B4     		push	{r7}
 237              	.LCFI10:
 238              		.cfi_def_cfa_offset 4
 239              		.cfi_offset 7, -4
 240 0002 83B0     		sub	sp, sp, #12
 241              	.LCFI11:
 242              		.cfi_def_cfa_offset 16
 243 0004 00AF     		add	r7, sp, #0
 244              	.LCFI12:
 245              		.cfi_def_cfa_register 7
 246 0006 7860     		str	r0, [r7, #4]
 247              	.LBB2:
 248              		.loc 2 19 30
 249 0008 044A     		ldr	r2, .L7
 250 000a 7B68     		ldr	r3, [r7, #4]
 251 000c 1A60     		str	r2, [r3]
 252              	.LBE2:
 253              		.loc 2 19 31
 254 000e 7B68     		ldr	r3, [r7, #4]
 255 0010 1846     		mov	r0, r3
 256 0012 0C37     		adds	r7, r7, #12
 257              	.LCFI13:
ARM GAS  /tmp/cc4qm6LU.s 			page 7


 258              		.cfi_def_cfa_offset 4
 259 0014 BD46     		mov	sp, r7
 260              	.LCFI14:
 261              		.cfi_def_cfa_register 13
 262              		@ sp needed
 263 0016 5DF8047B 		ldr	r7, [sp], #4
 264              	.LCFI15:
 265              		.cfi_restore 7
 266              		.cfi_def_cfa_offset 0
 267 001a 7047     		bx	lr
 268              	.L8:
 269              		.align	2
 270              	.L7:
 271 001c 08000000 		.word	_ZTV13PIDController+8
 272              		.cfi_endproc
 273              	.LFE4:
 274              		.cantunwind
 275              		.fnend
 277              		.weak	_ZN13PIDControllerD1Ev
 278              		.thumb_set _ZN13PIDControllerD1Ev,_ZN13PIDControllerD2Ev
 279              		.section	.text._ZN13PIDControllerD0Ev,"axG",%progbits,_ZN13PIDControllerD5Ev,comdat
 280              		.align	1
 281              		.weak	_ZN13PIDControllerD0Ev
 282              		.syntax unified
 283              		.thumb
 284              		.thumb_func
 285              		.fpu fpv4-sp-d16
 287              	_ZN13PIDControllerD0Ev:
 288              		.fnstart
 289              	.LFB6:
 290              		.loc 2 19 13
 291              		.cfi_startproc
 292              		@ args = 0, pretend = 0, frame = 8
 293              		@ frame_needed = 1, uses_anonymous_args = 0
 294 0000 80B5     		push	{r7, lr}
 295              	.LCFI16:
 296              		.cfi_def_cfa_offset 8
 297              		.cfi_offset 7, -8
 298              		.cfi_offset 14, -4
 299 0002 82B0     		sub	sp, sp, #8
 300              	.LCFI17:
 301              		.cfi_def_cfa_offset 16
 302 0004 00AF     		add	r7, sp, #0
 303              	.LCFI18:
 304              		.cfi_def_cfa_register 7
 305 0006 7860     		str	r0, [r7, #4]
 306              		.loc 2 19 31
 307 0008 7868     		ldr	r0, [r7, #4]
 308 000a FFF7FEFF 		bl	_ZN13PIDControllerD1Ev
 309 000e 7868     		ldr	r0, [r7, #4]
 310 0010 FFF7FEFF 		bl	_ZdlPv
 311 0014 7B68     		ldr	r3, [r7, #4]
 312 0016 1846     		mov	r0, r3
 313 0018 0837     		adds	r7, r7, #8
 314              	.LCFI19:
 315              		.cfi_def_cfa_offset 8
 316 001a BD46     		mov	sp, r7
ARM GAS  /tmp/cc4qm6LU.s 			page 8


 317              	.LCFI20:
 318              		.cfi_def_cfa_register 13
 319              		@ sp needed
 320 001c 80BD     		pop	{r7, pc}
 321              		.cfi_endproc
 322              	.LFE6:
 323              		.cantunwind
 324              		.fnend
 326              		.section	.text._Z4fsatff,"ax",%progbits
 327              		.align	1
 328              		.global	_Z4fsatff
 329              		.syntax unified
 330              		.thumb
 331              		.thumb_func
 332              		.fpu fpv4-sp-d16
 334              	_Z4fsatff:
 335              		.fnstart
 336              	.LFB231:
   5:motorlib/control_fun.cpp **** float fsat(float a, float sat) {
 337              		.loc 1 5 32
 338              		.cfi_startproc
 339              		@ args = 0, pretend = 0, frame = 16
 340              		@ frame_needed = 1, uses_anonymous_args = 0
 341              		@ link register save eliminated.
 342 0000 80B4     		push	{r7}
 343              	.LCFI21:
 344              		.cfi_def_cfa_offset 4
 345              		.cfi_offset 7, -4
 346 0002 85B0     		sub	sp, sp, #20
 347              	.LCFI22:
 348              		.cfi_def_cfa_offset 24
 349 0004 00AF     		add	r7, sp, #0
 350              	.LCFI23:
 351              		.cfi_def_cfa_register 7
 352 0006 87ED010A 		vstr.32	s0, [r7, #4]
 353 000a C7ED000A 		vstr.32	s1, [r7]
  19:motorlib/control_fun.cpp ****     float b = a>sat ? sat : a;
 354              		.loc 1 19 21
 355 000e 97ED017A 		vldr.32	s14, [r7, #4]
 356 0012 D7ED007A 		vldr.32	s15, [r7]
 357 0016 B4EEE77A 		vcmpe.f32	s14, s15
 358 001a F1EE10FA 		vmrs	APSR_nzcv, FPSCR
 359 001e 01DD     		ble	.L21
  19:motorlib/control_fun.cpp ****     float b = a>sat ? sat : a;
 360              		.loc 1 19 21 is_stmt 0 discriminator 1
 361 0020 3B68     		ldr	r3, [r7]	@ float
 362 0022 00E0     		b	.L14
 363              	.L21:
  19:motorlib/control_fun.cpp ****     float b = a>sat ? sat : a;
 364              		.loc 1 19 21 discriminator 2
 365 0024 7B68     		ldr	r3, [r7, #4]	@ float
 366              	.L14:
  19:motorlib/control_fun.cpp ****     float b = a>sat ? sat : a;
 367              		.loc 1 19 11 is_stmt 1 discriminator 4
 368 0026 FB60     		str	r3, [r7, #12]	@ float
  20:motorlib/control_fun.cpp ****     b = b<-sat ? -sat : b;
 369              		.loc 1 20 11 discriminator 4
ARM GAS  /tmp/cc4qm6LU.s 			page 9


 370 0028 D7ED007A 		vldr.32	s15, [r7]
 371 002c F1EE677A 		vneg.f32	s15, s15
  20:motorlib/control_fun.cpp ****     b = b<-sat ? -sat : b;
 372              		.loc 1 20 16 discriminator 4
 373 0030 97ED037A 		vldr.32	s14, [r7, #12]
 374 0034 B4EEE77A 		vcmpe.f32	s14, s15
 375 0038 F1EE10FA 		vmrs	APSR_nzcv, FPSCR
 376 003c 04D5     		bpl	.L22
  20:motorlib/control_fun.cpp ****     b = b<-sat ? -sat : b;
 377              		.loc 1 20 16 is_stmt 0 discriminator 1
 378 003e D7ED007A 		vldr.32	s15, [r7]
 379 0042 F1EE677A 		vneg.f32	s15, s15
 380 0046 01E0     		b	.L17
 381              	.L22:
  20:motorlib/control_fun.cpp ****     b = b<-sat ? -sat : b;
 382              		.loc 1 20 16 discriminator 2
 383 0048 D7ED037A 		vldr.32	s15, [r7, #12]
 384              	.L17:
  20:motorlib/control_fun.cpp ****     b = b<-sat ? -sat : b;
 385              		.loc 1 20 7 is_stmt 1 discriminator 4
 386 004c C7ED037A 		vstr.32	s15, [r7, #12]
  21:motorlib/control_fun.cpp ****     return b;
 387              		.loc 1 21 12 discriminator 4
 388 0050 FB68     		ldr	r3, [r7, #12]	@ float
 389 0052 07EE903A 		vmov	s15, r3
  40:motorlib/control_fun.cpp **** }
 390              		.loc 1 40 1 discriminator 4
 391 0056 B0EE670A 		vmov.f32	s0, s15
 392 005a 1437     		adds	r7, r7, #20
 393              	.LCFI24:
 394              		.cfi_def_cfa_offset 4
 395 005c BD46     		mov	sp, r7
 396              	.LCFI25:
 397              		.cfi_def_cfa_register 13
 398              		@ sp needed
 399 005e 5DF8047B 		ldr	r7, [sp], #4
 400              	.LCFI26:
 401              		.cfi_restore 7
 402              		.cfi_def_cfa_offset 0
 403 0062 7047     		bx	lr
 404              		.cfi_endproc
 405              	.LFE231:
 406              		.cantunwind
 407              		.fnend
 409              		.section	.text._ZN12PIController9set_paramERK7PIParam,"ax",%progbits
 410              		.align	1
 411              		.global	_ZN12PIController9set_paramERK7PIParam
 412              		.syntax unified
 413              		.thumb
 414              		.thumb_func
 415              		.fpu fpv4-sp-d16
 417              	_ZN12PIController9set_paramERK7PIParam:
 418              		.fnstart
 419              	.LFB232:
  42:motorlib/control_fun.cpp **** void PIController::set_param(const PIParam &pi_param) {
 420              		.loc 1 42 55
 421              		.cfi_startproc
ARM GAS  /tmp/cc4qm6LU.s 			page 10


 422              		@ args = 0, pretend = 0, frame = 8
 423              		@ frame_needed = 1, uses_anonymous_args = 0
 424              		@ link register save eliminated.
 425 0000 80B4     		push	{r7}
 426              	.LCFI27:
 427              		.cfi_def_cfa_offset 4
 428              		.cfi_offset 7, -4
 429 0002 83B0     		sub	sp, sp, #12
 430              	.LCFI28:
 431              		.cfi_def_cfa_offset 16
 432 0004 00AF     		add	r7, sp, #0
 433              	.LCFI29:
 434              		.cfi_def_cfa_register 7
 435 0006 7860     		str	r0, [r7, #4]
 436 0008 3960     		str	r1, [r7]
  43:motorlib/control_fun.cpp ****     ki_ = pi_param.ki;
 437              		.loc 1 43 20
 438 000a 3B68     		ldr	r3, [r7]
 439 000c 5A68     		ldr	r2, [r3, #4]	@ float
  43:motorlib/control_fun.cpp ****     ki_ = pi_param.ki;
 440              		.loc 1 43 9
 441 000e 7B68     		ldr	r3, [r7, #4]
 442 0010 5A60     		str	r2, [r3, #4]	@ float
  44:motorlib/control_fun.cpp ****     kp_ = pi_param.kp;
 443              		.loc 1 44 20
 444 0012 3B68     		ldr	r3, [r7]
 445 0014 1A68     		ldr	r2, [r3]	@ float
  44:motorlib/control_fun.cpp ****     kp_ = pi_param.kp;
 446              		.loc 1 44 9
 447 0016 7B68     		ldr	r3, [r7, #4]
 448 0018 1A60     		str	r2, [r3]	@ float
  45:motorlib/control_fun.cpp ****     ki_limit_ = pi_param.ki_limit;
 449              		.loc 1 45 26
 450 001a 3B68     		ldr	r3, [r7]
 451 001c 9A68     		ldr	r2, [r3, #8]	@ float
  45:motorlib/control_fun.cpp ****     ki_limit_ = pi_param.ki_limit;
 452              		.loc 1 45 15
 453 001e 7B68     		ldr	r3, [r7, #4]
 454 0020 DA60     		str	r2, [r3, #12]	@ float
  46:motorlib/control_fun.cpp ****     command_max_ = pi_param.command_max;
 455              		.loc 1 46 29
 456 0022 3B68     		ldr	r3, [r7]
 457 0024 DA68     		ldr	r2, [r3, #12]	@ float
  46:motorlib/control_fun.cpp ****     command_max_ = pi_param.command_max;
 458              		.loc 1 46 18
 459 0026 7B68     		ldr	r3, [r7, #4]
 460 0028 1A61     		str	r2, [r3, #16]	@ float
  47:motorlib/control_fun.cpp **** }
 461              		.loc 1 47 1
 462 002a 00BF     		nop
 463 002c 0C37     		adds	r7, r7, #12
 464              	.LCFI30:
 465              		.cfi_def_cfa_offset 4
 466 002e BD46     		mov	sp, r7
 467              	.LCFI31:
 468              		.cfi_def_cfa_register 13
 469              		@ sp needed
ARM GAS  /tmp/cc4qm6LU.s 			page 11


 470 0030 5DF8047B 		ldr	r7, [sp], #4
 471              	.LCFI32:
 472              		.cfi_restore 7
 473              		.cfi_def_cfa_offset 0
 474 0034 7047     		bx	lr
 475              		.cfi_endproc
 476              	.LFE232:
 477              		.cantunwind
 478              		.fnend
 480              		.section	.text._ZN13PIDController9set_paramERK8PIDParam,"ax",%progbits
 481              		.align	1
 482              		.global	_ZN13PIDController9set_paramERK8PIDParam
 483              		.syntax unified
 484              		.thumb
 485              		.thumb_func
 486              		.fpu fpv4-sp-d16
 488              	_ZN13PIDController9set_paramERK8PIDParam:
 489              		.fnstart
 490              	.LFB234:
  56:motorlib/control_fun.cpp **** void PIDController::set_param(const PIDParam &param) {
 491              		.loc 1 56 54
 492              		.cfi_startproc
 493              		@ args = 0, pretend = 0, frame = 8
 494              		@ frame_needed = 1, uses_anonymous_args = 0
 495              		@ link register save eliminated.
 496 0000 80B4     		push	{r7}
 497              	.LCFI33:
 498              		.cfi_def_cfa_offset 4
 499              		.cfi_offset 7, -4
 500 0002 83B0     		sub	sp, sp, #12
 501              	.LCFI34:
 502              		.cfi_def_cfa_offset 16
 503 0004 00AF     		add	r7, sp, #0
 504              	.LCFI35:
 505              		.cfi_def_cfa_register 7
 506 0006 7860     		str	r0, [r7, #4]
 507 0008 3960     		str	r1, [r7]
  57:motorlib/control_fun.cpp ****     ki_ = param.ki;
 508              		.loc 1 57 17
 509 000a 3B68     		ldr	r3, [r7]
 510 000c 5A68     		ldr	r2, [r3, #4]	@ float
  57:motorlib/control_fun.cpp ****     ki_ = param.ki;
 511              		.loc 1 57 9
 512 000e 7B68     		ldr	r3, [r7, #4]
 513 0010 DA60     		str	r2, [r3, #12]	@ float
  58:motorlib/control_fun.cpp ****     kp_ = param.kp;
 514              		.loc 1 58 17
 515 0012 3B68     		ldr	r3, [r7]
 516 0014 1A68     		ldr	r2, [r3]	@ float
  58:motorlib/control_fun.cpp ****     kp_ = param.kp;
 517              		.loc 1 58 9
 518 0016 7B68     		ldr	r3, [r7, #4]
 519 0018 5A60     		str	r2, [r3, #4]	@ float
  59:motorlib/control_fun.cpp ****     ki_limit_ = param.ki_limit;
 520              		.loc 1 59 23
 521 001a 3B68     		ldr	r3, [r7]
 522 001c 9A68     		ldr	r2, [r3, #8]	@ float
ARM GAS  /tmp/cc4qm6LU.s 			page 12


  59:motorlib/control_fun.cpp ****     ki_limit_ = param.ki_limit;
 523              		.loc 1 59 15
 524 001e 7B68     		ldr	r3, [r7, #4]
 525 0020 5A61     		str	r2, [r3, #20]	@ float
  60:motorlib/control_fun.cpp ****     kd_ = param.kd;
 526              		.loc 1 60 17
 527 0022 3B68     		ldr	r3, [r7]
 528 0024 DA68     		ldr	r2, [r3, #12]	@ float
  60:motorlib/control_fun.cpp ****     kd_ = param.kd;
 529              		.loc 1 60 9
 530 0026 7B68     		ldr	r3, [r7, #4]
 531 0028 9A60     		str	r2, [r3, #8]	@ float
  61:motorlib/control_fun.cpp ****     command_max_ = param.command_max;
 532              		.loc 1 61 26
 533 002a 3B68     		ldr	r3, [r7]
 534 002c 1A69     		ldr	r2, [r3, #16]	@ float
  61:motorlib/control_fun.cpp ****     command_max_ = param.command_max;
 535              		.loc 1 61 18
 536 002e 7B68     		ldr	r3, [r7, #4]
 537 0030 9A61     		str	r2, [r3, #24]	@ float
  62:motorlib/control_fun.cpp **** }
 538              		.loc 1 62 1
 539 0032 00BF     		nop
 540 0034 0C37     		adds	r7, r7, #12
 541              	.LCFI36:
 542              		.cfi_def_cfa_offset 4
 543 0036 BD46     		mov	sp, r7
 544              	.LCFI37:
 545              		.cfi_def_cfa_register 13
 546              		@ sp needed
 547 0038 5DF8047B 		ldr	r7, [sp], #4
 548              	.LCFI38:
 549              		.cfi_restore 7
 550              		.cfi_def_cfa_offset 0
 551 003c 7047     		bx	lr
 552              		.cfi_endproc
 553              	.LFE234:
 554              		.cantunwind
 555              		.fnend
 557              		.global	_ZTV13PIDController
 558              		.section	.rodata._ZTV13PIDController,"a"
 559              		.align	2
 562              	_ZTV13PIDController:
 563 0000 00000000 		.word	0
 564 0004 00000000 		.word	_ZTI13PIDController
 565 0008 00000000 		.word	_ZN13PIDControllerD1Ev
 566 000c 00000000 		.word	_ZN13PIDControllerD0Ev
 567 0010 00000000 		.word	_ZN13PIDController4stepEff
 568              		.global	_ZTI13PIDController
 569              		.section	.rodata._ZTI13PIDController,"a"
 570              		.align	2
 573              	_ZTI13PIDController:
 574 0000 08000000 		.word	_ZTVN10__cxxabiv117__class_type_infoE+8
 575 0004 00000000 		.word	_ZTS13PIDController
 576              		.global	_ZTS13PIDController
 577              		.section	.rodata._ZTS13PIDController,"a"
 578              		.align	2
ARM GAS  /tmp/cc4qm6LU.s 			page 13


 581              	_ZTS13PIDController:
 582 0000 31335049 		.ascii	"13PIDController\000"
 582      44436F6E 
 582      74726F6C 
 582      6C657200 
 583              		.text
 584              	.Letext0:
 585              		.file 3 "motorlib/messages.h"
 586              		.file 4 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/cpp_type_tra
 587              		.file 5 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cmath"
 588              		.file 6 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thu
 589              		.file 7 "/opt/gcc-arm-none-eabi-8-2019-q3-update/lib/gcc/arm-none-eabi/8.3.1/include/stddef.h"
 590              		.file 8 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_types.h"
 591              		.file 9 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/reent.h"
 592              		.file 10 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/lock.h"
 593              		.file 11 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/math.h"
 594              		.file 12 "<built-in>"
ARM GAS  /tmp/cc4qm6LU.s 			page 14


DEFINED SYMBOLS
                            *ABS*:0000000000000000 control_fun.cpp
     /tmp/cc4qm6LU.s:18     .ccmram:0000000000000000 $t
     /tmp/cc4qm6LU.s:26     .ccmram:0000000000000000 _ZN12PIController4stepEff
     /tmp/cc4qm6LU.s:334    .text._Z4fsatff:0000000000000000 _Z4fsatff
                .ARM.exidx.ccmram:0000000000000000 $d
     /tmp/cc4qm6LU.s:117    .ccmram:0000000000000098 _ZN13PIDController4stepEff
     /tmp/cc4qm6LU.s:220    .text._ZN13PIDControllerD2Ev:0000000000000000 $t
     /tmp/cc4qm6LU.s:227    .text._ZN13PIDControllerD2Ev:0000000000000000 _ZN13PIDControllerD2Ev
     /tmp/cc4qm6LU.s:271    .text._ZN13PIDControllerD2Ev:000000000000001c $d
     /tmp/cc4qm6LU.s:562    .rodata._ZTV13PIDController:0000000000000000 _ZTV13PIDController
.ARM.exidx.text._ZN13PIDControllerD2Ev:0000000000000000 $d
     /tmp/cc4qm6LU.s:227    .text._ZN13PIDControllerD2Ev:0000000000000000 _ZN13PIDControllerD1Ev
     /tmp/cc4qm6LU.s:280    .text._ZN13PIDControllerD0Ev:0000000000000000 $t
     /tmp/cc4qm6LU.s:287    .text._ZN13PIDControllerD0Ev:0000000000000000 _ZN13PIDControllerD0Ev
.ARM.exidx.text._ZN13PIDControllerD0Ev:0000000000000000 $d
     /tmp/cc4qm6LU.s:327    .text._Z4fsatff:0000000000000000 $t
        .ARM.exidx.text._Z4fsatff:0000000000000000 $d
     /tmp/cc4qm6LU.s:410    .text._ZN12PIController9set_paramERK7PIParam:0000000000000000 $t
     /tmp/cc4qm6LU.s:417    .text._ZN12PIController9set_paramERK7PIParam:0000000000000000 _ZN12PIController9set_paramERK7PIParam
.ARM.exidx.text._ZN12PIController9set_paramERK7PIParam:0000000000000000 $d
     /tmp/cc4qm6LU.s:481    .text._ZN13PIDController9set_paramERK8PIDParam:0000000000000000 $t
     /tmp/cc4qm6LU.s:488    .text._ZN13PIDController9set_paramERK8PIDParam:0000000000000000 _ZN13PIDController9set_paramERK8PIDParam
.ARM.exidx.text._ZN13PIDController9set_paramERK8PIDParam:0000000000000000 $d
     /tmp/cc4qm6LU.s:559    .rodata._ZTV13PIDController:0000000000000000 $d
     /tmp/cc4qm6LU.s:573    .rodata._ZTI13PIDController:0000000000000000 _ZTI13PIDController
     /tmp/cc4qm6LU.s:570    .rodata._ZTI13PIDController:0000000000000000 $d
     /tmp/cc4qm6LU.s:581    .rodata._ZTS13PIDController:0000000000000000 _ZTS13PIDController
     /tmp/cc4qm6LU.s:578    .rodata._ZTS13PIDController:0000000000000000 $d
                           .group:0000000000000000 _ZN13PIDControllerD5Ev

UNDEFINED SYMBOLS
_ZdlPv
_ZTVN10__cxxabiv117__class_type_infoE
