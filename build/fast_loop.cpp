ARM GAS  /tmp/cc2LZZBV.s 			page 1


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
  13              		.file	"fast_loop.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.ccmram,"ax",%progbits
  18              		.align	1
  19              		.global	_ZN8FastLoop6updateEv
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN8FastLoop6updateEv:
  27              		.fnstart
  28              	.LFB374:
  29              		.file 1 "motorlib/fast_loop.cpp"
   1:motorlib/fast_loop.cpp **** 
   2:motorlib/fast_loop.cpp **** #include "fast_loop.h"
   3:motorlib/fast_loop.cpp **** #include "foc.h"
   4:motorlib/fast_loop.cpp **** #include <cmath>
   5:motorlib/fast_loop.cpp **** #include "pwm.h"
   6:motorlib/fast_loop.cpp **** #include "util.h"
   7:motorlib/fast_loop.cpp **** #include "encoder.h"
   8:motorlib/fast_loop.cpp **** #include "../st_device.h"
   9:motorlib/fast_loop.cpp **** 
  10:motorlib/fast_loop.cpp **** FastLoop::FastLoop(PWM &pwm, Encoder &encoder) : pwm_(pwm), encoder_(encoder) {
  11:motorlib/fast_loop.cpp ****     foc_ = new FOC;
  12:motorlib/fast_loop.cpp **** }
  13:motorlib/fast_loop.cpp **** 
  14:motorlib/fast_loop.cpp **** FastLoop::~FastLoop() {
  15:motorlib/fast_loop.cpp ****     delete foc_;
  16:motorlib/fast_loop.cpp **** }
  17:motorlib/fast_loop.cpp **** 
  18:motorlib/fast_loop.cpp **** #include "sincos.h"
  19:motorlib/fast_loop.cpp **** #define M_PI 3.14159
  20:motorlib/fast_loop.cpp **** 
  21:motorlib/fast_loop.cpp **** // called at fixed frequency in an interrupt
  22:motorlib/fast_loop.cpp **** void FastLoop::update() {
  30              		.loc 1 22 25
  31              		.cfi_startproc
  32              		@ args = 0, pretend = 0, frame = 40
  33              		@ frame_needed = 1, uses_anonymous_args = 0
  34 0000 80B5     		push	{r7, lr}
  35              		.save {r7, lr}
  36              	.LCFI0:
ARM GAS  /tmp/cc2LZZBV.s 			page 2


  37              		.cfi_def_cfa_offset 8
  38              		.cfi_offset 7, -8
  39              		.cfi_offset 14, -4
  40              		.pad #40
  41 0002 8AB0     		sub	sp, sp, #40
  42              	.LCFI1:
  43              		.cfi_def_cfa_offset 48
  44              		.setfp r7, sp, #0
  45 0004 00AF     		add	r7, sp, #0
  46              	.LCFI2:
  47              		.cfi_def_cfa_register 7
  48 0006 7860     		str	r0, [r7, #4]
  23:motorlib/fast_loop.cpp ****     // trigger encoder read
  24:motorlib/fast_loop.cpp ****     encoder_.trigger();
  49              		.loc 1 24 5
  50 0008 7B68     		ldr	r3, [r7, #4]
  51 000a 03F58753 		add	r3, r3, #4320
  52 000e 0433     		adds	r3, r3, #4
  53 0010 1A68     		ldr	r2, [r3]
  54 0012 7B68     		ldr	r3, [r7, #4]
  55 0014 03F58753 		add	r3, r3, #4320
  56 0018 0433     		adds	r3, r3, #4
  57 001a 1B68     		ldr	r3, [r3]
  58              		.loc 1 24 22
  59 001c 1B68     		ldr	r3, [r3]
  60 001e 0433     		adds	r3, r3, #4
  61 0020 1B68     		ldr	r3, [r3]
  62              		.loc 1 24 21
  63 0022 1046     		mov	r0, r2
  64 0024 9847     		blx	r3
  65              	.LVL0:
  25:motorlib/fast_loop.cpp **** 
  26:motorlib/fast_loop.cpp ****     timestamp_ = get_clock();
  66              		.loc 1 26 27
  67 0026 FFF7FEFF 		bl	_ZL9get_clockv
  68 002a 0346     		mov	r3, r0
  69 002c 1A46     		mov	r2, r3
  70              		.loc 1 26 16
  71 002e 7B68     		ldr	r3, [r7, #4]
  72 0030 03F58753 		add	r3, r3, #4320
  73 0034 1A60     		str	r2, [r3]
  27:motorlib/fast_loop.cpp **** 
  28:motorlib/fast_loop.cpp ****     // get ADC
  29:motorlib/fast_loop.cpp ****     adc1 = ADC1->JDR1;
  74              		.loc 1 29 18
  75 0036 4FF0A043 		mov	r3, #1342177280
  76 003a D3F88030 		ldr	r3, [r3, #128]
  77              		.loc 1 29 10
  78 003e 9AB2     		uxth	r2, r3
  79 0040 7B68     		ldr	r3, [r7, #4]
  80 0042 03F58453 		add	r3, r3, #4224
  81 0046 1C33     		adds	r3, r3, #28
  82 0048 1A80     		strh	r2, [r3]	@ movhi
  30:motorlib/fast_loop.cpp ****     adc2 = ADC2->JDR1;
  83              		.loc 1 30 18
  84 004a D24B     		ldr	r3, .L5
  85 004c D3F88030 		ldr	r3, [r3, #128]
ARM GAS  /tmp/cc2LZZBV.s 			page 3


  86              		.loc 1 30 10
  87 0050 9AB2     		uxth	r2, r3
  88 0052 7B68     		ldr	r3, [r7, #4]
  89 0054 03F58453 		add	r3, r3, #4224
  90 0058 1E33     		adds	r3, r3, #30
  91 005a 1A80     		strh	r2, [r3]	@ movhi
  31:motorlib/fast_loop.cpp ****     adc3 = ADC3->JDR1;
  92              		.loc 1 31 18
  93 005c CE4B     		ldr	r3, .L5+4
  94 005e D3F88030 		ldr	r3, [r3, #128]
  95              		.loc 1 31 10
  96 0062 9AB2     		uxth	r2, r3
  97 0064 7B68     		ldr	r3, [r7, #4]
  98 0066 03F58553 		add	r3, r3, #4256
  99 006a 1A80     		strh	r2, [r3]	@ movhi
  32:motorlib/fast_loop.cpp ****     foc_command_.measured.i_a = param_.adc1_gain*(adc1-param_.adc1_offset) - ia_bias_;
 100              		.loc 1 32 40
 101 006c 7B68     		ldr	r3, [r7, #4]
 102 006e 93ED047A 		vldr.32	s14, [r3, #16]
 103              		.loc 1 32 51
 104 0072 7B68     		ldr	r3, [r7, #4]
 105 0074 03F58453 		add	r3, r3, #4224
 106 0078 1C33     		adds	r3, r3, #28
 107 007a 1B88     		ldrh	r3, [r3]
 108 007c 07EE903A 		vmov	s15, r3	@ int
 109              		.loc 1 32 55
 110 0080 F8EEE76A 		vcvt.f32.s32	s13, s15
 111              		.loc 1 32 63
 112 0084 7B68     		ldr	r3, [r7, #4]
 113 0086 D3ED017A 		vldr.32	s15, [r3, #4]
 114              		.loc 1 32 55
 115 008a 76EEE77A 		vsub.f32	s15, s13, s15
 116              		.loc 1 32 49
 117 008e 27EE277A 		vmul.f32	s14, s14, s15
 118              		.loc 1 32 78
 119 0092 7B68     		ldr	r3, [r7, #4]
 120 0094 03F58653 		add	r3, r3, #4288
 121 0098 0C33     		adds	r3, r3, #12
 122 009a D3ED007A 		vldr.32	s15, [r3]
 123              		.loc 1 32 76
 124 009e 77EE677A 		vsub.f32	s15, s14, s15
 125              		.loc 1 32 31
 126 00a2 7B68     		ldr	r3, [r7, #4]
 127 00a4 03F58553 		add	r3, r3, #4256
 128 00a8 0C33     		adds	r3, r3, #12
 129 00aa C3ED007A 		vstr.32	s15, [r3]
  33:motorlib/fast_loop.cpp ****     foc_command_.measured.i_b = param_.adc2_gain*(adc2-param_.adc2_offset) - ib_bias_;
 130              		.loc 1 33 40
 131 00ae 7B68     		ldr	r3, [r7, #4]
 132 00b0 93ED057A 		vldr.32	s14, [r3, #20]
 133              		.loc 1 33 51
 134 00b4 7B68     		ldr	r3, [r7, #4]
 135 00b6 03F58453 		add	r3, r3, #4224
 136 00ba 1E33     		adds	r3, r3, #30
 137 00bc 1B88     		ldrh	r3, [r3]
 138 00be 07EE903A 		vmov	s15, r3	@ int
 139              		.loc 1 33 55
ARM GAS  /tmp/cc2LZZBV.s 			page 4


 140 00c2 F8EEE76A 		vcvt.f32.s32	s13, s15
 141              		.loc 1 33 63
 142 00c6 7B68     		ldr	r3, [r7, #4]
 143 00c8 D3ED027A 		vldr.32	s15, [r3, #8]
 144              		.loc 1 33 55
 145 00cc 76EEE77A 		vsub.f32	s15, s13, s15
 146              		.loc 1 33 49
 147 00d0 27EE277A 		vmul.f32	s14, s14, s15
 148              		.loc 1 33 78
 149 00d4 7B68     		ldr	r3, [r7, #4]
 150 00d6 03F58653 		add	r3, r3, #4288
 151 00da 1033     		adds	r3, r3, #16
 152 00dc D3ED007A 		vldr.32	s15, [r3]
 153              		.loc 1 33 76
 154 00e0 77EE677A 		vsub.f32	s15, s14, s15
 155              		.loc 1 33 31
 156 00e4 7B68     		ldr	r3, [r7, #4]
 157 00e6 03F58553 		add	r3, r3, #4256
 158 00ea 1033     		adds	r3, r3, #16
 159 00ec C3ED007A 		vstr.32	s15, [r3]
  34:motorlib/fast_loop.cpp ****     foc_command_.measured.i_c = param_.adc3_gain*(adc3-param_.adc3_offset) - ic_bias_;
 160              		.loc 1 34 40
 161 00f0 7B68     		ldr	r3, [r7, #4]
 162 00f2 93ED067A 		vldr.32	s14, [r3, #24]
 163              		.loc 1 34 51
 164 00f6 7B68     		ldr	r3, [r7, #4]
 165 00f8 03F58553 		add	r3, r3, #4256
 166 00fc 1B88     		ldrh	r3, [r3]
 167 00fe 07EE903A 		vmov	s15, r3	@ int
 168              		.loc 1 34 55
 169 0102 F8EEE76A 		vcvt.f32.s32	s13, s15
 170              		.loc 1 34 63
 171 0106 7B68     		ldr	r3, [r7, #4]
 172 0108 D3ED037A 		vldr.32	s15, [r3, #12]
 173              		.loc 1 34 55
 174 010c 76EEE77A 		vsub.f32	s15, s13, s15
 175              		.loc 1 34 49
 176 0110 27EE277A 		vmul.f32	s14, s14, s15
 177              		.loc 1 34 78
 178 0114 7B68     		ldr	r3, [r7, #4]
 179 0116 03F58653 		add	r3, r3, #4288
 180 011a 1433     		adds	r3, r3, #20
 181 011c D3ED007A 		vldr.32	s15, [r3]
 182              		.loc 1 34 76
 183 0120 77EE677A 		vsub.f32	s15, s14, s15
 184              		.loc 1 34 31
 185 0124 7B68     		ldr	r3, [r7, #4]
 186 0126 03F58553 		add	r3, r3, #4256
 187 012a 1433     		adds	r3, r3, #20
 188 012c C3ED007A 		vstr.32	s15, [r3]
  35:motorlib/fast_loop.cpp ****     foc_command_.desired.i_d = 0;
 189              		.loc 1 35 30
 190 0130 7B68     		ldr	r3, [r7, #4]
 191 0132 03F58553 		add	r3, r3, #4256
 192 0136 0433     		adds	r3, r3, #4
 193 0138 4FF00002 		mov	r2, #0
 194 013c 1A60     		str	r2, [r3]	@ float
ARM GAS  /tmp/cc2LZZBV.s 			page 5


  36:motorlib/fast_loop.cpp ****     
  37:motorlib/fast_loop.cpp ****     // get encoder value, may wait a little
  38:motorlib/fast_loop.cpp ****     motor_enc = encoder_.get_value();
 195              		.loc 1 38 17
 196 013e 7B68     		ldr	r3, [r7, #4]
 197 0140 03F58753 		add	r3, r3, #4320
 198 0144 0433     		adds	r3, r3, #4
 199 0146 1A68     		ldr	r2, [r3]
 200 0148 7B68     		ldr	r3, [r7, #4]
 201 014a 03F58753 		add	r3, r3, #4320
 202 014e 0433     		adds	r3, r3, #4
 203 0150 1B68     		ldr	r3, [r3]
 204              		.loc 1 38 36
 205 0152 1B68     		ldr	r3, [r3]
 206 0154 1B68     		ldr	r3, [r3]
 207              		.loc 1 38 35
 208 0156 1046     		mov	r0, r2
 209 0158 9847     		blx	r3
 210              	.LVL1:
 211 015a 0246     		mov	r2, r0
 212              		.loc 1 38 15
 213 015c 7B68     		ldr	r3, [r7, #4]
 214 015e 03F58353 		add	r3, r3, #4192
 215 0162 0C33     		adds	r3, r3, #12
 216 0164 1A60     		str	r2, [r3]
  39:motorlib/fast_loop.cpp **** 
  40:motorlib/fast_loop.cpp ****     motor_position_ = param_.motor_encoder.dir * 2 * (float) M_PI * inv_motor_encoder_cpr_ * motor_
 217              		.loc 1 40 44
 218 0166 7B68     		ldr	r3, [r7, #4]
 219 0168 D3ED157A 		vldr.32	s15, [r3, #84]
 220              		.loc 1 40 48
 221 016c 77EEA77A 		vadd.f32	s15, s15, s15
 222              		.loc 1 40 52
 223 0170 9FED8A7A 		vldr.32	s14, .L5+8
 224 0174 27EE877A 		vmul.f32	s14, s15, s14
 225              		.loc 1 40 69
 226 0178 7B68     		ldr	r3, [r7, #4]
 227 017a 03F58653 		add	r3, r3, #4288
 228 017e 0433     		adds	r3, r3, #4
 229 0180 D3ED007A 		vldr.32	s15, [r3]
 230              		.loc 1 40 67
 231 0184 27EE277A 		vmul.f32	s14, s14, s15
 232              		.loc 1 40 94
 233 0188 7B68     		ldr	r3, [r7, #4]
 234 018a 03F58353 		add	r3, r3, #4192
 235 018e 0C33     		adds	r3, r3, #12
 236 0190 1B68     		ldr	r3, [r3]
 237              		.loc 1 40 92
 238 0192 07EE903A 		vmov	s15, r3	@ int
 239 0196 F8EEE77A 		vcvt.f32.s32	s15, s15
 240 019a 67EE277A 		vmul.f32	s15, s14, s15
 241              		.loc 1 40 21
 242 019e 7B68     		ldr	r3, [r7, #4]
 243 01a0 03F58353 		add	r3, r3, #4192
 244 01a4 1433     		adds	r3, r3, #20
 245 01a6 C3ED007A 		vstr.32	s15, [r3]
  41:motorlib/fast_loop.cpp ****     motor_velocity =  param_.motor_encoder.dir * (motor_enc-last_motor_enc)*(2*(float) M_PI * inv_m
ARM GAS  /tmp/cc2LZZBV.s 			page 6


 246              		.loc 1 41 44
 247 01aa 7B68     		ldr	r3, [r7, #4]
 248 01ac 93ED157A 		vldr.32	s14, [r3, #84]
 249              		.loc 1 41 51
 250 01b0 7B68     		ldr	r3, [r7, #4]
 251 01b2 03F58353 		add	r3, r3, #4192
 252 01b6 0C33     		adds	r3, r3, #12
 253 01b8 1A68     		ldr	r2, [r3]
 254              		.loc 1 41 61
 255 01ba 7B68     		ldr	r3, [r7, #4]
 256 01bc 03F58353 		add	r3, r3, #4192
 257 01c0 1033     		adds	r3, r3, #16
 258 01c2 1B68     		ldr	r3, [r3]
 259              		.loc 1 41 60
 260 01c4 D31A     		subs	r3, r2, r3
 261              		.loc 1 41 48
 262 01c6 07EE903A 		vmov	s15, r3	@ int
 263 01ca F8EEE77A 		vcvt.f32.s32	s15, s15
 264 01ce 27EE277A 		vmul.f32	s14, s14, s15
 265              		.loc 1 41 95
 266 01d2 7B68     		ldr	r3, [r7, #4]
 267 01d4 03F58653 		add	r3, r3, #4288
 268 01d8 0433     		adds	r3, r3, #4
 269 01da D3ED007A 		vldr.32	s15, [r3]
 270              		.loc 1 41 93
 271 01de DFED706A 		vldr.32	s13, .L5+12
 272 01e2 67EEA66A 		vmul.f32	s13, s15, s13
 273              		.loc 1 41 120
 274 01e6 7B68     		ldr	r3, [r7, #4]
 275 01e8 03F58653 		add	r3, r3, #4288
 276 01ec 0833     		adds	r3, r3, #8
 277 01ee 1B68     		ldr	r3, [r3]
 278              		.loc 1 41 118
 279 01f0 07EE903A 		vmov	s15, r3	@ int
 280 01f4 F8EEE77A 		vcvt.f32.s32	s15, s15
 281 01f8 66EEA77A 		vmul.f32	s15, s13, s15
 282              		.loc 1 41 76
 283 01fc 67EE277A 		vmul.f32	s15, s14, s15
 284              		.loc 1 41 20
 285 0200 7B68     		ldr	r3, [r7, #4]
 286 0202 03F58353 		add	r3, r3, #4192
 287 0206 1833     		adds	r3, r3, #24
 288 0208 C3ED007A 		vstr.32	s15, [r3]
  42:motorlib/fast_loop.cpp ****     motor_velocity_filtered = (1-alpha)*motor_velocity_filtered + alpha*motor_velocity;
 289              		.loc 1 42 34
 290 020c 7B68     		ldr	r3, [r7, #4]
 291 020e 03F58453 		add	r3, r3, #4224
 292 0212 D3ED007A 		vldr.32	s15, [r3]
 293              		.loc 1 42 33
 294 0216 B7EE007A 		vmov.f32	s14, #1.0e+0
 295 021a 37EE677A 		vsub.f32	s14, s14, s15
 296              		.loc 1 42 41
 297 021e 7B68     		ldr	r3, [r7, #4]
 298 0220 03F58353 		add	r3, r3, #4192
 299 0224 1C33     		adds	r3, r3, #28
 300 0226 D3ED007A 		vldr.32	s15, [r3]
 301              		.loc 1 42 40
ARM GAS  /tmp/cc2LZZBV.s 			page 7


 302 022a 27EE277A 		vmul.f32	s14, s14, s15
 303              		.loc 1 42 67
 304 022e 7B68     		ldr	r3, [r7, #4]
 305 0230 03F58453 		add	r3, r3, #4224
 306 0234 D3ED006A 		vldr.32	s13, [r3]
 307              		.loc 1 42 73
 308 0238 7B68     		ldr	r3, [r7, #4]
 309 023a 03F58353 		add	r3, r3, #4192
 310 023e 1833     		adds	r3, r3, #24
 311 0240 D3ED007A 		vldr.32	s15, [r3]
 312              		.loc 1 42 72
 313 0244 66EEA77A 		vmul.f32	s15, s13, s15
 314              		.loc 1 42 65
 315 0248 77EE277A 		vadd.f32	s15, s14, s15
 316              		.loc 1 42 29
 317 024c 7B68     		ldr	r3, [r7, #4]
 318 024e 03F58353 		add	r3, r3, #4192
 319 0252 1C33     		adds	r3, r3, #28
 320 0254 C3ED007A 		vstr.32	s15, [r3]
  43:motorlib/fast_loop.cpp ****     last_motor_enc = motor_enc;
 321              		.loc 1 43 22
 322 0258 7B68     		ldr	r3, [r7, #4]
 323 025a 03F58353 		add	r3, r3, #4192
 324 025e 0C33     		adds	r3, r3, #12
 325 0260 1A68     		ldr	r2, [r3]
 326              		.loc 1 43 20
 327 0262 7B68     		ldr	r3, [r7, #4]
 328 0264 03F58353 		add	r3, r3, #4192
 329 0268 1033     		adds	r3, r3, #16
 330 026a 1A60     		str	r2, [r3]
  44:motorlib/fast_loop.cpp **** 
  45:motorlib/fast_loop.cpp ****     // cogging compensation, interpolate in the table
  46:motorlib/fast_loop.cpp ****     motor_mechanical_position_ = (motor_enc - motor_index_pos_); 
 331              		.loc 1 46 35
 332 026c 7B68     		ldr	r3, [r7, #4]
 333 026e 03F58353 		add	r3, r3, #4192
 334 0272 0C33     		adds	r3, r3, #12
 335 0274 1A68     		ldr	r2, [r3]
 336              		.loc 1 46 47
 337 0276 7B68     		ldr	r3, [r7, #4]
 338 0278 03F58553 		add	r3, r3, #4256
 339 027c 1C33     		adds	r3, r3, #28
 340 027e 1B68     		ldr	r3, [r3]
 341              		.loc 1 46 45
 342 0280 D31A     		subs	r3, r2, r3
 343              		.loc 1 46 32
 344 0282 07EE903A 		vmov	s15, r3	@ int
 345 0286 F8EEE77A 		vcvt.f32.s32	s15, s15
 346 028a 7B68     		ldr	r3, [r7, #4]
 347 028c 03F58453 		add	r3, r3, #4224
 348 0290 0C33     		adds	r3, r3, #12
 349 0292 C3ED007A 		vstr.32	s15, [r3]
  47:motorlib/fast_loop.cpp ****     float i_pos = motor_mechanical_position_*COGGING_TABLE_SIZE*inv_motor_encoder_cpr_;
 350              		.loc 1 47 19
 351 0296 7B68     		ldr	r3, [r7, #4]
 352 0298 03F58453 		add	r3, r3, #4224
 353 029c 0C33     		adds	r3, r3, #12
ARM GAS  /tmp/cc2LZZBV.s 			page 8


 354 029e D3ED007A 		vldr.32	s15, [r3]
 355              		.loc 1 47 45
 356 02a2 9FED407A 		vldr.32	s14, .L5+16
 357 02a6 27EE877A 		vmul.f32	s14, s15, s14
 358              		.loc 1 47 65
 359 02aa 7B68     		ldr	r3, [r7, #4]
 360 02ac 03F58653 		add	r3, r3, #4288
 361 02b0 0433     		adds	r3, r3, #4
 362 02b2 D3ED007A 		vldr.32	s15, [r3]
 363              		.loc 1 47 11
 364 02b6 67EE277A 		vmul.f32	s15, s14, s15
 365 02ba C7ED097A 		vstr.32	s15, [r7, #36]
  48:motorlib/fast_loop.cpp ****     uint16_t i = (int16_t) i_pos & (COGGING_TABLE_SIZE - 1);
 366              		.loc 1 48 18
 367 02be D7ED097A 		vldr.32	s15, [r7, #36]
 368 02c2 FDEEE77A 		vcvt.s32.f32	s15, s15
 369 02c6 17EE903A 		vmov	r3, s15	@ int
 370 02ca 1BB2     		sxth	r3, r3
 371              		.loc 1 48 34
 372 02cc 9BB2     		uxth	r3, r3
 373              		.loc 1 48 14
 374 02ce C3F30903 		ubfx	r3, r3, #0, #10
 375 02d2 7B84     		strh	r3, [r7, #34]	@ movhi
  49:motorlib/fast_loop.cpp ****  //   float ifrac = i_pos - i;  // TODO fix for negative values
  50:motorlib/fast_loop.cpp ****     // Note (i+1) & (COGGING_TABLE_SIZE-1) allows wrap around, requires COGGING_TABLE_SIZE is multi
  51:motorlib/fast_loop.cpp ****  //   float iq_ff = param_.cogging.gain * (param_.cogging.table[i] + ifrac * (param_.cogging.table[
  52:motorlib/fast_loop.cpp ****     float iq_ff = param_.cogging.gain * param_.cogging.table[i];
 376              		.loc 1 52 34
 377 02d4 7B68     		ldr	r3, [r7, #4]
 378 02d6 03F58253 		add	r3, r3, #4160
 379 02da 1833     		adds	r3, r3, #24
 380 02dc 93ED007A 		vldr.32	s14, [r3]
 381              		.loc 1 52 63
 382 02e0 7B8C     		ldrh	r3, [r7, #34]
 383 02e2 7A68     		ldr	r2, [r7, #4]
 384 02e4 1633     		adds	r3, r3, #22
 385 02e6 9B00     		lsls	r3, r3, #2
 386 02e8 1344     		add	r3, r3, r2
 387 02ea D3ED007A 		vldr.32	s15, [r3]
 388              		.loc 1 52 11
 389 02ee 67EE277A 		vmul.f32	s15, s14, s15
 390 02f2 C7ED077A 		vstr.32	s15, [r7, #28]
  53:motorlib/fast_loop.cpp **** 
  54:motorlib/fast_loop.cpp ****     // update FOC
  55:motorlib/fast_loop.cpp ****     foc_command_.measured.motor_encoder = phase_mode_*(motor_enc - motor_electrical_zero_pos_)*(2*(
 391              		.loc 1 55 43
 392 02f6 7B68     		ldr	r3, [r7, #4]
 393 02f8 03F58453 		add	r3, r3, #4224
 394 02fc 0433     		adds	r3, r3, #4
 395 02fe 93ED007A 		vldr.32	s14, [r3]
 396              		.loc 1 55 56
 397 0302 7B68     		ldr	r3, [r7, #4]
 398 0304 03F58353 		add	r3, r3, #4192
 399 0308 0C33     		adds	r3, r3, #12
 400 030a 1A68     		ldr	r2, [r3]
 401              		.loc 1 55 68
 402 030c 7B68     		ldr	r3, [r7, #4]
ARM GAS  /tmp/cc2LZZBV.s 			page 9


 403 030e 03F58653 		add	r3, r3, #4288
 404 0312 1B68     		ldr	r3, [r3]
 405              		.loc 1 55 66
 406 0314 D31A     		subs	r3, r2, r3
 407              		.loc 1 55 54
 408 0316 07EE903A 		vmov	s15, r3	@ int
 409 031a F8EEE77A 		vcvt.f32.s32	s15, s15
 410 031e 27EE277A 		vmul.f32	s14, s14, s15
 411              		.loc 1 55 115
 412 0322 7B68     		ldr	r3, [r7, #4]
 413 0324 03F58653 		add	r3, r3, #4288
 414 0328 0433     		adds	r3, r3, #4
 415 032a D3ED007A 		vldr.32	s15, [r3]
 416              		.loc 1 55 113
 417 032e DFED1C6A 		vldr.32	s13, .L5+12
 418 0332 67EEA67A 		vmul.f32	s15, s15, s13
 419              		.loc 1 55 95
 420 0336 67EE277A 		vmul.f32	s15, s14, s15
 421              		.loc 1 55 41
 422 033a 7B68     		ldr	r3, [r7, #4]
 423 033c 03F58553 		add	r3, r3, #4256
 424 0340 1833     		adds	r3, r3, #24
 425 0342 C3ED007A 		vstr.32	s15, [r3]
  56:motorlib/fast_loop.cpp ****     foc_command_.desired.i_q = iq_des_gain_ * (iq_des + iq_ff);
 426              		.loc 1 56 32
 427 0346 7B68     		ldr	r3, [r7, #4]
 428 0348 03F58453 		add	r3, r3, #4224
 429 034c 1833     		adds	r3, r3, #24
 430 034e 93ED007A 		vldr.32	s14, [r3]
 431              		.loc 1 56 48
 432 0352 7B68     		ldr	r3, [r7, #4]
 433 0354 03F58453 		add	r3, r3, #4224
 434 0358 1033     		adds	r3, r3, #16
 435 035a D3ED006A 		vldr.32	s13, [r3]
 436              		.loc 1 56 55
 437 035e D7ED077A 		vldr.32	s15, [r7, #28]
 438 0362 76EEA77A 		vadd.f32	s15, s13, s15
 439              		.loc 1 56 45
 440 0366 67EE277A 		vmul.f32	s15, s14, s15
 441              		.loc 1 56 30
 442 036a 7B68     		ldr	r3, [r7, #4]
 443 036c 03F58553 		add	r3, r3, #4256
 444 0370 0833     		adds	r3, r3, #8
 445 0372 C3ED007A 		vstr.32	s15, [r3]
  57:motorlib/fast_loop.cpp **** 
  58:motorlib/fast_loop.cpp **** //         Sincos sincos;
  59:motorlib/fast_loop.cpp **** //         static uint32_t counter=0;
  60:motorlib/fast_loop.cpp **** //         counter++;
  61:motorlib/fast_loop.cpp **** //         if (counter > pow(2,22)) {
  62:motorlib/fast_loop.cpp **** //             counter = 0;
  63:motorlib/fast_loop.cpp **** //         }
  64:motorlib/fast_loop.cpp **** // //        float t = (timestamp_*(1.0f/170e6f))
  65:motorlib/fast_loop.cpp **** //         float t = counter*(1.0f/50000.0f);
  66:motorlib/fast_loop.cpp **** //     sincos = sincos1(2 * (float) M_PI * id_des * t);
  67:motorlib/fast_loop.cpp **** //     foc_command_.desired.i_q = iq_des*(id_des > 0 ? sincos.sin : ((sincos.sin > 0) - (sincos.sin
  68:motorlib/fast_loop.cpp **** //     foc_command_.measured.motor_encoder = 0;
  69:motorlib/fast_loop.cpp ****     
ARM GAS  /tmp/cc2LZZBV.s 			page 10


  70:motorlib/fast_loop.cpp ****     FOCStatus *foc_status = foc_->step(foc_command_);
 446              		.loc 1 70 29
 447 0376 7B68     		ldr	r3, [r7, #4]
 448 0378 03F58353 		add	r3, r3, #4192
 449 037c 1A68     		ldr	r2, [r3]
 450              		.loc 1 70 40
 451 037e 7B68     		ldr	r3, [r7, #4]
 452 0380 03F58553 		add	r3, r3, #4256
 453 0384 0433     		adds	r3, r3, #4
 454              		.loc 1 70 39
 455 0386 1946     		mov	r1, r3
 456 0388 1046     		mov	r0, r2
 457 038a FFF7FEFF 		bl	_ZN3FOC4stepERK10FOCCommand
 458 038e 0346     		mov	r3, r0
 459 0390 BB61     		str	r3, [r7, #24]
 460 0392 09E0     		b	.L6
 461              	.L7:
 462              		.align	2
 463              	.L5:
 464 0394 00010050 		.word	1342177536
 465 0398 00040050 		.word	1342178304
 466 039c D00F4940 		.word	1078530000
 467 03a0 D00FC940 		.word	1086918608
 468 03a4 00008044 		.word	1149239296
 469              	.L6:
 470              	.LBB2:
  71:motorlib/fast_loop.cpp **** 
  72:motorlib/fast_loop.cpp ****     // output pwm
  73:motorlib/fast_loop.cpp ****     // TODO better voltage mode
  74:motorlib/fast_loop.cpp ****     if (mode_ == VOLTAGE_MODE) {
 471              		.loc 1 74 9
 472 03a8 7B68     		ldr	r3, [r7, #4]
 473 03aa 03F58353 		add	r3, r3, #4192
 474 03ae 0833     		adds	r3, r3, #8
 475 03b0 1B78     		ldrb	r3, [r3]	@ zero_extendqisi2
 476              		.loc 1 74 5
 477 03b2 042B     		cmp	r3, #4
 478 03b4 11D1     		bne	.L2
 479              	.LBB3:
  75:motorlib/fast_loop.cpp ****         float abc[3] = {};
 480              		.loc 1 75 15
 481 03b6 07F10C03 		add	r3, r7, #12
 482 03ba 0022     		movs	r2, #0
 483 03bc 1A60     		str	r2, [r3]
 484 03be 5A60     		str	r2, [r3, #4]
 485 03c0 9A60     		str	r2, [r3, #8]
  76:motorlib/fast_loop.cpp ****         pwm_.set_voltage(abc);
 486              		.loc 1 76 9
 487 03c2 7B68     		ldr	r3, [r7, #4]
 488 03c4 03F58353 		add	r3, r3, #4192
 489 03c8 0433     		adds	r3, r3, #4
 490 03ca 1B68     		ldr	r3, [r3]
 491              		.loc 1 76 25
 492 03cc 07F10C02 		add	r2, r7, #12
 493 03d0 1146     		mov	r1, r2
 494 03d2 1846     		mov	r0, r3
 495 03d4 FFF7FEFF 		bl	_ZN3PWM11set_voltageEPf
ARM GAS  /tmp/cc2LZZBV.s 			page 11


 496              	.LBE3:
 497              	.LBE2:
  77:motorlib/fast_loop.cpp ****     } else {
  78:motorlib/fast_loop.cpp ****         pwm_.set_voltage(&foc_status->command.v_a);
  79:motorlib/fast_loop.cpp ****     }
  80:motorlib/fast_loop.cpp **** }
 498              		.loc 1 80 1
 499 03d8 0AE0     		b	.L4
 500              	.L2:
 501              	.LBB4:
  78:motorlib/fast_loop.cpp ****     }
 502              		.loc 1 78 9
 503 03da 7B68     		ldr	r3, [r7, #4]
 504 03dc 03F58353 		add	r3, r3, #4192
 505 03e0 0433     		adds	r3, r3, #4
 506 03e2 1A68     		ldr	r2, [r3]
  78:motorlib/fast_loop.cpp ****     }
 507              		.loc 1 78 25
 508 03e4 BB69     		ldr	r3, [r7, #24]
 509 03e6 1833     		adds	r3, r3, #24
 510 03e8 1946     		mov	r1, r3
 511 03ea 1046     		mov	r0, r2
 512 03ec FFF7FEFF 		bl	_ZN3PWM11set_voltageEPf
 513              	.L4:
 514              	.LBE4:
 515              		.loc 1 80 1
 516 03f0 00BF     		nop
 517 03f2 2837     		adds	r7, r7, #40
 518              	.LCFI3:
 519              		.cfi_def_cfa_offset 8
 520 03f4 BD46     		mov	sp, r7
 521              	.LCFI4:
 522              		.cfi_def_cfa_register 13
 523              		@ sp needed
 524 03f6 80BD     		pop	{r7, pc}
 525              		.cfi_endproc
 526              	.LFE374:
 527              		.fnend
 529              		.section	.text._ZNK3FOC10get_statusEP9FOCStatus,"axG",%progbits,_ZNK3FOC10get_statusEP9FOCStatus,c
 530              		.align	1
 531              		.weak	_ZNK3FOC10get_statusEP9FOCStatus
 532              		.syntax unified
 533              		.thumb
 534              		.thumb_func
 535              		.fpu fpv4-sp-d16
 537              	_ZNK3FOC10get_statusEP9FOCStatus:
 538              		.fnstart
 539              	.LFB2:
 540              		.file 2 "motorlib/foc.h"
   1:motorlib/foc.h **** #ifndef MOTOR_FOC_H
   2:motorlib/foc.h **** #define MOTOR_FOC_H
   3:motorlib/foc.h **** 
   4:motorlib/foc.h **** #include "messages.h"
   5:motorlib/foc.h **** 
   6:motorlib/foc.h **** class PIController;
   7:motorlib/foc.h **** 
   8:motorlib/foc.h **** class FOC {
ARM GAS  /tmp/cc2LZZBV.s 			page 12


   9:motorlib/foc.h **** public:
  10:motorlib/foc.h ****     FOC();
  11:motorlib/foc.h ****     ~FOC();
  12:motorlib/foc.h **** 
  13:motorlib/foc.h ****     FOCStatus * const step(const FOCCommand &command)  __attribute__((section (".ccmram")));
  14:motorlib/foc.h ****     void set_param(const FOCParam &param);
  15:motorlib/foc.h ****     void get_status(FOCStatus *status) const { *status = status_; }
 541              		.loc 2 15 10
 542              		.cfi_startproc
 543              		@ args = 0, pretend = 0, frame = 8
 544              		@ frame_needed = 1, uses_anonymous_args = 0
 545              		@ link register save eliminated.
 546 0000 B0B4     		push	{r4, r5, r7}
 547              	.LCFI5:
 548              		.cfi_def_cfa_offset 12
 549              		.cfi_offset 4, -12
 550              		.cfi_offset 5, -8
 551              		.cfi_offset 7, -4
 552 0002 83B0     		sub	sp, sp, #12
 553              	.LCFI6:
 554              		.cfi_def_cfa_offset 24
 555 0004 00AF     		add	r7, sp, #0
 556              	.LCFI7:
 557              		.cfi_def_cfa_register 7
 558 0006 7860     		str	r0, [r7, #4]
 559 0008 3960     		str	r1, [r7]
 560              		.loc 2 15 56
 561 000a 3A68     		ldr	r2, [r7]
 562 000c 7B68     		ldr	r3, [r7, #4]
 563 000e 1546     		mov	r5, r2
 564 0010 03F10C04 		add	r4, r3, #12
 565 0014 0FCC     		ldmia	r4!, {r0, r1, r2, r3}
 566 0016 0FC5     		stmia	r5!, {r0, r1, r2, r3}
 567 0018 0FCC     		ldmia	r4!, {r0, r1, r2, r3}
 568 001a 0FC5     		stmia	r5!, {r0, r1, r2, r3}
 569 001c 94E80700 		ldm	r4, {r0, r1, r2}
 570 0020 85E80700 		stm	r5, {r0, r1, r2}
 571              		.loc 2 15 67
 572 0024 00BF     		nop
 573 0026 0C37     		adds	r7, r7, #12
 574              	.LCFI8:
 575              		.cfi_def_cfa_offset 12
 576 0028 BD46     		mov	sp, r7
 577              	.LCFI9:
 578              		.cfi_def_cfa_register 13
 579              		@ sp needed
 580 002a B0BC     		pop	{r4, r5, r7}
 581              	.LCFI10:
 582              		.cfi_restore 7
 583              		.cfi_restore 5
 584              		.cfi_restore 4
 585              		.cfi_def_cfa_offset 0
 586 002c 7047     		bx	lr
 587              		.cfi_endproc
 588              	.LFE2:
 589              		.cantunwind
 590              		.fnend
ARM GAS  /tmp/cc2LZZBV.s 			page 13


 592              		.section	.text._ZL9get_clockv,"ax",%progbits
 593              		.align	1
 594              		.syntax unified
 595              		.thumb
 596              		.thumb_func
 597              		.fpu fpv4-sp-d16
 599              	_ZL9get_clockv:
 600              		.fnstart
 601              	.LFB360:
 602              		.file 3 "motorlib/util.h"
   1:motorlib/util.h **** #ifndef UTIL_H
   2:motorlib/util.h **** #define UTIL_H
   3:motorlib/util.h **** 
   4:motorlib/util.h **** #include "stdint.h"
   5:motorlib/util.h **** #include "core_cm4.h"
   6:motorlib/util.h **** 
   7:motorlib/util.h **** static inline uint32_t get_clock() { return DWT->CYCCNT; }
 603              		.loc 3 7 36
 604              		.cfi_startproc
 605              		@ args = 0, pretend = 0, frame = 0
 606              		@ frame_needed = 1, uses_anonymous_args = 0
 607              		@ link register save eliminated.
 608 0000 80B4     		push	{r7}
 609              	.LCFI11:
 610              		.cfi_def_cfa_offset 4
 611              		.cfi_offset 7, -4
 612 0002 00AF     		add	r7, sp, #0
 613              	.LCFI12:
 614              		.cfi_def_cfa_register 7
 615              		.loc 3 7 50
 616 0004 034B     		ldr	r3, .L11
 617 0006 5B68     		ldr	r3, [r3, #4]
 618              		.loc 3 7 58
 619 0008 1846     		mov	r0, r3
 620 000a BD46     		mov	sp, r7
 621              	.LCFI13:
 622              		.cfi_def_cfa_register 13
 623              		@ sp needed
 624 000c 5DF8047B 		ldr	r7, [sp], #4
 625              	.LCFI14:
 626              		.cfi_restore 7
 627              		.cfi_def_cfa_offset 0
 628 0010 7047     		bx	lr
 629              	.L12:
 630 0012 00BF     		.align	2
 631              	.L11:
 632 0014 001000E0 		.word	-536866816
 633              		.cfi_endproc
 634              	.LFE360:
 635              		.cantunwind
 636              		.fnend
 638              		.section	.text._ZN8FastLoopC2ER3PWMR7Encoder,"ax",%progbits
 639              		.align	1
 640              		.global	_ZN8FastLoopC2ER3PWMR7Encoder
 641              		.syntax unified
 642              		.thumb
 643              		.thumb_func
ARM GAS  /tmp/cc2LZZBV.s 			page 14


 644              		.fpu fpv4-sp-d16
 646              	_ZN8FastLoopC2ER3PWMR7Encoder:
 647              		.fnstart
 648              	.LFB369:
  10:motorlib/fast_loop.cpp ****     foc_ = new FOC;
 649              		.loc 1 10 1
 650              		.cfi_startproc
 651              		@ args = 0, pretend = 0, frame = 16
 652              		@ frame_needed = 1, uses_anonymous_args = 0
 653 0000 90B5     		push	{r4, r7, lr}
 654              		.save {r4, r7, lr}
 655              	.LCFI15:
 656              		.cfi_def_cfa_offset 12
 657              		.cfi_offset 4, -12
 658              		.cfi_offset 7, -8
 659              		.cfi_offset 14, -4
 660              		.pad #20
 661 0002 85B0     		sub	sp, sp, #20
 662              	.LCFI16:
 663              		.cfi_def_cfa_offset 32
 664              		.setfp r7, sp, #0
 665 0004 00AF     		add	r7, sp, #0
 666              	.LCFI17:
 667              		.cfi_def_cfa_register 7
 668 0006 F860     		str	r0, [r7, #12]
 669 0008 B960     		str	r1, [r7, #8]
 670 000a 7A60     		str	r2, [r7, #4]
 671              	.LBB5:
  10:motorlib/fast_loop.cpp ****     foc_ = new FOC;
 672              		.loc 1 10 77
 673 000c FB68     		ldr	r3, [r7, #12]
 674 000e 03F58353 		add	r3, r3, #4192
 675 0012 0433     		adds	r3, r3, #4
 676 0014 BA68     		ldr	r2, [r7, #8]
 677 0016 1A60     		str	r2, [r3]
 678 0018 FB68     		ldr	r3, [r7, #12]
 679 001a 03F58353 		add	r3, r3, #4192
 680 001e 0833     		adds	r3, r3, #8
 681 0020 0222     		movs	r2, #2
 682 0022 1A70     		strb	r2, [r3]
 683 0024 FB68     		ldr	r3, [r7, #12]
 684 0026 03F58353 		add	r3, r3, #4192
 685 002a 1033     		adds	r3, r3, #16
 686 002c 0022     		movs	r2, #0
 687 002e 1A60     		str	r2, [r3]
 688 0030 FB68     		ldr	r3, [r7, #12]
 689 0032 03F58353 		add	r3, r3, #4192
 690 0036 1433     		adds	r3, r3, #20
 691 0038 4FF00002 		mov	r2, #0
 692 003c 1A60     		str	r2, [r3]	@ float
 693 003e FB68     		ldr	r3, [r7, #12]
 694 0040 03F58353 		add	r3, r3, #4192
 695 0044 1833     		adds	r3, r3, #24
 696 0046 4FF00002 		mov	r2, #0
 697 004a 1A60     		str	r2, [r3]	@ float
 698 004c FB68     		ldr	r3, [r7, #12]
 699 004e 03F58353 		add	r3, r3, #4192
ARM GAS  /tmp/cc2LZZBV.s 			page 15


 700 0052 1C33     		adds	r3, r3, #28
 701 0054 4FF00002 		mov	r2, #0
 702 0058 1A60     		str	r2, [r3]	@ float
 703 005a FB68     		ldr	r3, [r7, #12]
 704 005c 03F58453 		add	r3, r3, #4224
 705 0060 3D4A     		ldr	r2, .L18
 706 0062 1A60     		str	r2, [r3]	@ float
 707 0064 FB68     		ldr	r3, [r7, #12]
 708 0066 03F58453 		add	r3, r3, #4224
 709 006a 0433     		adds	r3, r3, #4
 710 006c 4FF07E52 		mov	r2, #1065353216
 711 0070 1A60     		str	r2, [r3]	@ float
 712 0072 FB68     		ldr	r3, [r7, #12]
 713 0074 03F58453 		add	r3, r3, #4224
 714 0078 0833     		adds	r3, r3, #8
 715 007a 4FF07E52 		mov	r2, #1065353216
 716 007e 1A60     		str	r2, [r3]	@ float
 717 0080 FB68     		ldr	r3, [r7, #12]
 718 0082 03F58453 		add	r3, r3, #4224
 719 0086 0C33     		adds	r3, r3, #12
 720 0088 4FF00002 		mov	r2, #0
 721 008c 1A60     		str	r2, [r3]	@ float
 722 008e FB68     		ldr	r3, [r7, #12]
 723 0090 03F58453 		add	r3, r3, #4224
 724 0094 1033     		adds	r3, r3, #16
 725 0096 4FF00002 		mov	r2, #0
 726 009a 1A60     		str	r2, [r3]	@ float
 727 009c FB68     		ldr	r3, [r7, #12]
 728 009e 03F58453 		add	r3, r3, #4224
 729 00a2 1433     		adds	r3, r3, #20
 730 00a4 4FF00002 		mov	r2, #0
 731 00a8 1A60     		str	r2, [r3]	@ float
 732 00aa FB68     		ldr	r3, [r7, #12]
 733 00ac 03F58453 		add	r3, r3, #4224
 734 00b0 1833     		adds	r3, r3, #24
 735 00b2 4FF07E52 		mov	r2, #1065353216
 736 00b6 1A60     		str	r2, [r3]	@ float
 737 00b8 FB68     		ldr	r3, [r7, #12]
 738 00ba 03F58553 		add	r3, r3, #4256
 739 00be 0433     		adds	r3, r3, #4
 740 00c0 1A46     		mov	r2, r3
 741 00c2 0023     		movs	r3, #0
 742 00c4 1360     		str	r3, [r2]
 743 00c6 5360     		str	r3, [r2, #4]
 744 00c8 9360     		str	r3, [r2, #8]
 745 00ca D360     		str	r3, [r2, #12]
 746 00cc 1361     		str	r3, [r2, #16]
 747 00ce 5361     		str	r3, [r2, #20]
 748 00d0 FB68     		ldr	r3, [r7, #12]
 749 00d2 03F58653 		add	r3, r3, #4288
 750 00d6 0833     		adds	r3, r3, #8
 751 00d8 204A     		ldr	r2, .L18+4
 752 00da 1A60     		str	r2, [r3]
 753 00dc FB68     		ldr	r3, [r7, #12]
 754 00de 03F58653 		add	r3, r3, #4288
 755 00e2 0C33     		adds	r3, r3, #12
 756 00e4 4FF00002 		mov	r2, #0
ARM GAS  /tmp/cc2LZZBV.s 			page 16


 757 00e8 1A60     		str	r2, [r3]	@ float
 758 00ea FB68     		ldr	r3, [r7, #12]
 759 00ec 03F58653 		add	r3, r3, #4288
 760 00f0 1033     		adds	r3, r3, #16
 761 00f2 4FF00002 		mov	r2, #0
 762 00f6 1A60     		str	r2, [r3]	@ float
 763 00f8 FB68     		ldr	r3, [r7, #12]
 764 00fa 03F58653 		add	r3, r3, #4288
 765 00fe 1433     		adds	r3, r3, #20
 766 0100 4FF00002 		mov	r2, #0
 767 0104 1A60     		str	r2, [r3]	@ float
 768 0106 FB68     		ldr	r3, [r7, #12]
 769 0108 03F58653 		add	r3, r3, #4288
 770 010c 1833     		adds	r3, r3, #24
 771 010e 124A     		ldr	r2, .L18
 772 0110 1A60     		str	r2, [r3]	@ float
 773 0112 FB68     		ldr	r3, [r7, #12]
 774 0114 03F58653 		add	r3, r3, #4288
 775 0118 1C33     		adds	r3, r3, #28
 776 011a 114A     		ldr	r2, .L18+8
 777 011c 1A60     		str	r2, [r3]	@ float
 778 011e FB68     		ldr	r3, [r7, #12]
 779 0120 03F58753 		add	r3, r3, #4320
 780 0124 0433     		adds	r3, r3, #4
 781 0126 7A68     		ldr	r2, [r7, #4]
 782 0128 1A60     		str	r2, [r3]
  11:motorlib/fast_loop.cpp **** }
 783              		.loc 1 11 16
 784 012a 3820     		movs	r0, #56
 785              	.LEHB0:
 786 012c FFF7FEFF 		bl	_Znwj
 787              	.LEHE0:
 788 0130 0346     		mov	r3, r0
 789 0132 1C46     		mov	r4, r3
 790 0134 2046     		mov	r0, r4
 791              	.LEHB1:
 792 0136 FFF7FEFF 		bl	_ZN3FOCC1Ev
 793              	.LEHE1:
  11:motorlib/fast_loop.cpp **** }
 794              		.loc 1 11 10 discriminator 2
 795 013a FB68     		ldr	r3, [r7, #12]
 796 013c 03F58353 		add	r3, r3, #4192
 797 0140 1C60     		str	r4, [r3]
 798              	.LBE5:
  12:motorlib/fast_loop.cpp **** 
 799              		.loc 1 12 1 discriminator 2
 800 0142 FB68     		ldr	r3, [r7, #12]
 801 0144 04E0     		b	.L17
 802              	.L16:
 803              	.LBB6:
  11:motorlib/fast_loop.cpp **** }
 804              		.loc 1 11 16
 805 0146 2046     		mov	r0, r4
 806 0148 FFF7FEFF 		bl	_ZdlPv
 807              	.LEHB2:
 808 014c FFF7FEFF 		bl	__cxa_end_cleanup
 809              	.LEHE2:
ARM GAS  /tmp/cc2LZZBV.s 			page 17


 810              	.L17:
 811              	.LBE6:
  12:motorlib/fast_loop.cpp **** 
 812              		.loc 1 12 1
 813 0150 1846     		mov	r0, r3
 814 0152 1437     		adds	r7, r7, #20
 815              	.LCFI18:
 816              		.cfi_def_cfa_offset 12
 817 0154 BD46     		mov	sp, r7
 818              	.LCFI19:
 819              		.cfi_def_cfa_register 13
 820              		@ sp needed
 821 0156 90BD     		pop	{r4, r7, pc}
 822              	.L19:
 823              		.align	2
 824              	.L18:
 825 0158 6F12833A 		.word	981668463
 826 015c A0860100 		.word	100000
 827 0160 00004041 		.word	1094713344
 828              		.cfi_endproc
 829              	.LFE369:
 830              		.global	__gxx_personality_v0
 831              		.personality	__gxx_personality_v0
 832              		.handlerdata
 833              	.LLSDA369:
 834 000c FF       		.byte	0xff
 835 000d FF       		.byte	0xff
 836 000e 01       		.byte	0x1
 837 000f 10       		.uleb128 .LLSDACSE369-.LLSDACSB369
 838              	.LLSDACSB369:
 839 0010 AC02     		.uleb128 .LEHB0-.LFB369
 840 0012 04       		.uleb128 .LEHE0-.LEHB0
 841 0013 00       		.uleb128 0
 842 0014 00       		.uleb128 0
 843 0015 B602     		.uleb128 .LEHB1-.LFB369
 844 0017 04       		.uleb128 .LEHE1-.LEHB1
 845 0018 C602     		.uleb128 .L16-.LFB369
 846 001a 00       		.uleb128 0
 847 001b CC02     		.uleb128 .LEHB2-.LFB369
 848 001d 04       		.uleb128 .LEHE2-.LEHB2
 849 001e 00       		.uleb128 0
 850 001f 00       		.uleb128 0
 851              	.LLSDACSE369:
 852              		.section	.text._ZN8FastLoopC2ER3PWMR7Encoder
 853              		.fnend
 855              		.global	_ZN8FastLoopC1ER3PWMR7Encoder
 856              		.thumb_set _ZN8FastLoopC1ER3PWMR7Encoder,_ZN8FastLoopC2ER3PWMR7Encoder
 857              		.section	.text._ZN8FastLoopD2Ev,"ax",%progbits
 858              		.align	1
 859              		.global	_ZN8FastLoopD2Ev
 860              		.syntax unified
 861              		.thumb
 862              		.thumb_func
 863              		.fpu fpv4-sp-d16
 865              	_ZN8FastLoopD2Ev:
 866              		.fnstart
 867              	.LFB372:
ARM GAS  /tmp/cc2LZZBV.s 			page 18


  14:motorlib/fast_loop.cpp ****     delete foc_;
 868              		.loc 1 14 1
 869              		.cfi_startproc
 870              		@ args = 0, pretend = 0, frame = 8
 871              		@ frame_needed = 1, uses_anonymous_args = 0
 872 0000 90B5     		push	{r4, r7, lr}
 873              	.LCFI20:
 874              		.cfi_def_cfa_offset 12
 875              		.cfi_offset 4, -12
 876              		.cfi_offset 7, -8
 877              		.cfi_offset 14, -4
 878 0002 83B0     		sub	sp, sp, #12
 879              	.LCFI21:
 880              		.cfi_def_cfa_offset 24
 881 0004 00AF     		add	r7, sp, #0
 882              	.LCFI22:
 883              		.cfi_def_cfa_register 7
 884 0006 7860     		str	r0, [r7, #4]
 885              	.LBB7:
  15:motorlib/fast_loop.cpp **** }
 886              		.loc 1 15 12
 887 0008 7B68     		ldr	r3, [r7, #4]
 888 000a 03F58353 		add	r3, r3, #4192
 889 000e 1C68     		ldr	r4, [r3]
  15:motorlib/fast_loop.cpp **** }
 890              		.loc 1 15 16
 891 0010 002C     		cmp	r4, #0
 892 0012 05D0     		beq	.L21
  15:motorlib/fast_loop.cpp **** }
 893              		.loc 1 15 12 discriminator 1
 894 0014 2046     		mov	r0, r4
 895 0016 FFF7FEFF 		bl	_ZN3FOCD1Ev
 896 001a 2046     		mov	r0, r4
 897 001c FFF7FEFF 		bl	_ZdlPv
 898              	.L21:
 899              	.LBE7:
  16:motorlib/fast_loop.cpp **** 
 900              		.loc 1 16 1
 901 0020 7B68     		ldr	r3, [r7, #4]
 902 0022 1846     		mov	r0, r3
 903 0024 0C37     		adds	r7, r7, #12
 904              	.LCFI23:
 905              		.cfi_def_cfa_offset 12
 906 0026 BD46     		mov	sp, r7
 907              	.LCFI24:
 908              		.cfi_def_cfa_register 13
 909              		@ sp needed
 910 0028 90BD     		pop	{r4, r7, pc}
 911              		.cfi_endproc
 912              	.LFE372:
 913              		.cantunwind
 914              		.fnend
 916              		.global	_ZN8FastLoopD1Ev
 917              		.thumb_set _ZN8FastLoopD1Ev,_ZN8FastLoopD2Ev
 918              		.section	.text._ZN8FastLoop11maintenanceEv,"ax",%progbits
 919              		.align	1
 920              		.global	_ZN8FastLoop11maintenanceEv
ARM GAS  /tmp/cc2LZZBV.s 			page 19


 921              		.syntax unified
 922              		.thumb
 923              		.thumb_func
 924              		.fpu fpv4-sp-d16
 926              	_ZN8FastLoop11maintenanceEv:
 927              		.fnstart
 928              	.LFB375:
  81:motorlib/fast_loop.cpp **** 
  82:motorlib/fast_loop.cpp **** // called at a slow frequency in a non interrupt
  83:motorlib/fast_loop.cpp **** void FastLoop::maintenance() {
 929              		.loc 1 83 30
 930              		.cfi_startproc
 931              		@ args = 0, pretend = 0, frame = 8
 932              		@ frame_needed = 1, uses_anonymous_args = 0
 933 0000 80B5     		push	{r7, lr}
 934              		.save {r7, lr}
 935              	.LCFI25:
 936              		.cfi_def_cfa_offset 8
 937              		.cfi_offset 7, -8
 938              		.cfi_offset 14, -4
 939              		.pad #8
 940 0002 82B0     		sub	sp, sp, #8
 941              	.LCFI26:
 942              		.cfi_def_cfa_offset 16
 943              		.setfp r7, sp, #0
 944 0004 00AF     		add	r7, sp, #0
 945              	.LCFI27:
 946              		.cfi_def_cfa_register 7
 947 0006 7860     		str	r0, [r7, #4]
  84:motorlib/fast_loop.cpp ****     if (TIM2->SR & TIM_SR_CC3IF) {
 948              		.loc 1 84 15
 949 0008 4FF08043 		mov	r3, #1073741824
 950 000c 1B69     		ldr	r3, [r3, #16]
 951              		.loc 1 84 18
 952 000e 03F00803 		and	r3, r3, #8
 953              		.loc 1 84 32
 954 0012 002B     		cmp	r3, #0
 955 0014 14BF     		ite	ne
 956 0016 0123     		movne	r3, #1
 957 0018 0023     		moveq	r3, #0
 958 001a DBB2     		uxtb	r3, r3
 959              		.loc 1 84 5
 960 001c 002B     		cmp	r3, #0
 961 001e 23D0     		beq	.L24
  85:motorlib/fast_loop.cpp ****         // qep index received
  86:motorlib/fast_loop.cpp ****         // TODO cleared by reading CCR3?
  87:motorlib/fast_loop.cpp ****         motor_index_pos_ = TIM2->CCR3;
 962              		.loc 1 87 34
 963 0020 4FF08043 		mov	r3, #1073741824
 964 0024 DB6B     		ldr	r3, [r3, #60]
 965 0026 1A46     		mov	r2, r3
 966              		.loc 1 87 26
 967 0028 7B68     		ldr	r3, [r7, #4]
 968 002a 03F58553 		add	r3, r3, #4256
 969 002e 1C33     		adds	r3, r3, #28
 970 0030 1A60     		str	r2, [r3]
  88:motorlib/fast_loop.cpp ****         if (param_.motor_encoder.use_index_electrical_offset_pos) {
ARM GAS  /tmp/cc2LZZBV.s 			page 20


 971              		.loc 1 88 34
 972 0032 7B68     		ldr	r3, [r7, #4]
 973 0034 93F84C30 		ldrb	r3, [r3, #76]	@ zero_extendqisi2
 974              		.loc 1 88 9
 975 0038 002B     		cmp	r3, #0
 976 003a 15D0     		beq	.L24
  89:motorlib/fast_loop.cpp ****           // motor_index_electrical_offset_pos is the value of an electrical zero minus the index p
  90:motorlib/fast_loop.cpp ****           // motor_electrical_zero_pos is the offset to the initial encoder value
  91:motorlib/fast_loop.cpp ****           motor_electrical_zero_pos_ = param_.motor_encoder.index_electrical_offset_pos + motor_ind
 977              		.loc 1 91 61
 978 003c 7B68     		ldr	r3, [r7, #4]
 979 003e 93ED127A 		vldr.32	s14, [r3, #72]
 980              		.loc 1 91 91
 981 0042 7B68     		ldr	r3, [r7, #4]
 982 0044 03F58553 		add	r3, r3, #4256
 983 0048 1C33     		adds	r3, r3, #28
 984 004a 1B68     		ldr	r3, [r3]
 985              		.loc 1 91 89
 986 004c 07EE903A 		vmov	s15, r3	@ int
 987 0050 F8EEE77A 		vcvt.f32.s32	s15, s15
 988 0054 77EE277A 		vadd.f32	s15, s14, s15
 989              		.loc 1 91 38
 990 0058 FDEEE77A 		vcvt.s32.f32	s15, s15
 991 005c 17EE902A 		vmov	r2, s15	@ int
 992 0060 7B68     		ldr	r3, [r7, #4]
 993 0062 03F58653 		add	r3, r3, #4288
 994 0066 1A60     		str	r2, [r3]
 995              	.L24:
  92:motorlib/fast_loop.cpp ****         }
  93:motorlib/fast_loop.cpp ****     }
  94:motorlib/fast_loop.cpp **** 
  95:motorlib/fast_loop.cpp ****     if (mode_ == PHASE_LOCK_MODE) {
 996              		.loc 1 95 9
 997 0068 7B68     		ldr	r3, [r7, #4]
 998 006a 03F58353 		add	r3, r3, #4192
 999 006e 0833     		adds	r3, r3, #8
 1000 0070 1B78     		ldrb	r3, [r3]	@ zero_extendqisi2
 1001              		.loc 1 95 5
 1002 0072 032B     		cmp	r3, #3
 1003 0074 07D1     		bne	.L25
  96:motorlib/fast_loop.cpp ****         motor_electrical_zero_pos_ = TIM2->CNT;
 1004              		.loc 1 96 44
 1005 0076 4FF08043 		mov	r3, #1073741824
 1006 007a 5B6A     		ldr	r3, [r3, #36]
 1007 007c 1A46     		mov	r2, r3
 1008              		.loc 1 96 36
 1009 007e 7B68     		ldr	r3, [r7, #4]
 1010 0080 03F58653 		add	r3, r3, #4288
 1011 0084 1A60     		str	r2, [r3]
 1012              	.L25:
  97:motorlib/fast_loop.cpp ****     }
  98:motorlib/fast_loop.cpp **** 
  99:motorlib/fast_loop.cpp ****     v_bus_ = ADC1->DR*param_.vbus_gain;
 1013              		.loc 1 99 20
 1014 0086 4FF0A043 		mov	r3, #1342177280
 1015 008a 1B6C     		ldr	r3, [r3, #64]
 1016              		.loc 1 99 22
ARM GAS  /tmp/cc2LZZBV.s 			page 21


 1017 008c 07EE903A 		vmov	s15, r3	@ int
 1018 0090 B8EE677A 		vcvt.f32.u32	s14, s15
 1019              		.loc 1 99 30
 1020 0094 7B68     		ldr	r3, [r7, #4]
 1021 0096 03F58253 		add	r3, r3, #4160
 1022 009a 1C33     		adds	r3, r3, #28
 1023 009c D3ED007A 		vldr.32	s15, [r3]
 1024              		.loc 1 99 22
 1025 00a0 67EE277A 		vmul.f32	s15, s14, s15
 1026              		.loc 1 99 12
 1027 00a4 7B68     		ldr	r3, [r7, #4]
 1028 00a6 03F58653 		add	r3, r3, #4288
 1029 00aa 1C33     		adds	r3, r3, #28
 1030 00ac C3ED007A 		vstr.32	s15, [r3]
 100:motorlib/fast_loop.cpp ****     pwm_.set_vbus(v_bus_);
 1031              		.loc 1 100 5
 1032 00b0 7B68     		ldr	r3, [r7, #4]
 1033 00b2 03F58353 		add	r3, r3, #4192
 1034 00b6 0433     		adds	r3, r3, #4
 1035 00b8 1A68     		ldr	r2, [r3]
 1036              		.loc 1 100 18
 1037 00ba 7B68     		ldr	r3, [r7, #4]
 1038 00bc 03F58653 		add	r3, r3, #4288
 1039 00c0 1C33     		adds	r3, r3, #28
 1040 00c2 D3ED007A 		vldr.32	s15, [r3]
 1041 00c6 B0EE670A 		vmov.f32	s0, s15
 1042 00ca 1046     		mov	r0, r2
 1043 00cc FFF7FEFF 		bl	_ZN3PWM8set_vbusEf
 101:motorlib/fast_loop.cpp **** }
 1044              		.loc 1 101 1
 1045 00d0 00BF     		nop
 1046 00d2 0837     		adds	r7, r7, #8
 1047              	.LCFI28:
 1048              		.cfi_def_cfa_offset 8
 1049 00d4 BD46     		mov	sp, r7
 1050              	.LCFI29:
 1051              		.cfi_def_cfa_register 13
 1052              		@ sp needed
 1053 00d6 80BD     		pop	{r7, pc}
 1054              		.cfi_endproc
 1055              	.LFE375:
 1056              		.fnend
 1058              		.section	.text._ZN8FastLoop9set_paramERK13FastLoopParam,"ax",%progbits
 1059              		.align	1
 1060              		.global	_ZN8FastLoop9set_paramERK13FastLoopParam
 1061              		.syntax unified
 1062              		.thumb
 1063              		.thumb_func
 1064              		.fpu fpv4-sp-d16
 1066              	_ZN8FastLoop9set_paramERK13FastLoopParam:
 1067              		.fnstart
 1068              	.LFB376:
 102:motorlib/fast_loop.cpp **** 
 103:motorlib/fast_loop.cpp **** void FastLoop::set_param(const FastLoopParam &fast_loop_param) {
 1069              		.loc 1 103 64
 1070              		.cfi_startproc
 1071              		@ args = 0, pretend = 0, frame = 8
ARM GAS  /tmp/cc2LZZBV.s 			page 22


 1072              		@ frame_needed = 1, uses_anonymous_args = 0
 1073 0000 80B5     		push	{r7, lr}
 1074              		.save {r7, lr}
 1075              	.LCFI30:
 1076              		.cfi_def_cfa_offset 8
 1077              		.cfi_offset 7, -8
 1078              		.cfi_offset 14, -4
 1079              		.pad #8
 1080 0002 82B0     		sub	sp, sp, #8
 1081              	.LCFI31:
 1082              		.cfi_def_cfa_offset 16
 1083              		.setfp r7, sp, #0
 1084 0004 00AF     		add	r7, sp, #0
 1085              	.LCFI32:
 1086              		.cfi_def_cfa_register 7
 1087 0006 7860     		str	r0, [r7, #4]
 1088 0008 3960     		str	r1, [r7]
 104:motorlib/fast_loop.cpp ****     foc_->set_param(fast_loop_param.foc_param);
 1089              		.loc 1 104 5
 1090 000a 7B68     		ldr	r3, [r7, #4]
 1091 000c 03F58353 		add	r3, r3, #4192
 1092 0010 1A68     		ldr	r2, [r3]
 1093              		.loc 1 104 37
 1094 0012 3B68     		ldr	r3, [r7]
 1095 0014 1C33     		adds	r3, r3, #28
 1096              		.loc 1 104 20
 1097 0016 1946     		mov	r1, r3
 1098 0018 1046     		mov	r0, r2
 1099 001a FFF7FEFF 		bl	_ZN3FOC9set_paramERK8FOCParam
 105:motorlib/fast_loop.cpp ****     param_ = fast_loop_param;
 1100              		.loc 1 105 12
 1101 001e 7A68     		ldr	r2, [r7, #4]
 1102 0020 3B68     		ldr	r3, [r7]
 1103 0022 1046     		mov	r0, r2
 1104 0024 1946     		mov	r1, r3
 1105 0026 4FF48353 		mov	r3, #4192
 1106 002a 1A46     		mov	r2, r3
 1107 002c FFF7FEFF 		bl	memcpy
 106:motorlib/fast_loop.cpp ****     set_phase_mode();
 1108              		.loc 1 106 19
 1109 0030 7868     		ldr	r0, [r7, #4]
 1110 0032 FFF7FEFF 		bl	_ZN8FastLoop14set_phase_modeEv
 107:motorlib/fast_loop.cpp ****     inv_motor_encoder_cpr_ = param_.motor_encoder.cpr != 0 ? 1.f/param_.motor_encoder.cpr : 0;
 1111              		.loc 1 107 51
 1112 0036 7B68     		ldr	r3, [r7, #4]
 1113 0038 1B6D     		ldr	r3, [r3, #80]
 1114              		.loc 1 107 60
 1115 003a 002B     		cmp	r3, #0
 1116 003c 0AD0     		beq	.L27
 1117              		.loc 1 107 87 discriminator 1
 1118 003e 7B68     		ldr	r3, [r7, #4]
 1119 0040 1B6D     		ldr	r3, [r3, #80]
 1120              		.loc 1 107 65 discriminator 1
 1121 0042 07EE903A 		vmov	s15, r3	@ int
 1122 0046 B8EE677A 		vcvt.f32.u32	s14, s15
 1123              		.loc 1 107 60 discriminator 1
 1124 004a F7EE006A 		vmov.f32	s13, #1.0e+0
ARM GAS  /tmp/cc2LZZBV.s 			page 23


 1125 004e C6EE877A 		vdiv.f32	s15, s13, s14
 1126 0052 01E0     		b	.L28
 1127              	.L27:
 1128              		.loc 1 107 60 is_stmt 0 discriminator 2
 1129 0054 DFED097A 		vldr.32	s15, .L29
 1130              	.L28:
 1131              		.loc 1 107 28 is_stmt 1 discriminator 4
 1132 0058 7B68     		ldr	r3, [r7, #4]
 1133 005a 03F58653 		add	r3, r3, #4288
 1134 005e 0433     		adds	r3, r3, #4
 1135 0060 C3ED007A 		vstr.32	s15, [r3]
 108:motorlib/fast_loop.cpp ****     frequency_hz_ = param_.pwm_frequency;
 1136              		.loc 1 108 28 discriminator 4
 1137 0064 7B68     		ldr	r3, [r7, #4]
 1138 0066 1A68     		ldr	r2, [r3]
 1139              		.loc 1 108 19 discriminator 4
 1140 0068 7B68     		ldr	r3, [r7, #4]
 1141 006a 03F58653 		add	r3, r3, #4288
 1142 006e 0833     		adds	r3, r3, #8
 1143 0070 1A60     		str	r2, [r3]
 109:motorlib/fast_loop.cpp **** }
 1144              		.loc 1 109 1 discriminator 4
 1145 0072 00BF     		nop
 1146 0074 0837     		adds	r7, r7, #8
 1147              	.LCFI33:
 1148              		.cfi_def_cfa_offset 8
 1149 0076 BD46     		mov	sp, r7
 1150              	.LCFI34:
 1151              		.cfi_def_cfa_register 13
 1152              		@ sp needed
 1153 0078 80BD     		pop	{r7, pc}
 1154              	.L30:
 1155 007a 00BF     		.align	2
 1156              	.L29:
 1157 007c 00000000 		.word	0
 1158              		.cfi_endproc
 1159              	.LFE376:
 1160              		.fnend
 1162              		.section	.text._ZN8FastLoop12voltage_modeEv,"ax",%progbits
 1163              		.align	1
 1164              		.global	_ZN8FastLoop12voltage_modeEv
 1165              		.syntax unified
 1166              		.thumb
 1167              		.thumb_func
 1168              		.fpu fpv4-sp-d16
 1170              	_ZN8FastLoop12voltage_modeEv:
 1171              		.fnstart
 1172              	.LFB377:
 110:motorlib/fast_loop.cpp **** 
 111:motorlib/fast_loop.cpp **** void FastLoop::voltage_mode() {
 1173              		.loc 1 111 31
 1174              		.cfi_startproc
 1175              		@ args = 0, pretend = 0, frame = 8
 1176              		@ frame_needed = 1, uses_anonymous_args = 0
 1177 0000 80B5     		push	{r7, lr}
 1178              		.save {r7, lr}
 1179              	.LCFI35:
ARM GAS  /tmp/cc2LZZBV.s 			page 24


 1180              		.cfi_def_cfa_offset 8
 1181              		.cfi_offset 7, -8
 1182              		.cfi_offset 14, -4
 1183              		.pad #8
 1184 0002 82B0     		sub	sp, sp, #8
 1185              	.LCFI36:
 1186              		.cfi_def_cfa_offset 16
 1187              		.setfp r7, sp, #0
 1188 0004 00AF     		add	r7, sp, #0
 1189              	.LCFI37:
 1190              		.cfi_def_cfa_register 7
 1191 0006 7860     		str	r0, [r7, #4]
 112:motorlib/fast_loop.cpp ****     pwm_.voltage_mode();
 1192              		.loc 1 112 5
 1193 0008 7B68     		ldr	r3, [r7, #4]
 1194 000a 03F58353 		add	r3, r3, #4192
 1195 000e 0433     		adds	r3, r3, #4
 1196 0010 1B68     		ldr	r3, [r3]
 1197              		.loc 1 112 22
 1198 0012 1846     		mov	r0, r3
 1199 0014 FFF7FEFF 		bl	_ZN3PWM12voltage_modeEv
 113:motorlib/fast_loop.cpp ****     mode_ = VOLTAGE_MODE;
 1200              		.loc 1 113 11
 1201 0018 7B68     		ldr	r3, [r7, #4]
 1202 001a 03F58353 		add	r3, r3, #4192
 1203 001e 0833     		adds	r3, r3, #8
 1204 0020 0422     		movs	r2, #4
 1205 0022 1A70     		strb	r2, [r3]
 114:motorlib/fast_loop.cpp **** }
 1206              		.loc 1 114 1
 1207 0024 00BF     		nop
 1208 0026 0837     		adds	r7, r7, #8
 1209              	.LCFI38:
 1210              		.cfi_def_cfa_offset 8
 1211 0028 BD46     		mov	sp, r7
 1212              	.LCFI39:
 1213              		.cfi_def_cfa_register 13
 1214              		@ sp needed
 1215 002a 80BD     		pop	{r7, pc}
 1216              		.cfi_endproc
 1217              	.LFE377:
 1218              		.fnend
 1220              		.section	.text._ZN8FastLoop20zero_current_sensorsEv,"ax",%progbits
 1221              		.align	1
 1222              		.global	_ZN8FastLoop20zero_current_sensorsEv
 1223              		.syntax unified
 1224              		.thumb
 1225              		.thumb_func
 1226              		.fpu fpv4-sp-d16
 1228              	_ZN8FastLoop20zero_current_sensorsEv:
 1229              		.fnstart
 1230              	.LFB378:
 115:motorlib/fast_loop.cpp **** 
 116:motorlib/fast_loop.cpp **** void FastLoop::zero_current_sensors() {
 1231              		.loc 1 116 39
 1232              		.cfi_startproc
 1233              		@ args = 0, pretend = 0, frame = 8
ARM GAS  /tmp/cc2LZZBV.s 			page 25


 1234              		@ frame_needed = 1, uses_anonymous_args = 0
 1235              		@ link register save eliminated.
 1236 0000 80B4     		push	{r7}
 1237              	.LCFI40:
 1238              		.cfi_def_cfa_offset 4
 1239              		.cfi_offset 7, -4
 1240 0002 83B0     		sub	sp, sp, #12
 1241              	.LCFI41:
 1242              		.cfi_def_cfa_offset 16
 1243 0004 00AF     		add	r7, sp, #0
 1244              	.LCFI42:
 1245              		.cfi_def_cfa_register 7
 1246 0006 7860     		str	r0, [r7, #4]
 117:motorlib/fast_loop.cpp ****     ia_bias_ = (1-alpha_zero_)*ia_bias_ + alpha_zero_* param_.adc1_gain*(adc1-param_.adc1_offset);
 1247              		.loc 1 117 19
 1248 0008 7B68     		ldr	r3, [r7, #4]
 1249 000a 03F58653 		add	r3, r3, #4288
 1250 000e 1833     		adds	r3, r3, #24
 1251 0010 D3ED007A 		vldr.32	s15, [r3]
 1252              		.loc 1 117 18
 1253 0014 B7EE007A 		vmov.f32	s14, #1.0e+0
 1254 0018 37EE677A 		vsub.f32	s14, s14, s15
 1255              		.loc 1 117 32
 1256 001c 7B68     		ldr	r3, [r7, #4]
 1257 001e 03F58653 		add	r3, r3, #4288
 1258 0022 0C33     		adds	r3, r3, #12
 1259 0024 D3ED007A 		vldr.32	s15, [r3]
 1260              		.loc 1 117 31
 1261 0028 27EE277A 		vmul.f32	s14, s14, s15
 1262              		.loc 1 117 43
 1263 002c 7B68     		ldr	r3, [r7, #4]
 1264 002e 03F58653 		add	r3, r3, #4288
 1265 0032 1833     		adds	r3, r3, #24
 1266 0034 D3ED006A 		vldr.32	s13, [r3]
 1267              		.loc 1 117 63
 1268 0038 7B68     		ldr	r3, [r7, #4]
 1269 003a D3ED047A 		vldr.32	s15, [r3, #16]
 1270              		.loc 1 117 54
 1271 003e 66EEA76A 		vmul.f32	s13, s13, s15
 1272              		.loc 1 117 74
 1273 0042 7B68     		ldr	r3, [r7, #4]
 1274 0044 03F58453 		add	r3, r3, #4224
 1275 0048 1C33     		adds	r3, r3, #28
 1276 004a 1B88     		ldrh	r3, [r3]
 1277 004c 07EE903A 		vmov	s15, r3	@ int
 1278              		.loc 1 117 78
 1279 0050 B8EEE76A 		vcvt.f32.s32	s12, s15
 1280              		.loc 1 117 86
 1281 0054 7B68     		ldr	r3, [r7, #4]
 1282 0056 D3ED017A 		vldr.32	s15, [r3, #4]
 1283              		.loc 1 117 78
 1284 005a 76EE677A 		vsub.f32	s15, s12, s15
 1285              		.loc 1 117 72
 1286 005e 66EEA77A 		vmul.f32	s15, s13, s15
 1287              		.loc 1 117 41
 1288 0062 77EE277A 		vadd.f32	s15, s14, s15
 1289              		.loc 1 117 14
ARM GAS  /tmp/cc2LZZBV.s 			page 26


 1290 0066 7B68     		ldr	r3, [r7, #4]
 1291 0068 03F58653 		add	r3, r3, #4288
 1292 006c 0C33     		adds	r3, r3, #12
 1293 006e C3ED007A 		vstr.32	s15, [r3]
 118:motorlib/fast_loop.cpp ****     ib_bias_ = (1-alpha_zero_)*ib_bias_ + alpha_zero_* param_.adc2_gain*(adc2-param_.adc2_offset);
 1294              		.loc 1 118 19
 1295 0072 7B68     		ldr	r3, [r7, #4]
 1296 0074 03F58653 		add	r3, r3, #4288
 1297 0078 1833     		adds	r3, r3, #24
 1298 007a D3ED007A 		vldr.32	s15, [r3]
 1299              		.loc 1 118 18
 1300 007e B7EE007A 		vmov.f32	s14, #1.0e+0
 1301 0082 37EE677A 		vsub.f32	s14, s14, s15
 1302              		.loc 1 118 32
 1303 0086 7B68     		ldr	r3, [r7, #4]
 1304 0088 03F58653 		add	r3, r3, #4288
 1305 008c 1033     		adds	r3, r3, #16
 1306 008e D3ED007A 		vldr.32	s15, [r3]
 1307              		.loc 1 118 31
 1308 0092 27EE277A 		vmul.f32	s14, s14, s15
 1309              		.loc 1 118 43
 1310 0096 7B68     		ldr	r3, [r7, #4]
 1311 0098 03F58653 		add	r3, r3, #4288
 1312 009c 1833     		adds	r3, r3, #24
 1313 009e D3ED006A 		vldr.32	s13, [r3]
 1314              		.loc 1 118 63
 1315 00a2 7B68     		ldr	r3, [r7, #4]
 1316 00a4 D3ED057A 		vldr.32	s15, [r3, #20]
 1317              		.loc 1 118 54
 1318 00a8 66EEA76A 		vmul.f32	s13, s13, s15
 1319              		.loc 1 118 74
 1320 00ac 7B68     		ldr	r3, [r7, #4]
 1321 00ae 03F58453 		add	r3, r3, #4224
 1322 00b2 1E33     		adds	r3, r3, #30
 1323 00b4 1B88     		ldrh	r3, [r3]
 1324 00b6 07EE903A 		vmov	s15, r3	@ int
 1325              		.loc 1 118 78
 1326 00ba B8EEE76A 		vcvt.f32.s32	s12, s15
 1327              		.loc 1 118 86
 1328 00be 7B68     		ldr	r3, [r7, #4]
 1329 00c0 D3ED027A 		vldr.32	s15, [r3, #8]
 1330              		.loc 1 118 78
 1331 00c4 76EE677A 		vsub.f32	s15, s12, s15
 1332              		.loc 1 118 72
 1333 00c8 66EEA77A 		vmul.f32	s15, s13, s15
 1334              		.loc 1 118 41
 1335 00cc 77EE277A 		vadd.f32	s15, s14, s15
 1336              		.loc 1 118 14
 1337 00d0 7B68     		ldr	r3, [r7, #4]
 1338 00d2 03F58653 		add	r3, r3, #4288
 1339 00d6 1033     		adds	r3, r3, #16
 1340 00d8 C3ED007A 		vstr.32	s15, [r3]
 119:motorlib/fast_loop.cpp ****     ic_bias_ = (1-alpha_zero_)*ic_bias_ + alpha_zero_* param_.adc3_gain*(adc3-param_.adc3_offset);
 1341              		.loc 1 119 19
 1342 00dc 7B68     		ldr	r3, [r7, #4]
 1343 00de 03F58653 		add	r3, r3, #4288
 1344 00e2 1833     		adds	r3, r3, #24
ARM GAS  /tmp/cc2LZZBV.s 			page 27


 1345 00e4 D3ED007A 		vldr.32	s15, [r3]
 1346              		.loc 1 119 18
 1347 00e8 B7EE007A 		vmov.f32	s14, #1.0e+0
 1348 00ec 37EE677A 		vsub.f32	s14, s14, s15
 1349              		.loc 1 119 32
 1350 00f0 7B68     		ldr	r3, [r7, #4]
 1351 00f2 03F58653 		add	r3, r3, #4288
 1352 00f6 1433     		adds	r3, r3, #20
 1353 00f8 D3ED007A 		vldr.32	s15, [r3]
 1354              		.loc 1 119 31
 1355 00fc 27EE277A 		vmul.f32	s14, s14, s15
 1356              		.loc 1 119 43
 1357 0100 7B68     		ldr	r3, [r7, #4]
 1358 0102 03F58653 		add	r3, r3, #4288
 1359 0106 1833     		adds	r3, r3, #24
 1360 0108 D3ED006A 		vldr.32	s13, [r3]
 1361              		.loc 1 119 63
 1362 010c 7B68     		ldr	r3, [r7, #4]
 1363 010e D3ED067A 		vldr.32	s15, [r3, #24]
 1364              		.loc 1 119 54
 1365 0112 66EEA76A 		vmul.f32	s13, s13, s15
 1366              		.loc 1 119 74
 1367 0116 7B68     		ldr	r3, [r7, #4]
 1368 0118 03F58553 		add	r3, r3, #4256
 1369 011c 1B88     		ldrh	r3, [r3]
 1370 011e 07EE903A 		vmov	s15, r3	@ int
 1371              		.loc 1 119 78
 1372 0122 B8EEE76A 		vcvt.f32.s32	s12, s15
 1373              		.loc 1 119 86
 1374 0126 7B68     		ldr	r3, [r7, #4]
 1375 0128 D3ED037A 		vldr.32	s15, [r3, #12]
 1376              		.loc 1 119 78
 1377 012c 76EE677A 		vsub.f32	s15, s12, s15
 1378              		.loc 1 119 72
 1379 0130 66EEA77A 		vmul.f32	s15, s13, s15
 1380              		.loc 1 119 41
 1381 0134 77EE277A 		vadd.f32	s15, s14, s15
 1382              		.loc 1 119 14
 1383 0138 7B68     		ldr	r3, [r7, #4]
 1384 013a 03F58653 		add	r3, r3, #4288
 1385 013e 1433     		adds	r3, r3, #20
 1386 0140 C3ED007A 		vstr.32	s15, [r3]
 120:motorlib/fast_loop.cpp **** }
 1387              		.loc 1 120 1
 1388 0144 00BF     		nop
 1389 0146 0C37     		adds	r7, r7, #12
 1390              	.LCFI43:
 1391              		.cfi_def_cfa_offset 4
 1392 0148 BD46     		mov	sp, r7
 1393              	.LCFI44:
 1394              		.cfi_def_cfa_register 13
 1395              		@ sp needed
 1396 014a 5DF8047B 		ldr	r7, [sp], #4
 1397              	.LCFI45:
 1398              		.cfi_restore 7
 1399              		.cfi_def_cfa_offset 0
 1400 014e 7047     		bx	lr
ARM GAS  /tmp/cc2LZZBV.s 			page 28


 1401              		.cfi_endproc
 1402              	.LFE378:
 1403              		.cantunwind
 1404              		.fnend
 1406              		.section	.text._ZN8FastLoop15phase_lock_modeEf,"ax",%progbits
 1407              		.align	1
 1408              		.global	_ZN8FastLoop15phase_lock_modeEf
 1409              		.syntax unified
 1410              		.thumb
 1411              		.thumb_func
 1412              		.fpu fpv4-sp-d16
 1414              	_ZN8FastLoop15phase_lock_modeEf:
 1415              		.fnstart
 1416              	.LFB379:
 121:motorlib/fast_loop.cpp **** 
 122:motorlib/fast_loop.cpp **** void FastLoop::phase_lock_mode(float id) {
 1417              		.loc 1 122 42
 1418              		.cfi_startproc
 1419              		@ args = 0, pretend = 0, frame = 8
 1420              		@ frame_needed = 1, uses_anonymous_args = 0
 1421 0000 80B5     		push	{r7, lr}
 1422              		.save {r7, lr}
 1423              	.LCFI46:
 1424              		.cfi_def_cfa_offset 8
 1425              		.cfi_offset 7, -8
 1426              		.cfi_offset 14, -4
 1427              		.pad #8
 1428 0002 82B0     		sub	sp, sp, #8
 1429              	.LCFI47:
 1430              		.cfi_def_cfa_offset 16
 1431              		.setfp r7, sp, #0
 1432 0004 00AF     		add	r7, sp, #0
 1433              	.LCFI48:
 1434              		.cfi_def_cfa_register 7
 1435 0006 7860     		str	r0, [r7, #4]
 1436 0008 87ED000A 		vstr.32	s0, [r7]
 123:motorlib/fast_loop.cpp ****     phase_mode_ = 0;
 1437              		.loc 1 123 17
 1438 000c 7B68     		ldr	r3, [r7, #4]
 1439 000e 03F58453 		add	r3, r3, #4224
 1440 0012 0433     		adds	r3, r3, #4
 1441 0014 4FF00002 		mov	r2, #0
 1442 0018 1A60     		str	r2, [r3]	@ float
 124:motorlib/fast_loop.cpp ****     id_des = id;
 1443              		.loc 1 124 12
 1444 001a 7B68     		ldr	r3, [r7, #4]
 1445 001c 03F58453 		add	r3, r3, #4224
 1446 0020 1433     		adds	r3, r3, #20
 1447 0022 3A68     		ldr	r2, [r7]	@ float
 1448 0024 1A60     		str	r2, [r3]	@ float
 125:motorlib/fast_loop.cpp ****     iq_des_gain_ = 0;
 1449              		.loc 1 125 18
 1450 0026 7B68     		ldr	r3, [r7, #4]
 1451 0028 03F58453 		add	r3, r3, #4224
 1452 002c 1833     		adds	r3, r3, #24
 1453 002e 4FF00002 		mov	r2, #0
 1454 0032 1A60     		str	r2, [r3]	@ float
ARM GAS  /tmp/cc2LZZBV.s 			page 29


 126:motorlib/fast_loop.cpp ****     pwm_.voltage_mode();
 1455              		.loc 1 126 5
 1456 0034 7B68     		ldr	r3, [r7, #4]
 1457 0036 03F58353 		add	r3, r3, #4192
 1458 003a 0433     		adds	r3, r3, #4
 1459 003c 1B68     		ldr	r3, [r3]
 1460              		.loc 1 126 22
 1461 003e 1846     		mov	r0, r3
 1462 0040 FFF7FEFF 		bl	_ZN3PWM12voltage_modeEv
 127:motorlib/fast_loop.cpp ****     mode_ = PHASE_LOCK_MODE;
 1463              		.loc 1 127 11
 1464 0044 7B68     		ldr	r3, [r7, #4]
 1465 0046 03F58353 		add	r3, r3, #4192
 1466 004a 0833     		adds	r3, r3, #8
 1467 004c 0322     		movs	r2, #3
 1468 004e 1A70     		strb	r2, [r3]
 128:motorlib/fast_loop.cpp **** }
 1469              		.loc 1 128 1
 1470 0050 00BF     		nop
 1471 0052 0837     		adds	r7, r7, #8
 1472              	.LCFI49:
 1473              		.cfi_def_cfa_offset 8
 1474 0054 BD46     		mov	sp, r7
 1475              	.LCFI50:
 1476              		.cfi_def_cfa_register 13
 1477              		@ sp needed
 1478 0056 80BD     		pop	{r7, pc}
 1479              		.cfi_endproc
 1480              	.LFE379:
 1481              		.fnend
 1483              		.section	.text._ZN8FastLoop12current_modeEv,"ax",%progbits
 1484              		.align	1
 1485              		.global	_ZN8FastLoop12current_modeEv
 1486              		.syntax unified
 1487              		.thumb
 1488              		.thumb_func
 1489              		.fpu fpv4-sp-d16
 1491              	_ZN8FastLoop12current_modeEv:
 1492              		.fnstart
 1493              	.LFB380:
 129:motorlib/fast_loop.cpp **** 
 130:motorlib/fast_loop.cpp **** void FastLoop::current_mode() {
 1494              		.loc 1 130 31
 1495              		.cfi_startproc
 1496              		@ args = 0, pretend = 0, frame = 8
 1497              		@ frame_needed = 1, uses_anonymous_args = 0
 1498 0000 80B5     		push	{r7, lr}
 1499              		.save {r7, lr}
 1500              	.LCFI51:
 1501              		.cfi_def_cfa_offset 8
 1502              		.cfi_offset 7, -8
 1503              		.cfi_offset 14, -4
 1504              		.pad #8
 1505 0002 82B0     		sub	sp, sp, #8
 1506              	.LCFI52:
 1507              		.cfi_def_cfa_offset 16
 1508              		.setfp r7, sp, #0
ARM GAS  /tmp/cc2LZZBV.s 			page 30


 1509 0004 00AF     		add	r7, sp, #0
 1510              	.LCFI53:
 1511              		.cfi_def_cfa_register 7
 1512 0006 7860     		str	r0, [r7, #4]
 131:motorlib/fast_loop.cpp ****     phase_mode_ = phase_mode_desired_;
 1513              		.loc 1 131 19
 1514 0008 7B68     		ldr	r3, [r7, #4]
 1515 000a 03F58453 		add	r3, r3, #4224
 1516 000e 0833     		adds	r3, r3, #8
 1517 0010 1A68     		ldr	r2, [r3]	@ float
 1518              		.loc 1 131 17
 1519 0012 7B68     		ldr	r3, [r7, #4]
 1520 0014 03F58453 		add	r3, r3, #4224
 1521 0018 0433     		adds	r3, r3, #4
 1522 001a 1A60     		str	r2, [r3]	@ float
 132:motorlib/fast_loop.cpp ****     id_des = 0;
 1523              		.loc 1 132 12
 1524 001c 7B68     		ldr	r3, [r7, #4]
 1525 001e 03F58453 		add	r3, r3, #4224
 1526 0022 1433     		adds	r3, r3, #20
 1527 0024 4FF00002 		mov	r2, #0
 1528 0028 1A60     		str	r2, [r3]	@ float
 133:motorlib/fast_loop.cpp ****     iq_des_gain_ = 1;
 1529              		.loc 1 133 18
 1530 002a 7B68     		ldr	r3, [r7, #4]
 1531 002c 03F58453 		add	r3, r3, #4224
 1532 0030 1833     		adds	r3, r3, #24
 1533 0032 4FF07E52 		mov	r2, #1065353216
 1534 0036 1A60     		str	r2, [r3]	@ float
 134:motorlib/fast_loop.cpp ****     pwm_.voltage_mode();
 1535              		.loc 1 134 5
 1536 0038 7B68     		ldr	r3, [r7, #4]
 1537 003a 03F58353 		add	r3, r3, #4192
 1538 003e 0433     		adds	r3, r3, #4
 1539 0040 1B68     		ldr	r3, [r3]
 1540              		.loc 1 134 22
 1541 0042 1846     		mov	r0, r3
 1542 0044 FFF7FEFF 		bl	_ZN3PWM12voltage_modeEv
 135:motorlib/fast_loop.cpp ****     mode_ = CURRENT_MODE;
 1543              		.loc 1 135 11
 1544 0048 7B68     		ldr	r3, [r7, #4]
 1545 004a 03F58353 		add	r3, r3, #4192
 1546 004e 0833     		adds	r3, r3, #8
 1547 0050 0222     		movs	r2, #2
 1548 0052 1A70     		strb	r2, [r3]
 136:motorlib/fast_loop.cpp **** }
 1549              		.loc 1 136 1
 1550 0054 00BF     		nop
 1551 0056 0837     		adds	r7, r7, #8
 1552              	.LCFI54:
 1553              		.cfi_def_cfa_offset 8
 1554 0058 BD46     		mov	sp, r7
 1555              	.LCFI55:
 1556              		.cfi_def_cfa_register 13
 1557              		@ sp needed
 1558 005a 80BD     		pop	{r7, pc}
 1559              		.cfi_endproc
ARM GAS  /tmp/cc2LZZBV.s 			page 31


 1560              	.LFE380:
 1561              		.fnend
 1563              		.section	.text._ZN8FastLoop10brake_modeEv,"ax",%progbits
 1564              		.align	1
 1565              		.global	_ZN8FastLoop10brake_modeEv
 1566              		.syntax unified
 1567              		.thumb
 1568              		.thumb_func
 1569              		.fpu fpv4-sp-d16
 1571              	_ZN8FastLoop10brake_modeEv:
 1572              		.fnstart
 1573              	.LFB381:
 137:motorlib/fast_loop.cpp **** 
 138:motorlib/fast_loop.cpp **** void FastLoop::brake_mode() {
 1574              		.loc 1 138 29
 1575              		.cfi_startproc
 1576              		@ args = 0, pretend = 0, frame = 8
 1577              		@ frame_needed = 1, uses_anonymous_args = 0
 1578 0000 80B5     		push	{r7, lr}
 1579              		.save {r7, lr}
 1580              	.LCFI56:
 1581              		.cfi_def_cfa_offset 8
 1582              		.cfi_offset 7, -8
 1583              		.cfi_offset 14, -4
 1584              		.pad #8
 1585 0002 82B0     		sub	sp, sp, #8
 1586              	.LCFI57:
 1587              		.cfi_def_cfa_offset 16
 1588              		.setfp r7, sp, #0
 1589 0004 00AF     		add	r7, sp, #0
 1590              	.LCFI58:
 1591              		.cfi_def_cfa_register 7
 1592 0006 7860     		str	r0, [r7, #4]
 139:motorlib/fast_loop.cpp ****     pwm_.brake_mode();
 1593              		.loc 1 139 5
 1594 0008 7B68     		ldr	r3, [r7, #4]
 1595 000a 03F58353 		add	r3, r3, #4192
 1596 000e 0433     		adds	r3, r3, #4
 1597 0010 1B68     		ldr	r3, [r3]
 1598              		.loc 1 139 20
 1599 0012 1846     		mov	r0, r3
 1600 0014 FFF7FEFF 		bl	_ZN3PWM10brake_modeEv
 140:motorlib/fast_loop.cpp **** }
 1601              		.loc 1 140 1
 1602 0018 00BF     		nop
 1603 001a 0837     		adds	r7, r7, #8
 1604              	.LCFI59:
 1605              		.cfi_def_cfa_offset 8
 1606 001c BD46     		mov	sp, r7
 1607              	.LCFI60:
 1608              		.cfi_def_cfa_register 13
 1609              		@ sp needed
 1610 001e 80BD     		pop	{r7, pc}
 1611              		.cfi_endproc
 1612              	.LFE381:
 1613              		.fnend
 1615              		.section	.text._ZN8FastLoop9open_modeEv,"ax",%progbits
ARM GAS  /tmp/cc2LZZBV.s 			page 32


 1616              		.align	1
 1617              		.global	_ZN8FastLoop9open_modeEv
 1618              		.syntax unified
 1619              		.thumb
 1620              		.thumb_func
 1621              		.fpu fpv4-sp-d16
 1623              	_ZN8FastLoop9open_modeEv:
 1624              		.fnstart
 1625              	.LFB382:
 141:motorlib/fast_loop.cpp **** 
 142:motorlib/fast_loop.cpp **** void FastLoop::open_mode() {
 1626              		.loc 1 142 28
 1627              		.cfi_startproc
 1628              		@ args = 0, pretend = 0, frame = 8
 1629              		@ frame_needed = 1, uses_anonymous_args = 0
 1630 0000 80B5     		push	{r7, lr}
 1631              		.save {r7, lr}
 1632              	.LCFI61:
 1633              		.cfi_def_cfa_offset 8
 1634              		.cfi_offset 7, -8
 1635              		.cfi_offset 14, -4
 1636              		.pad #8
 1637 0002 82B0     		sub	sp, sp, #8
 1638              	.LCFI62:
 1639              		.cfi_def_cfa_offset 16
 1640              		.setfp r7, sp, #0
 1641 0004 00AF     		add	r7, sp, #0
 1642              	.LCFI63:
 1643              		.cfi_def_cfa_register 7
 1644 0006 7860     		str	r0, [r7, #4]
 143:motorlib/fast_loop.cpp ****     pwm_.open_mode();
 1645              		.loc 1 143 5
 1646 0008 7B68     		ldr	r3, [r7, #4]
 1647 000a 03F58353 		add	r3, r3, #4192
 1648 000e 0433     		adds	r3, r3, #4
 1649 0010 1B68     		ldr	r3, [r3]
 1650              		.loc 1 143 19
 1651 0012 1846     		mov	r0, r3
 1652 0014 FFF7FEFF 		bl	_ZN3PWM9open_modeEv
 144:motorlib/fast_loop.cpp **** }
 1653              		.loc 1 144 1
 1654 0018 00BF     		nop
 1655 001a 0837     		adds	r7, r7, #8
 1656              	.LCFI64:
 1657              		.cfi_def_cfa_offset 8
 1658 001c BD46     		mov	sp, r7
 1659              	.LCFI65:
 1660              		.cfi_def_cfa_register 13
 1661              		@ sp needed
 1662 001e 80BD     		pop	{r7, pc}
 1663              		.cfi_endproc
 1664              	.LFE382:
 1665              		.fnend
 1667              		.section	.text._ZN8FastLoop10get_statusEP14FastLoopStatus,"ax",%progbits
 1668              		.align	1
 1669              		.global	_ZN8FastLoop10get_statusEP14FastLoopStatus
 1670              		.syntax unified
ARM GAS  /tmp/cc2LZZBV.s 			page 33


 1671              		.thumb
 1672              		.thumb_func
 1673              		.fpu fpv4-sp-d16
 1675              	_ZN8FastLoop10get_statusEP14FastLoopStatus:
 1676              		.fnstart
 1677              	.LFB383:
 145:motorlib/fast_loop.cpp **** 
 146:motorlib/fast_loop.cpp **** void FastLoop::get_status(FastLoopStatus *fast_loop_status) {
 1678              		.loc 1 146 61
 1679              		.cfi_startproc
 1680              		@ args = 0, pretend = 0, frame = 8
 1681              		@ frame_needed = 1, uses_anonymous_args = 0
 1682 0000 B0B5     		push	{r4, r5, r7, lr}
 1683              	.LCFI66:
 1684              		.cfi_def_cfa_offset 16
 1685              		.cfi_offset 4, -16
 1686              		.cfi_offset 5, -12
 1687              		.cfi_offset 7, -8
 1688              		.cfi_offset 14, -4
 1689 0002 82B0     		sub	sp, sp, #8
 1690              	.LCFI67:
 1691              		.cfi_def_cfa_offset 24
 1692 0004 00AF     		add	r7, sp, #0
 1693              	.LCFI68:
 1694              		.cfi_def_cfa_register 7
 1695 0006 7860     		str	r0, [r7, #4]
 1696 0008 3960     		str	r1, [r7]
 147:motorlib/fast_loop.cpp ****     foc_->get_status(&(fast_loop_status->foc_status));
 1697              		.loc 1 147 5
 1698 000a 7B68     		ldr	r3, [r7, #4]
 1699 000c 03F58353 		add	r3, r3, #4192
 1700 0010 1A68     		ldr	r2, [r3]
 1701              		.loc 1 147 21
 1702 0012 3B68     		ldr	r3, [r7]
 1703 0014 0433     		adds	r3, r3, #4
 1704 0016 1946     		mov	r1, r3
 1705 0018 1046     		mov	r0, r2
 1706 001a FFF7FEFF 		bl	_ZNK3FOC10get_statusEP9FOCStatus
 148:motorlib/fast_loop.cpp ****     fast_loop_status->motor_mechanical_position = motor_mechanical_position_;
 1707              		.loc 1 148 51
 1708 001e 7B68     		ldr	r3, [r7, #4]
 1709 0020 03F58453 		add	r3, r3, #4224
 1710 0024 0C33     		adds	r3, r3, #12
 1711 0026 1A68     		ldr	r2, [r3]	@ float
 1712              		.loc 1 148 49
 1713 0028 3B68     		ldr	r3, [r7]
 1714 002a DA63     		str	r2, [r3, #60]	@ float
 149:motorlib/fast_loop.cpp ****     fast_loop_status->foc_command = foc_command_;
 1715              		.loc 1 149 35
 1716 002c 3A68     		ldr	r2, [r7]
 1717 002e 7B68     		ldr	r3, [r7, #4]
 1718 0030 03F58553 		add	r3, r3, #4256
 1719 0034 0433     		adds	r3, r3, #4
 1720 0036 02F14004 		add	r4, r2, #64
 1721 003a 1D46     		mov	r5, r3
 1722 003c 0FCD     		ldmia	r5!, {r0, r1, r2, r3}
 1723 003e 0FC4     		stmia	r4!, {r0, r1, r2, r3}
ARM GAS  /tmp/cc2LZZBV.s 			page 34


 1724 0040 95E80300 		ldm	r5, {r0, r1}
 1725 0044 84E80300 		stm	r4, {r0, r1}
 150:motorlib/fast_loop.cpp ****     fast_loop_status->motor_position.position = motor_position_;
 1726              		.loc 1 150 49
 1727 0048 7B68     		ldr	r3, [r7, #4]
 1728 004a 03F58353 		add	r3, r3, #4192
 1729 004e 1433     		adds	r3, r3, #20
 1730 0050 1A68     		ldr	r2, [r3]	@ float
 1731              		.loc 1 150 47
 1732 0052 3B68     		ldr	r3, [r7]
 1733 0054 5A63     		str	r2, [r3, #52]	@ float
 151:motorlib/fast_loop.cpp ****     fast_loop_status->motor_position.velocity = motor_velocity_filtered;
 1734              		.loc 1 151 49
 1735 0056 7B68     		ldr	r3, [r7, #4]
 1736 0058 03F58353 		add	r3, r3, #4192
 1737 005c 1C33     		adds	r3, r3, #28
 1738 005e 1A68     		ldr	r2, [r3]	@ float
 1739              		.loc 1 151 47
 1740 0060 3B68     		ldr	r3, [r7]
 1741 0062 9A63     		str	r2, [r3, #56]	@ float
 152:motorlib/fast_loop.cpp ****     fast_loop_status->motor_position.raw = motor_enc;
 1742              		.loc 1 152 44
 1743 0064 7B68     		ldr	r3, [r7, #4]
 1744 0066 03F58353 		add	r3, r3, #4192
 1745 006a 0C33     		adds	r3, r3, #12
 1746 006c 1A68     		ldr	r2, [r3]
 1747              		.loc 1 152 42
 1748 006e 3B68     		ldr	r3, [r7]
 1749 0070 1A63     		str	r2, [r3, #48]
 153:motorlib/fast_loop.cpp ****     fast_loop_status->timestamp = timestamp_;
 1750              		.loc 1 153 35
 1751 0072 7B68     		ldr	r3, [r7, #4]
 1752 0074 03F58753 		add	r3, r3, #4320
 1753 0078 1A68     		ldr	r2, [r3]
 1754              		.loc 1 153 33
 1755 007a 3B68     		ldr	r3, [r7]
 1756 007c 1A60     		str	r2, [r3]
 154:motorlib/fast_loop.cpp **** }
 1757              		.loc 1 154 1
 1758 007e 00BF     		nop
 1759 0080 0837     		adds	r7, r7, #8
 1760              	.LCFI69:
 1761              		.cfi_def_cfa_offset 16
 1762 0082 BD46     		mov	sp, r7
 1763              	.LCFI70:
 1764              		.cfi_def_cfa_register 13
 1765              		@ sp needed
 1766 0084 B0BD     		pop	{r4, r5, r7, pc}
 1767              		.cfi_endproc
 1768              	.LFE383:
 1769              		.cantunwind
 1770              		.fnend
 1772              		.section	.text._ZN8FastLoop14set_phase_modeEv,"ax",%progbits
 1773              		.align	1
 1774              		.global	_ZN8FastLoop14set_phase_modeEv
 1775              		.syntax unified
 1776              		.thumb
ARM GAS  /tmp/cc2LZZBV.s 			page 35


 1777              		.thumb_func
 1778              		.fpu fpv4-sp-d16
 1780              	_ZN8FastLoop14set_phase_modeEv:
 1781              		.fnstart
 1782              	.LFB384:
 155:motorlib/fast_loop.cpp **** 
 156:motorlib/fast_loop.cpp **** void FastLoop::set_phase_mode() {
 1783              		.loc 1 156 33
 1784              		.cfi_startproc
 1785              		@ args = 0, pretend = 0, frame = 8
 1786              		@ frame_needed = 1, uses_anonymous_args = 0
 1787              		@ link register save eliminated.
 1788 0000 80B4     		push	{r7}
 1789              	.LCFI71:
 1790              		.cfi_def_cfa_offset 4
 1791              		.cfi_offset 7, -4
 1792 0002 83B0     		sub	sp, sp, #12
 1793              	.LCFI72:
 1794              		.cfi_def_cfa_offset 16
 1795 0004 00AF     		add	r7, sp, #0
 1796              	.LCFI73:
 1797              		.cfi_def_cfa_register 7
 1798 0006 7860     		str	r0, [r7, #4]
 157:motorlib/fast_loop.cpp ****     phase_mode_desired_ = param_.phase_mode == 0 ? 1 : -1;
 1799              		.loc 1 157 34
 1800 0008 7B68     		ldr	r3, [r7, #4]
 1801 000a 93F84430 		ldrb	r3, [r3, #68]	@ zero_extendqisi2
 1802              		.loc 1 157 25
 1803 000e 002B     		cmp	r3, #0
 1804 0010 02D1     		bne	.L39
 1805              		.loc 1 157 25 is_stmt 0 discriminator 1
 1806 0012 4FF07E52 		mov	r2, #1065353216
 1807 0016 00E0     		b	.L40
 1808              	.L39:
 1809              		.loc 1 157 25 discriminator 2
 1810 0018 054A     		ldr	r2, .L41
 1811              	.L40:
 1812              		.loc 1 157 25 discriminator 4
 1813 001a 7B68     		ldr	r3, [r7, #4]
 1814 001c 03F58453 		add	r3, r3, #4224
 1815 0020 0833     		adds	r3, r3, #8
 1816 0022 1A60     		str	r2, [r3]	@ float
 158:motorlib/fast_loop.cpp **** }...
 1817              		.loc 1 158 1 is_stmt 1 discriminator 4
 1818 0024 00BF     		nop
 1819 0026 0C37     		adds	r7, r7, #12
 1820              	.LCFI74:
 1821              		.cfi_def_cfa_offset 4
 1822 0028 BD46     		mov	sp, r7
 1823              	.LCFI75:
 1824              		.cfi_def_cfa_register 13
 1825              		@ sp needed
 1826 002a 5DF8047B 		ldr	r7, [sp], #4
 1827              	.LCFI76:
 1828              		.cfi_restore 7
 1829              		.cfi_def_cfa_offset 0
 1830 002e 7047     		bx	lr
ARM GAS  /tmp/cc2LZZBV.s 			page 36


 1831              	.L42:
 1832              		.align	2
 1833              	.L41:
 1834 0030 000080BF 		.word	-1082130432
 1835              		.cfi_endproc
 1836              	.LFE384:
 1837              		.cantunwind
 1838              		.fnend
 1840              		.text
 1841              	.Letext0:
 1842              		.file 4 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdint"
 1843              		.file 5 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cmath"
 1844              		.file 6 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thu
 1845              		.file 7 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 1846              		.file 8 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 1847              		.file 9 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdint.h"
 1848              		.file 10 "motorlib/messages.h"
 1849              		.file 11 "motorlib/fast_loop.h"
 1850              		.file 12 "motorlib/pwm.h"
 1851              		.file 13 "motorlib/encoder.h"
 1852              		.file 14 "/opt/gcc-arm-none-eabi-8-2019-q3-update/lib/gcc/arm-none-eabi/8.3.1/include/stddef.h"
 1853              		.file 15 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_types.h"
 1854              		.file 16 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/reent.h"
 1855              		.file 17 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/lock.h"
 1856              		.file 18 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/math.h"
 1857              		.file 19 "Drivers/CMSIS/Include/core_cm4.h"
 1858              		.file 20 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/system_stm32g4xx.h"
 1859              		.file 21 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/stm32g474xx.h"
 1860              		.file 22 "<built-in>"
ARM GAS  /tmp/cc2LZZBV.s 			page 37


DEFINED SYMBOLS
                            *ABS*:0000000000000000 fast_loop.cpp
     /tmp/cc2LZZBV.s:18     .ccmram:0000000000000000 $t
     /tmp/cc2LZZBV.s:26     .ccmram:0000000000000000 _ZN8FastLoop6updateEv
     /tmp/cc2LZZBV.s:599    .text._ZL9get_clockv:0000000000000000 _ZL9get_clockv
     /tmp/cc2LZZBV.s:464    .ccmram:0000000000000394 $d
     /tmp/cc2LZZBV.s:472    .ccmram:00000000000003a8 $t
                .ARM.extab.ccmram:0000000000000000 $d
                .ARM.exidx.ccmram:0000000000000000 $d
     /tmp/cc2LZZBV.s:530    .text._ZNK3FOC10get_statusEP9FOCStatus:0000000000000000 $t
     /tmp/cc2LZZBV.s:537    .text._ZNK3FOC10get_statusEP9FOCStatus:0000000000000000 _ZNK3FOC10get_statusEP9FOCStatus
.ARM.exidx.text._ZNK3FOC10get_statusEP9FOCStatus:0000000000000000 $d
     /tmp/cc2LZZBV.s:593    .text._ZL9get_clockv:0000000000000000 $t
     /tmp/cc2LZZBV.s:632    .text._ZL9get_clockv:0000000000000014 $d
   .ARM.exidx.text._ZL9get_clockv:0000000000000000 $d
     /tmp/cc2LZZBV.s:639    .text._ZN8FastLoopC2ER3PWMR7Encoder:0000000000000000 $t
     /tmp/cc2LZZBV.s:646    .text._ZN8FastLoopC2ER3PWMR7Encoder:0000000000000000 _ZN8FastLoopC2ER3PWMR7Encoder
     /tmp/cc2LZZBV.s:825    .text._ZN8FastLoopC2ER3PWMR7Encoder:0000000000000158 $d
.ARM.extab.text._ZN8FastLoopC2ER3PWMR7Encoder:0000000000000000 $d
.ARM.exidx.text._ZN8FastLoopC2ER3PWMR7Encoder:0000000000000000 $d
     /tmp/cc2LZZBV.s:646    .text._ZN8FastLoopC2ER3PWMR7Encoder:0000000000000000 _ZN8FastLoopC1ER3PWMR7Encoder
     /tmp/cc2LZZBV.s:858    .text._ZN8FastLoopD2Ev:0000000000000000 $t
     /tmp/cc2LZZBV.s:865    .text._ZN8FastLoopD2Ev:0000000000000000 _ZN8FastLoopD2Ev
 .ARM.exidx.text._ZN8FastLoopD2Ev:0000000000000000 $d
     /tmp/cc2LZZBV.s:865    .text._ZN8FastLoopD2Ev:0000000000000000 _ZN8FastLoopD1Ev
     /tmp/cc2LZZBV.s:919    .text._ZN8FastLoop11maintenanceEv:0000000000000000 $t
     /tmp/cc2LZZBV.s:926    .text._ZN8FastLoop11maintenanceEv:0000000000000000 _ZN8FastLoop11maintenanceEv
.ARM.extab.text._ZN8FastLoop11maintenanceEv:0000000000000000 $d
.ARM.exidx.text._ZN8FastLoop11maintenanceEv:0000000000000000 $d
     /tmp/cc2LZZBV.s:1059   .text._ZN8FastLoop9set_paramERK13FastLoopParam:0000000000000000 $t
     /tmp/cc2LZZBV.s:1066   .text._ZN8FastLoop9set_paramERK13FastLoopParam:0000000000000000 _ZN8FastLoop9set_paramERK13FastLoopParam
     /tmp/cc2LZZBV.s:1780   .text._ZN8FastLoop14set_phase_modeEv:0000000000000000 _ZN8FastLoop14set_phase_modeEv
     /tmp/cc2LZZBV.s:1157   .text._ZN8FastLoop9set_paramERK13FastLoopParam:000000000000007c $d
.ARM.extab.text._ZN8FastLoop9set_paramERK13FastLoopParam:0000000000000000 $d
.ARM.exidx.text._ZN8FastLoop9set_paramERK13FastLoopParam:0000000000000000 $d
     /tmp/cc2LZZBV.s:1163   .text._ZN8FastLoop12voltage_modeEv:0000000000000000 $t
     /tmp/cc2LZZBV.s:1170   .text._ZN8FastLoop12voltage_modeEv:0000000000000000 _ZN8FastLoop12voltage_modeEv
.ARM.extab.text._ZN8FastLoop12voltage_modeEv:0000000000000000 $d
.ARM.exidx.text._ZN8FastLoop12voltage_modeEv:0000000000000000 $d
     /tmp/cc2LZZBV.s:1221   .text._ZN8FastLoop20zero_current_sensorsEv:0000000000000000 $t
     /tmp/cc2LZZBV.s:1228   .text._ZN8FastLoop20zero_current_sensorsEv:0000000000000000 _ZN8FastLoop20zero_current_sensorsEv
.ARM.exidx.text._ZN8FastLoop20zero_current_sensorsEv:0000000000000000 $d
     /tmp/cc2LZZBV.s:1407   .text._ZN8FastLoop15phase_lock_modeEf:0000000000000000 $t
     /tmp/cc2LZZBV.s:1414   .text._ZN8FastLoop15phase_lock_modeEf:0000000000000000 _ZN8FastLoop15phase_lock_modeEf
.ARM.extab.text._ZN8FastLoop15phase_lock_modeEf:0000000000000000 $d
.ARM.exidx.text._ZN8FastLoop15phase_lock_modeEf:0000000000000000 $d
     /tmp/cc2LZZBV.s:1484   .text._ZN8FastLoop12current_modeEv:0000000000000000 $t
     /tmp/cc2LZZBV.s:1491   .text._ZN8FastLoop12current_modeEv:0000000000000000 _ZN8FastLoop12current_modeEv
.ARM.extab.text._ZN8FastLoop12current_modeEv:0000000000000000 $d
.ARM.exidx.text._ZN8FastLoop12current_modeEv:0000000000000000 $d
     /tmp/cc2LZZBV.s:1564   .text._ZN8FastLoop10brake_modeEv:0000000000000000 $t
     /tmp/cc2LZZBV.s:1571   .text._ZN8FastLoop10brake_modeEv:0000000000000000 _ZN8FastLoop10brake_modeEv
.ARM.extab.text._ZN8FastLoop10brake_modeEv:0000000000000000 $d
.ARM.exidx.text._ZN8FastLoop10brake_modeEv:0000000000000000 $d
     /tmp/cc2LZZBV.s:1616   .text._ZN8FastLoop9open_modeEv:0000000000000000 $t
     /tmp/cc2LZZBV.s:1623   .text._ZN8FastLoop9open_modeEv:0000000000000000 _ZN8FastLoop9open_modeEv
.ARM.extab.text._ZN8FastLoop9open_modeEv:0000000000000000 $d
ARM GAS  /tmp/cc2LZZBV.s 			page 38


.ARM.exidx.text._ZN8FastLoop9open_modeEv:0000000000000000 $d
     /tmp/cc2LZZBV.s:1668   .text._ZN8FastLoop10get_statusEP14FastLoopStatus:0000000000000000 $t
     /tmp/cc2LZZBV.s:1675   .text._ZN8FastLoop10get_statusEP14FastLoopStatus:0000000000000000 _ZN8FastLoop10get_statusEP14FastLoopStatus
.ARM.exidx.text._ZN8FastLoop10get_statusEP14FastLoopStatus:0000000000000000 $d
     /tmp/cc2LZZBV.s:1773   .text._ZN8FastLoop14set_phase_modeEv:0000000000000000 $t
     /tmp/cc2LZZBV.s:1834   .text._ZN8FastLoop14set_phase_modeEv:0000000000000030 $d
.ARM.exidx.text._ZN8FastLoop14set_phase_modeEv:0000000000000000 $d

UNDEFINED SYMBOLS
_ZN3FOC4stepERK10FOCCommand
_ZN3PWM11set_voltageEPf
__aeabi_unwind_cpp_pr1
_Znwj
_ZN3FOCC1Ev
_ZdlPv
__cxa_end_cleanup
__gxx_personality_v0
_ZN3FOCD1Ev
_ZN3PWM8set_vbusEf
_ZN3FOC9set_paramERK8FOCParam
memcpy
_ZN3PWM12voltage_modeEv
_ZN3PWM10brake_modeEv
_ZN3PWM9open_modeEv
