ARM GAS  /tmp/cclhizz5.s 			page 1


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
  13              		.file	"foc.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.global	__aeabi_f2d
  18              		.global	__aeabi_dsub
  19              		.global	__aeabi_dmul
  20              		.global	__aeabi_d2f
  21              		.section	.ccmram,"ax",%progbits
  22              		.align	1
  23              		.global	_ZN3FOC4stepERK10FOCCommand
  24              		.arch armv7e-m
  25              		.syntax unified
  26              		.thumb
  27              		.thumb_func
  28              		.fpu fpv4-sp-d16
  30              	_ZN3FOC4stepERK10FOCCommand:
  31              		.fnstart
  32              	.LFB372:
  33              		.file 1 "motorlib/foc.cpp"
   1:motorlib/foc.cpp **** #include "foc.h"
   2:motorlib/foc.cpp **** //#include "hal_fun.h"
   3:motorlib/foc.cpp **** //#include "hal_adc.h"
   4:motorlib/foc.cpp **** 
   5:motorlib/foc.cpp **** //#include "hal_pwm.h"
   6:motorlib/foc.cpp **** //#include "../sensor/encoder.h"
   7:motorlib/foc.cpp **** #include "control_fun.h"
   8:motorlib/foc.cpp **** #include "sincos.h"
   9:motorlib/foc.cpp **** 
  10:motorlib/foc.cpp **** #include <cmath>
  11:motorlib/foc.cpp **** 
  12:motorlib/foc.cpp **** FOC::FOC() { //hal::PWM *pwm, const hal::ADC &adc, const Encoder &encoder) : pwm_(pwm), adc_(adc), 
  13:motorlib/foc.cpp ****     pi_id_ = new PIController();
  14:motorlib/foc.cpp ****     pi_iq_ = new PIController();
  15:motorlib/foc.cpp **** }
  16:motorlib/foc.cpp **** 
  17:motorlib/foc.cpp **** FOC::~FOC() {
  18:motorlib/foc.cpp ****     delete pi_id_;
  19:motorlib/foc.cpp ****     delete pi_iq_;
  20:motorlib/foc.cpp **** }
  21:motorlib/foc.cpp **** 
  22:motorlib/foc.cpp **** #define SQRT3 (float) std::sqrt(3)
  23:motorlib/foc.cpp **** const float FOC::Kc[2][3] = {{2.0/3, -1.0/3, -1.0/3},
  24:motorlib/foc.cpp ****                                {0,      1/SQRT3, -1/SQRT3}};
  25:motorlib/foc.cpp **** 
ARM GAS  /tmp/cclhizz5.s 			page 2


  26:motorlib/foc.cpp **** 
  27:motorlib/foc.cpp **** #include "../st_device.h"
  28:motorlib/foc.cpp **** 
  29:motorlib/foc.cpp **** float alpha_current = .01;
  30:motorlib/foc.cpp **** float id_last = 0, iq_last=0;
  31:motorlib/foc.cpp **** float alpha_current2 = 1;
  32:motorlib/foc.cpp **** float ial_last = 0, ibe_last=0;
  33:motorlib/foc.cpp **** FOCStatus * const FOC::step(const FOCCommand &command) {
  34              		.loc 1 33 56
  35              		.cfi_startproc
  36              		@ args = 0, pretend = 0, frame = 88
  37              		@ frame_needed = 1, uses_anonymous_args = 0
  38 0000 B0B5     		push	{r4, r5, r7, lr}
  39              		.save {r4, r5, r7, lr}
  40              	.LCFI0:
  41              		.cfi_def_cfa_offset 16
  42              		.cfi_offset 4, -16
  43              		.cfi_offset 5, -12
  44              		.cfi_offset 7, -8
  45              		.cfi_offset 14, -4
  46              		.pad #88
  47 0002 96B0     		sub	sp, sp, #88
  48              	.LCFI1:
  49              		.cfi_def_cfa_offset 104
  50              		.setfp r7, sp, #0
  51 0004 00AF     		add	r7, sp, #0
  52              	.LCFI2:
  53              		.cfi_def_cfa_register 7
  54 0006 7860     		str	r0, [r7, #4]
  55 0008 3960     		str	r1, [r7]
  34:motorlib/foc.cpp ****     status_.measured.position = command.measured.motor_encoder;
  56              		.loc 1 34 50
  57 000a 3B68     		ldr	r3, [r7]
  58 000c 5A69     		ldr	r2, [r3, #20]	@ float
  59              		.loc 1 34 31
  60 000e 7B68     		ldr	r3, [r7, #4]
  61 0010 5A61     		str	r2, [r3, #20]	@ float
  35:motorlib/foc.cpp ****     status_.desired.i_d = command.desired.i_d;
  62              		.loc 1 35 43
  63 0012 3B68     		ldr	r3, [r7]
  64 0014 1A68     		ldr	r2, [r3]	@ float
  65              		.loc 1 35 25
  66 0016 7B68     		ldr	r3, [r7, #4]
  67 0018 DA60     		str	r2, [r3, #12]	@ float
  36:motorlib/foc.cpp ****     status_.desired.i_q = command.desired.i_q;
  68              		.loc 1 36 43
  69 001a 3B68     		ldr	r3, [r7]
  70 001c 5A68     		ldr	r2, [r3, #4]	@ float
  71              		.loc 1 36 25
  72 001e 7B68     		ldr	r3, [r7, #4]
  73 0020 1A61     		str	r2, [r3, #16]	@ float
  37:motorlib/foc.cpp ****     float i_abc_measured[3] = {command.measured.i_a, command.measured.i_b, command.measured.i_c};
  74              		.loc 1 37 49
  75 0022 3B68     		ldr	r3, [r7]
  76 0024 9B68     		ldr	r3, [r3, #8]	@ float
  77              		.loc 1 37 96
  78 0026 7B61     		str	r3, [r7, #20]	@ float
ARM GAS  /tmp/cclhizz5.s 			page 3


  79              		.loc 1 37 71
  80 0028 3B68     		ldr	r3, [r7]
  81 002a DB68     		ldr	r3, [r3, #12]	@ float
  82              		.loc 1 37 96
  83 002c BB61     		str	r3, [r7, #24]	@ float
  84              		.loc 1 37 93
  85 002e 3B68     		ldr	r3, [r7]
  86 0030 1B69     		ldr	r3, [r3, #16]	@ float
  87              		.loc 1 37 96
  88 0032 FB61     		str	r3, [r7, #28]	@ float
  38:motorlib/foc.cpp ****     float electrical_angle = status_.measured.position * num_poles_;
  89              		.loc 1 38 47
  90 0034 7B68     		ldr	r3, [r7, #4]
  91 0036 93ED057A 		vldr.32	s14, [r3, #20]
  92              		.loc 1 38 58
  93 003a 7B68     		ldr	r3, [r7, #4]
  94 003c 1B88     		ldrh	r3, [r3]
  95 003e 07EE903A 		vmov	s15, r3	@ int
  96              		.loc 1 38 56
  97 0042 F8EEE77A 		vcvt.f32.s32	s15, s15
  98              		.loc 1 38 11
  99 0046 67EE277A 		vmul.f32	s15, s14, s15
 100 004a C7ED157A 		vstr.32	s15, [r7, #84]
  39:motorlib/foc.cpp **** 
  40:motorlib/foc.cpp ****     //float sin_t = std::sin(electrical_angle);
  41:motorlib/foc.cpp ****     //float cos_t = std::cos(electrical_angle);
  42:motorlib/foc.cpp **** 
  43:motorlib/foc.cpp ****     Sincos sincos = sincos1(electrical_angle);
 101              		.loc 1 43 45
 102 004e 97ED150A 		vldr.32	s0, [r7, #84]
 103 0052 FFF7FEFF 		bl	_Z7sincos1f
 104 0056 B0EE407A 		vmov.f32	s14, s0
 105 005a F0EE607A 		vmov.f32	s15, s1
 106 005e 87ED037A 		vstr.32	s14, [r7, #12]
 107 0062 C7ED047A 		vstr.32	s15, [r7, #16]
  44:motorlib/foc.cpp ****     float &sin_t = sincos.sin;
 108              		.loc 1 44 12
 109 0066 07F10C03 		add	r3, r7, #12
 110 006a 3B65     		str	r3, [r7, #80]
  45:motorlib/foc.cpp ****     float &cos_t = sincos.cos;
 111              		.loc 1 45 12
 112 006c 07F10C03 		add	r3, r7, #12
 113 0070 0433     		adds	r3, r3, #4
 114 0072 FB64     		str	r3, [r7, #76]
  46:motorlib/foc.cpp ****     float  i_alpha_measured = Kc[0][0] * i_abc_measured[0] +
 115              		.loc 1 46 38
 116 0074 9FEDB27A 		vldr.32	s14, .L3
 117              		.loc 1 46 58
 118 0078 D7ED057A 		vldr.32	s15, [r7, #20]
 119              		.loc 1 46 40
 120 007c 27EE277A 		vmul.f32	s14, s14, s15
  47:motorlib/foc.cpp ****             Kc[0][1] * i_abc_measured[1] +
 121              		.loc 1 47 20
 122 0080 DFEDB06A 		vldr.32	s13, .L3+4
 123              		.loc 1 47 40
 124 0084 D7ED067A 		vldr.32	s15, [r7, #24]
 125              		.loc 1 47 22
ARM GAS  /tmp/cclhizz5.s 			page 4


 126 0088 66EEA77A 		vmul.f32	s15, s13, s15
  46:motorlib/foc.cpp ****     float  i_alpha_measured = Kc[0][0] * i_abc_measured[0] +
 127              		.loc 1 46 60
 128 008c 37EE277A 		vadd.f32	s14, s14, s15
  48:motorlib/foc.cpp ****             Kc[0][2] * i_abc_measured[2];
 129              		.loc 1 48 20
 130 0090 DFEDAC6A 		vldr.32	s13, .L3+4
 131              		.loc 1 48 40
 132 0094 D7ED077A 		vldr.32	s15, [r7, #28]
 133              		.loc 1 48 22
 134 0098 66EEA77A 		vmul.f32	s15, s13, s15
  46:motorlib/foc.cpp ****     float  i_alpha_measured = Kc[0][0] * i_abc_measured[0] +
 135              		.loc 1 46 12
 136 009c 77EE277A 		vadd.f32	s15, s14, s15
 137 00a0 C7ED127A 		vstr.32	s15, [r7, #72]
  49:motorlib/foc.cpp ****     float  i_beta_measured = Kc[1][0] * i_abc_measured[0] +
 138              		.loc 1 49 37
 139 00a4 9FEDA87A 		vldr.32	s14, .L3+8
 140              		.loc 1 49 57
 141 00a8 D7ED057A 		vldr.32	s15, [r7, #20]
 142              		.loc 1 49 39
 143 00ac 27EE277A 		vmul.f32	s14, s14, s15
  50:motorlib/foc.cpp ****                      Kc[1][1] * i_abc_measured[1] +
 144              		.loc 1 50 29
 145 00b0 DFEDA66A 		vldr.32	s13, .L3+12
 146              		.loc 1 50 49
 147 00b4 D7ED067A 		vldr.32	s15, [r7, #24]
 148              		.loc 1 50 31
 149 00b8 66EEA77A 		vmul.f32	s15, s13, s15
  49:motorlib/foc.cpp ****     float  i_beta_measured = Kc[1][0] * i_abc_measured[0] +
 150              		.loc 1 49 59
 151 00bc 37EE277A 		vadd.f32	s14, s14, s15
  51:motorlib/foc.cpp ****                      Kc[1][2] * i_abc_measured[2];
 152              		.loc 1 51 29
 153 00c0 DFEDA36A 		vldr.32	s13, .L3+16
 154              		.loc 1 51 49
 155 00c4 D7ED077A 		vldr.32	s15, [r7, #28]
 156              		.loc 1 51 31
 157 00c8 66EEA77A 		vmul.f32	s15, s13, s15
  49:motorlib/foc.cpp ****     float  i_beta_measured = Kc[1][0] * i_abc_measured[0] +
 158              		.loc 1 49 12
 159 00cc 77EE277A 		vadd.f32	s15, s14, s15
 160 00d0 C7ED117A 		vstr.32	s15, [r7, #68]
  52:motorlib/foc.cpp **** 
  53:motorlib/foc.cpp ****     i_alpha_measured = alpha_current2*i_alpha_measured + (1-alpha_current2)*ial_last;
 161              		.loc 1 53 38
 162 00d4 9F4B     		ldr	r3, .L3+20
 163 00d6 93ED007A 		vldr.32	s14, [r3]
 164 00da D7ED127A 		vldr.32	s15, [r7, #72]
 165 00de 27EE277A 		vmul.f32	s14, s14, s15
 166              		.loc 1 53 60
 167 00e2 9C4B     		ldr	r3, .L3+20
 168 00e4 D3ED007A 		vldr.32	s15, [r3]
 169 00e8 F7EE006A 		vmov.f32	s13, #1.0e+0
 170 00ec 76EEE76A 		vsub.f32	s13, s13, s15
 171              		.loc 1 53 76
 172 00f0 994B     		ldr	r3, .L3+24
ARM GAS  /tmp/cclhizz5.s 			page 5


 173 00f2 D3ED007A 		vldr.32	s15, [r3]
 174 00f6 66EEA77A 		vmul.f32	s15, s13, s15
 175              		.loc 1 53 22
 176 00fa 77EE277A 		vadd.f32	s15, s14, s15
 177 00fe C7ED127A 		vstr.32	s15, [r7, #72]
  54:motorlib/foc.cpp ****     ial_last = i_alpha_measured;
 178              		.loc 1 54 14
 179 0102 954A     		ldr	r2, .L3+24
 180 0104 BB6C     		ldr	r3, [r7, #72]	@ float
 181 0106 1360     		str	r3, [r2]	@ float
  55:motorlib/foc.cpp ****     i_beta_measured = alpha_current2*i_beta_measured + (1-alpha_current2)*ibe_last;
 182              		.loc 1 55 37
 183 0108 924B     		ldr	r3, .L3+20
 184 010a 93ED007A 		vldr.32	s14, [r3]
 185 010e D7ED117A 		vldr.32	s15, [r7, #68]
 186 0112 27EE277A 		vmul.f32	s14, s14, s15
 187              		.loc 1 55 58
 188 0116 8F4B     		ldr	r3, .L3+20
 189 0118 D3ED007A 		vldr.32	s15, [r3]
 190 011c F7EE006A 		vmov.f32	s13, #1.0e+0
 191 0120 76EEE76A 		vsub.f32	s13, s13, s15
 192              		.loc 1 55 74
 193 0124 8D4B     		ldr	r3, .L3+28
 194 0126 D3ED007A 		vldr.32	s15, [r3]
 195 012a 66EEA77A 		vmul.f32	s15, s13, s15
 196              		.loc 1 55 21
 197 012e 77EE277A 		vadd.f32	s15, s14, s15
 198 0132 C7ED117A 		vstr.32	s15, [r7, #68]
  56:motorlib/foc.cpp ****     ibe_last = i_beta_measured;
 199              		.loc 1 56 14
 200 0136 894A     		ldr	r2, .L3+28
 201 0138 7B6C     		ldr	r3, [r7, #68]	@ float
 202 013a 1360     		str	r3, [r2]	@ float
  57:motorlib/foc.cpp **** 
  58:motorlib/foc.cpp ****     float i_d_measured = cos_t * i_alpha_measured - sin_t * i_beta_measured;
 203              		.loc 1 58 26
 204 013c FB6C     		ldr	r3, [r7, #76]
 205 013e 93ED007A 		vldr.32	s14, [r3]
 206              		.loc 1 58 32
 207 0142 D7ED127A 		vldr.32	s15, [r7, #72]
 208 0146 27EE277A 		vmul.f32	s14, s14, s15
 209              		.loc 1 58 53
 210 014a 3B6D     		ldr	r3, [r7, #80]
 211 014c D3ED006A 		vldr.32	s13, [r3]
 212              		.loc 1 58 59
 213 0150 D7ED117A 		vldr.32	s15, [r7, #68]
 214 0154 66EEA77A 		vmul.f32	s15, s13, s15
 215              		.loc 1 58 11
 216 0158 77EE677A 		vsub.f32	s15, s14, s15
 217 015c C7ED107A 		vstr.32	s15, [r7, #64]
  59:motorlib/foc.cpp ****     float i_q_measured = sin_t * i_alpha_measured + cos_t * i_beta_measured;
 218              		.loc 1 59 26
 219 0160 3B6D     		ldr	r3, [r7, #80]
 220 0162 93ED007A 		vldr.32	s14, [r3]
 221              		.loc 1 59 32
 222 0166 D7ED127A 		vldr.32	s15, [r7, #72]
 223 016a 27EE277A 		vmul.f32	s14, s14, s15
ARM GAS  /tmp/cclhizz5.s 			page 6


 224              		.loc 1 59 53
 225 016e FB6C     		ldr	r3, [r7, #76]
 226 0170 D3ED006A 		vldr.32	s13, [r3]
 227              		.loc 1 59 59
 228 0174 D7ED117A 		vldr.32	s15, [r7, #68]
 229 0178 66EEA77A 		vmul.f32	s15, s13, s15
 230              		.loc 1 59 11
 231 017c 77EE277A 		vadd.f32	s15, s14, s15
 232 0180 C7ED0F7A 		vstr.32	s15, [r7, #60]
  60:motorlib/foc.cpp **** 
  61:motorlib/foc.cpp ****     i_d_measured = alpha_current*i_d_measured + (1-alpha_current)*id_last;
 233              		.loc 1 61 33
 234 0184 764B     		ldr	r3, .L3+32
 235 0186 93ED007A 		vldr.32	s14, [r3]
 236 018a D7ED107A 		vldr.32	s15, [r7, #64]
 237 018e 27EE277A 		vmul.f32	s14, s14, s15
 238              		.loc 1 61 51
 239 0192 734B     		ldr	r3, .L3+32
 240 0194 D3ED007A 		vldr.32	s15, [r3]
 241 0198 F7EE006A 		vmov.f32	s13, #1.0e+0
 242 019c 76EEE76A 		vsub.f32	s13, s13, s15
 243              		.loc 1 61 66
 244 01a0 704B     		ldr	r3, .L3+36
 245 01a2 D3ED007A 		vldr.32	s15, [r3]
 246 01a6 66EEA77A 		vmul.f32	s15, s13, s15
 247              		.loc 1 61 18
 248 01aa 77EE277A 		vadd.f32	s15, s14, s15
 249 01ae C7ED107A 		vstr.32	s15, [r7, #64]
  62:motorlib/foc.cpp ****     id_last = i_d_measured;
 250              		.loc 1 62 13
 251 01b2 6C4A     		ldr	r2, .L3+36
 252 01b4 3B6C     		ldr	r3, [r7, #64]	@ float
 253 01b6 1360     		str	r3, [r2]	@ float
  63:motorlib/foc.cpp ****     i_q_measured = alpha_current*i_q_measured + (1-alpha_current)*iq_last;
 254              		.loc 1 63 33
 255 01b8 694B     		ldr	r3, .L3+32
 256 01ba 93ED007A 		vldr.32	s14, [r3]
 257 01be D7ED0F7A 		vldr.32	s15, [r7, #60]
 258 01c2 27EE277A 		vmul.f32	s14, s14, s15
 259              		.loc 1 63 51
 260 01c6 664B     		ldr	r3, .L3+32
 261 01c8 D3ED007A 		vldr.32	s15, [r3]
 262 01cc F7EE006A 		vmov.f32	s13, #1.0e+0
 263 01d0 76EEE76A 		vsub.f32	s13, s13, s15
 264              		.loc 1 63 66
 265 01d4 644B     		ldr	r3, .L3+40
 266 01d6 D3ED007A 		vldr.32	s15, [r3]
 267 01da 66EEA77A 		vmul.f32	s15, s13, s15
 268              		.loc 1 63 18
 269 01de 77EE277A 		vadd.f32	s15, s14, s15
 270 01e2 C7ED0F7A 		vstr.32	s15, [r7, #60]
  64:motorlib/foc.cpp ****     iq_last = i_q_measured;
 271              		.loc 1 64 13
 272 01e6 604A     		ldr	r2, .L3+40
 273 01e8 FB6B     		ldr	r3, [r7, #60]	@ float
 274 01ea 1360     		str	r3, [r2]	@ float
  65:motorlib/foc.cpp **** 
ARM GAS  /tmp/cclhizz5.s 			page 7


  66:motorlib/foc.cpp ****    //float v_d_desired = pi_id_->step(status_.desired.i_d, i_d_measured);
  67:motorlib/foc.cpp ****    //float v_q_desired = pi_iq_->step(status_.desired.i_q, i_q_measured);
  68:motorlib/foc.cpp ****     // float v_d_desired = status_.desired.i_d;
  69:motorlib/foc.cpp ****     // float v_q_desired =status_.desired.i_q;
  70:motorlib/foc.cpp ****     float v_d_desired = 0;
 275              		.loc 1 70 11
 276 01ec 4FF00003 		mov	r3, #0
 277 01f0 BB63     		str	r3, [r7, #56]	@ float
  71:motorlib/foc.cpp ****     float v_q_desired = status_.desired.i_q*((float) rand()/RAND_MAX-.5);
 278              		.loc 1 71 41
 279 01f2 7B68     		ldr	r3, [r7, #4]
 280 01f4 1B69     		ldr	r3, [r3, #16]	@ float
 281 01f6 1846     		mov	r0, r3	@ float
 282 01f8 FFF7FEFF 		bl	__aeabi_f2d
 283              	.LVL0:
 284 01fc 0446     		mov	r4, r0
 285 01fe 0D46     		mov	r5, r1
 286              		.loc 1 71 58
 287 0200 FFF7FEFF 		bl	rand
 288 0204 07EE900A 		vmov	s15, r0	@ int
 289              		.loc 1 71 46
 290 0208 F8EEE77A 		vcvt.f32.s32	s15, s15
 291              		.loc 1 71 60
 292 020c 9FED577A 		vldr.32	s14, .L3+44
 293 0210 C7EE876A 		vdiv.f32	s13, s15, s14
 294 0214 16EE900A 		vmov	r0, s13
 295 0218 FFF7FEFF 		bl	__aeabi_f2d
 296              	.LVL1:
 297              		.loc 1 71 69
 298 021c 4FF00002 		mov	r2, #0
 299 0220 534B     		ldr	r3, .L3+48
 300 0222 FFF7FEFF 		bl	__aeabi_dsub
 301              	.LVL2:
 302 0226 0246     		mov	r2, r0
 303 0228 0B46     		mov	r3, r1
 304              		.loc 1 71 44
 305 022a 2046     		mov	r0, r4
 306 022c 2946     		mov	r1, r5
 307 022e FFF7FEFF 		bl	__aeabi_dmul
 308              	.LVL3:
 309 0232 0346     		mov	r3, r0
 310 0234 0C46     		mov	r4, r1
 311              		.loc 1 71 72
 312 0236 1846     		mov	r0, r3
 313 0238 2146     		mov	r1, r4
 314 023a FFF7FEFF 		bl	__aeabi_d2f
 315              	.LVL4:
 316 023e 0346     		mov	r3, r0	@ float
 317 0240 7B63     		str	r3, [r7, #52]	@ float
  72:motorlib/foc.cpp **** 
  73:motorlib/foc.cpp ****     float v_alpha_desired = cos_t * v_d_desired + sin_t * v_q_desired;
 318              		.loc 1 73 29
 319 0242 FB6C     		ldr	r3, [r7, #76]
 320 0244 93ED007A 		vldr.32	s14, [r3]
 321              		.loc 1 73 35
 322 0248 D7ED0E7A 		vldr.32	s15, [r7, #56]
 323 024c 27EE277A 		vmul.f32	s14, s14, s15
ARM GAS  /tmp/cclhizz5.s 			page 8


 324              		.loc 1 73 51
 325 0250 3B6D     		ldr	r3, [r7, #80]
 326 0252 D3ED006A 		vldr.32	s13, [r3]
 327              		.loc 1 73 57
 328 0256 D7ED0D7A 		vldr.32	s15, [r7, #52]
 329 025a 66EEA77A 		vmul.f32	s15, s13, s15
 330              		.loc 1 73 11
 331 025e 77EE277A 		vadd.f32	s15, s14, s15
 332 0262 C7ED0C7A 		vstr.32	s15, [r7, #48]
  74:motorlib/foc.cpp ****     float v_beta_desired = -sin_t * v_d_desired + cos_t * v_q_desired;
 333              		.loc 1 74 28
 334 0266 3B6D     		ldr	r3, [r7, #80]
 335 0268 D3ED007A 		vldr.32	s15, [r3]
 336 026c B1EE677A 		vneg.f32	s14, s15
 337              		.loc 1 74 35
 338 0270 D7ED0E7A 		vldr.32	s15, [r7, #56]
 339 0274 27EE277A 		vmul.f32	s14, s14, s15
 340              		.loc 1 74 51
 341 0278 FB6C     		ldr	r3, [r7, #76]
 342 027a D3ED006A 		vldr.32	s13, [r3]
 343              		.loc 1 74 57
 344 027e D7ED0D7A 		vldr.32	s15, [r7, #52]
 345 0282 66EEA77A 		vmul.f32	s15, s13, s15
 346              		.loc 1 74 11
 347 0286 77EE277A 		vadd.f32	s15, s14, s15
 348 028a C7ED0B7A 		vstr.32	s15, [r7, #44]
  75:motorlib/foc.cpp **** 
  76:motorlib/foc.cpp ****     float v_a_desired = Kc[0][0] * v_alpha_desired + Kc[1][0] * v_beta_desired;
 349              		.loc 1 76 32
 350 028e 9FED2C7A 		vldr.32	s14, .L3
 351              		.loc 1 76 34
 352 0292 D7ED0C7A 		vldr.32	s15, [r7, #48]
 353 0296 27EE277A 		vmul.f32	s14, s14, s15
 354              		.loc 1 76 61
 355 029a DFED2B6A 		vldr.32	s13, .L3+8
 356              		.loc 1 76 63
 357 029e D7ED0B7A 		vldr.32	s15, [r7, #44]
 358 02a2 66EEA77A 		vmul.f32	s15, s13, s15
 359              		.loc 1 76 11
 360 02a6 77EE277A 		vadd.f32	s15, s14, s15
 361 02aa C7ED0A7A 		vstr.32	s15, [r7, #40]
  77:motorlib/foc.cpp ****     float v_b_desired = Kc[0][1] * v_alpha_desired + Kc[1][1] * v_beta_desired;
 362              		.loc 1 77 32
 363 02ae 9FED257A 		vldr.32	s14, .L3+4
 364              		.loc 1 77 34
 365 02b2 D7ED0C7A 		vldr.32	s15, [r7, #48]
 366 02b6 27EE277A 		vmul.f32	s14, s14, s15
 367              		.loc 1 77 61
 368 02ba DFED246A 		vldr.32	s13, .L3+12
 369              		.loc 1 77 63
 370 02be D7ED0B7A 		vldr.32	s15, [r7, #44]
 371 02c2 66EEA77A 		vmul.f32	s15, s13, s15
 372              		.loc 1 77 11
 373 02c6 77EE277A 		vadd.f32	s15, s14, s15
 374 02ca C7ED097A 		vstr.32	s15, [r7, #36]
  78:motorlib/foc.cpp ****     float v_c_desired = Kc[0][2] * v_alpha_desired + Kc[1][2] * v_beta_desired;
 375              		.loc 1 78 32
ARM GAS  /tmp/cclhizz5.s 			page 9


 376 02ce 9FED1D7A 		vldr.32	s14, .L3+4
 377              		.loc 1 78 34
 378 02d2 D7ED0C7A 		vldr.32	s15, [r7, #48]
 379 02d6 27EE277A 		vmul.f32	s14, s14, s15
 380              		.loc 1 78 61
 381 02da DFED1D6A 		vldr.32	s13, .L3+16
 382              		.loc 1 78 63
 383 02de D7ED0B7A 		vldr.32	s15, [r7, #44]
 384 02e2 66EEA77A 		vmul.f32	s15, s13, s15
 385              		.loc 1 78 11
 386 02e6 77EE277A 		vadd.f32	s15, s14, s15
 387 02ea C7ED087A 		vstr.32	s15, [r7, #32]
  79:motorlib/foc.cpp **** 
  80:motorlib/foc.cpp ****     status_.command.v_a = v_a_desired;
 388              		.loc 1 80 25
 389 02ee 7B68     		ldr	r3, [r7, #4]
 390 02f0 BA6A     		ldr	r2, [r7, #40]	@ float
 391 02f2 5A62     		str	r2, [r3, #36]	@ float
  81:motorlib/foc.cpp ****     status_.command.v_b = v_b_desired;
 392              		.loc 1 81 25
 393 02f4 7B68     		ldr	r3, [r7, #4]
 394 02f6 7A6A     		ldr	r2, [r7, #36]	@ float
 395 02f8 9A62     		str	r2, [r3, #40]	@ float
  82:motorlib/foc.cpp ****     status_.command.v_c = v_c_desired;
 396              		.loc 1 82 25
 397 02fa 7B68     		ldr	r3, [r7, #4]
 398 02fc 3A6A     		ldr	r2, [r7, #32]	@ float
 399 02fe DA62     		str	r2, [r3, #44]	@ float
  83:motorlib/foc.cpp ****     status_.command.v_d = v_d_desired;
 400              		.loc 1 83 25
 401 0300 7B68     		ldr	r3, [r7, #4]
 402 0302 BA6B     		ldr	r2, [r7, #56]	@ float
 403 0304 1A63     		str	r2, [r3, #48]	@ float
  84:motorlib/foc.cpp ****     status_.command.v_q = v_q_desired;
 404              		.loc 1 84 25
 405 0306 7B68     		ldr	r3, [r7, #4]
 406 0308 7A6B     		ldr	r2, [r7, #52]	@ float
 407 030a 5A63     		str	r2, [r3, #52]	@ float
  85:motorlib/foc.cpp ****     status_.measured.i_d = i_d_measured;
 408              		.loc 1 85 26
 409 030c 7B68     		ldr	r3, [r7, #4]
 410 030e 3A6C     		ldr	r2, [r7, #64]	@ float
 411 0310 9A61     		str	r2, [r3, #24]	@ float
  86:motorlib/foc.cpp ****     status_.measured.i_q = i_q_measured;
 412              		.loc 1 86 26
 413 0312 7B68     		ldr	r3, [r7, #4]
 414 0314 FA6B     		ldr	r2, [r7, #60]	@ float
 415 0316 DA61     		str	r2, [r3, #28]	@ float
  87:motorlib/foc.cpp ****     status_.measured.i_0 = i_abc_measured[0] + i_abc_measured[1] + i_abc_measured[2];
 416              		.loc 1 87 44
 417 0318 97ED057A 		vldr.32	s14, [r7, #20]
 418              		.loc 1 87 64
 419 031c D7ED067A 		vldr.32	s15, [r7, #24]
 420              		.loc 1 87 46
 421 0320 37EE277A 		vadd.f32	s14, s14, s15
 422              		.loc 1 87 84
 423 0324 D7ED077A 		vldr.32	s15, [r7, #28]
ARM GAS  /tmp/cclhizz5.s 			page 10


 424              		.loc 1 87 66
 425 0328 77EE277A 		vadd.f32	s15, s14, s15
 426              		.loc 1 87 26
 427 032c 7B68     		ldr	r3, [r7, #4]
 428 032e C3ED087A 		vstr.32	s15, [r3, #32]
  88:motorlib/foc.cpp ****     return &status_;
 429              		.loc 1 88 13
 430 0332 7B68     		ldr	r3, [r7, #4]
 431 0334 0C33     		adds	r3, r3, #12
  89:motorlib/foc.cpp **** }
 432              		.loc 1 89 1
 433 0336 1846     		mov	r0, r3
 434 0338 5837     		adds	r7, r7, #88
 435              	.LCFI3:
 436              		.cfi_def_cfa_offset 16
 437 033a BD46     		mov	sp, r7
 438              	.LCFI4:
 439              		.cfi_def_cfa_register 13
 440              		@ sp needed
 441 033c B0BD     		pop	{r4, r5, r7, pc}
 442              	.L4:
 443 033e 00BF     		.align	2
 444              	.L3:
 445 0340 ABAA2A3F 		.word	1059760811
 446 0344 ABAAAABE 		.word	3198855851
 447 0348 00000000 		.word	0
 448 034c 3ACD133F 		.word	1058262330
 449 0350 3ACD13BF 		.word	3205745978
 450 0354 00000000 		.word	alpha_current2
 451 0358 00000000 		.word	ial_last
 452 035c 00000000 		.word	ibe_last
 453 0360 00000000 		.word	alpha_current
 454 0364 00000000 		.word	id_last
 455 0368 00000000 		.word	iq_last
 456 036c 0000004F 		.word	1325400064
 457 0370 0000E03F 		.word	1071644672
 458              		.cfi_endproc
 459              	.LFE372:
 460              		.fnend
 462              		.section	.text._ZN12PIControllerD2Ev,"axG",%progbits,_ZN12PIControllerD5Ev,comdat
 463              		.align	1
 464              		.weak	_ZN12PIControllerD2Ev
 465              		.syntax unified
 466              		.thumb
 467              		.thumb_func
 468              		.fpu fpv4-sp-d16
 470              	_ZN12PIControllerD2Ev:
 471              		.fnstart
 472              	.LFB2:
 473              		.file 2 "motorlib/control_fun.h"
   1:motorlib/control_fun.h **** #ifndef MOTOR_CONTROL_FUN_H
   2:motorlib/control_fun.h **** #define MOTOR_CONTROL_FUN_H
   3:motorlib/control_fun.h **** 
   4:motorlib/control_fun.h **** #include "messages.h"
   5:motorlib/control_fun.h **** 
   6:motorlib/control_fun.h **** class PIController {
   7:motorlib/control_fun.h **** public:
ARM GAS  /tmp/cclhizz5.s 			page 11


   8:motorlib/control_fun.h ****     ~PIController() {}
 474              		.loc 2 8 5
 475              		.cfi_startproc
 476              		@ args = 0, pretend = 0, frame = 8
 477              		@ frame_needed = 1, uses_anonymous_args = 0
 478              		@ link register save eliminated.
 479 0000 80B4     		push	{r7}
 480              	.LCFI5:
 481              		.cfi_def_cfa_offset 4
 482              		.cfi_offset 7, -4
 483 0002 83B0     		sub	sp, sp, #12
 484              	.LCFI6:
 485              		.cfi_def_cfa_offset 16
 486 0004 00AF     		add	r7, sp, #0
 487              	.LCFI7:
 488              		.cfi_def_cfa_register 7
 489 0006 7860     		str	r0, [r7, #4]
 490              		.loc 2 8 22
 491 0008 7B68     		ldr	r3, [r7, #4]
 492 000a 1846     		mov	r0, r3
 493 000c 0C37     		adds	r7, r7, #12
 494              	.LCFI8:
 495              		.cfi_def_cfa_offset 4
 496 000e BD46     		mov	sp, r7
 497              	.LCFI9:
 498              		.cfi_def_cfa_register 13
 499              		@ sp needed
 500 0010 5DF8047B 		ldr	r7, [sp], #4
 501              	.LCFI10:
 502              		.cfi_restore 7
 503              		.cfi_def_cfa_offset 0
 504 0014 7047     		bx	lr
 505              		.cfi_endproc
 506              	.LFE2:
 507              		.cantunwind
 508              		.fnend
 510              		.weak	_ZN12PIControllerD1Ev
 511              		.thumb_set _ZN12PIControllerD1Ev,_ZN12PIControllerD2Ev
 512              		.section	.text._ZN12PIControllerC2Ev,"axG",%progbits,_ZN12PIControllerC5Ev,comdat
 513              		.align	1
 514              		.weak	_ZN12PIControllerC2Ev
 515              		.syntax unified
 516              		.thumb
 517              		.thumb_func
 518              		.fpu fpv4-sp-d16
 520              	_ZN12PIControllerC2Ev:
 521              		.fnstart
 522              	.LFB234:
   6:motorlib/control_fun.h **** public:
 523              		.loc 2 6 7
 524              		.cfi_startproc
 525              		@ args = 0, pretend = 0, frame = 8
 526              		@ frame_needed = 1, uses_anonymous_args = 0
 527              		@ link register save eliminated.
 528 0000 80B4     		push	{r7}
 529              	.LCFI11:
 530              		.cfi_def_cfa_offset 4
ARM GAS  /tmp/cclhizz5.s 			page 12


 531              		.cfi_offset 7, -4
 532 0002 83B0     		sub	sp, sp, #12
 533              	.LCFI12:
 534              		.cfi_def_cfa_offset 16
 535 0004 00AF     		add	r7, sp, #0
 536              	.LCFI13:
 537              		.cfi_def_cfa_register 7
 538 0006 7860     		str	r0, [r7, #4]
 539              	.LBB2:
   6:motorlib/control_fun.h **** public:
 540              		.loc 2 6 7
 541 0008 7B68     		ldr	r3, [r7, #4]
 542 000a 4FF00002 		mov	r2, #0
 543 000e 1A60     		str	r2, [r3]	@ float
 544 0010 7B68     		ldr	r3, [r7, #4]
 545 0012 4FF00002 		mov	r2, #0
 546 0016 5A60     		str	r2, [r3, #4]	@ float
 547 0018 7B68     		ldr	r3, [r7, #4]
 548 001a 4FF00002 		mov	r2, #0
 549 001e 9A60     		str	r2, [r3, #8]	@ float
 550 0020 7B68     		ldr	r3, [r7, #4]
 551 0022 4FF00002 		mov	r2, #0
 552 0026 DA60     		str	r2, [r3, #12]	@ float
 553 0028 7B68     		ldr	r3, [r7, #4]
 554 002a 4FF00002 		mov	r2, #0
 555 002e 1A61     		str	r2, [r3, #16]	@ float
 556              	.LBE2:
 557 0030 7B68     		ldr	r3, [r7, #4]
 558 0032 1846     		mov	r0, r3
 559 0034 0C37     		adds	r7, r7, #12
 560              	.LCFI14:
 561              		.cfi_def_cfa_offset 4
 562 0036 BD46     		mov	sp, r7
 563              	.LCFI15:
 564              		.cfi_def_cfa_register 13
 565              		@ sp needed
 566 0038 5DF8047B 		ldr	r7, [sp], #4
 567              	.LCFI16:
 568              		.cfi_restore 7
 569              		.cfi_def_cfa_offset 0
 570 003c 7047     		bx	lr
 571              		.cfi_endproc
 572              	.LFE234:
 573              		.cantunwind
 574              		.fnend
 576              		.weak	_ZN12PIControllerC1Ev
 577              		.thumb_set _ZN12PIControllerC1Ev,_ZN12PIControllerC2Ev
 578              		.section	.text._ZN3FOCC2Ev,"ax",%progbits
 579              		.align	1
 580              		.global	_ZN3FOCC2Ev
 581              		.syntax unified
 582              		.thumb
 583              		.thumb_func
 584              		.fpu fpv4-sp-d16
 586              	_ZN3FOCC2Ev:
 587              		.fnstart
 588              	.LFB236:
ARM GAS  /tmp/cclhizz5.s 			page 13


  12:motorlib/foc.cpp ****     pi_id_ = new PIController();
 589              		.loc 1 12 1
 590              		.cfi_startproc
 591              		@ args = 0, pretend = 0, frame = 8
 592              		@ frame_needed = 1, uses_anonymous_args = 0
 593 0000 90B5     		push	{r4, r7, lr}
 594              		.save {r4, r7, lr}
 595              	.LCFI17:
 596              		.cfi_def_cfa_offset 12
 597              		.cfi_offset 4, -12
 598              		.cfi_offset 7, -8
 599              		.cfi_offset 14, -4
 600              		.pad #12
 601 0002 83B0     		sub	sp, sp, #12
 602              	.LCFI18:
 603              		.cfi_def_cfa_offset 24
 604              		.setfp r7, sp, #0
 605 0004 00AF     		add	r7, sp, #0
 606              	.LCFI19:
 607              		.cfi_def_cfa_register 7
 608 0006 7860     		str	r0, [r7, #4]
 609              	.LBB3:
  12:motorlib/foc.cpp ****     pi_id_ = new PIController();
 610              		.loc 1 12 10
 611 0008 7B68     		ldr	r3, [r7, #4]
 612 000a 0722     		movs	r2, #7
 613 000c 1A80     		strh	r2, [r3]	@ movhi
  13:motorlib/foc.cpp ****     pi_iq_ = new PIController();
 614              		.loc 1 13 31
 615 000e 1420     		movs	r0, #20
 616 0010 FFF7FEFF 		bl	_Znwj
 617 0014 0346     		mov	r3, r0
 618 0016 1C46     		mov	r4, r3
 619 0018 2246     		mov	r2, r4
 620 001a 0023     		movs	r3, #0
 621 001c 1360     		str	r3, [r2]
 622 001e 5360     		str	r3, [r2, #4]
 623 0020 9360     		str	r3, [r2, #8]
 624 0022 D360     		str	r3, [r2, #12]
 625 0024 1361     		str	r3, [r2, #16]
 626 0026 2046     		mov	r0, r4
 627 0028 FFF7FEFF 		bl	_ZN12PIControllerC1Ev
  13:motorlib/foc.cpp ****     pi_iq_ = new PIController();
 628              		.loc 1 13 12
 629 002c 7B68     		ldr	r3, [r7, #4]
 630 002e 5C60     		str	r4, [r3, #4]
  14:motorlib/foc.cpp **** }
 631              		.loc 1 14 31
 632 0030 1420     		movs	r0, #20
 633 0032 FFF7FEFF 		bl	_Znwj
 634 0036 0346     		mov	r3, r0
 635 0038 1C46     		mov	r4, r3
 636 003a 2246     		mov	r2, r4
 637 003c 0023     		movs	r3, #0
 638 003e 1360     		str	r3, [r2]
 639 0040 5360     		str	r3, [r2, #4]
 640 0042 9360     		str	r3, [r2, #8]
ARM GAS  /tmp/cclhizz5.s 			page 14


 641 0044 D360     		str	r3, [r2, #12]
 642 0046 1361     		str	r3, [r2, #16]
 643 0048 2046     		mov	r0, r4
 644 004a FFF7FEFF 		bl	_ZN12PIControllerC1Ev
  14:motorlib/foc.cpp **** }
 645              		.loc 1 14 12
 646 004e 7B68     		ldr	r3, [r7, #4]
 647 0050 9C60     		str	r4, [r3, #8]
 648              	.LBE3:
  15:motorlib/foc.cpp **** 
 649              		.loc 1 15 1
 650 0052 7B68     		ldr	r3, [r7, #4]
 651 0054 1846     		mov	r0, r3
 652 0056 0C37     		adds	r7, r7, #12
 653              	.LCFI20:
 654              		.cfi_def_cfa_offset 12
 655 0058 BD46     		mov	sp, r7
 656              	.LCFI21:
 657              		.cfi_def_cfa_register 13
 658              		@ sp needed
 659 005a 90BD     		pop	{r4, r7, pc}
 660              		.cfi_endproc
 661              	.LFE236:
 662              		.fnend
 664              		.global	_ZN3FOCC1Ev
 665              		.thumb_set _ZN3FOCC1Ev,_ZN3FOCC2Ev
 666              		.section	.text._ZN3FOCD2Ev,"ax",%progbits
 667              		.align	1
 668              		.global	_ZN3FOCD2Ev
 669              		.syntax unified
 670              		.thumb
 671              		.thumb_func
 672              		.fpu fpv4-sp-d16
 674              	_ZN3FOCD2Ev:
 675              		.fnstart
 676              	.LFB239:
  17:motorlib/foc.cpp ****     delete pi_id_;
 677              		.loc 1 17 1
 678              		.cfi_startproc
 679              		@ args = 0, pretend = 0, frame = 8
 680              		@ frame_needed = 1, uses_anonymous_args = 0
 681 0000 90B5     		push	{r4, r7, lr}
 682              	.LCFI22:
 683              		.cfi_def_cfa_offset 12
 684              		.cfi_offset 4, -12
 685              		.cfi_offset 7, -8
 686              		.cfi_offset 14, -4
 687 0002 83B0     		sub	sp, sp, #12
 688              	.LCFI23:
 689              		.cfi_def_cfa_offset 24
 690 0004 00AF     		add	r7, sp, #0
 691              	.LCFI24:
 692              		.cfi_def_cfa_register 7
 693 0006 7860     		str	r0, [r7, #4]
 694              	.LBB4:
  18:motorlib/foc.cpp ****     delete pi_iq_;
 695              		.loc 1 18 12
ARM GAS  /tmp/cclhizz5.s 			page 15


 696 0008 7B68     		ldr	r3, [r7, #4]
 697 000a 5C68     		ldr	r4, [r3, #4]
  18:motorlib/foc.cpp ****     delete pi_iq_;
 698              		.loc 1 18 18
 699 000c 002C     		cmp	r4, #0
 700 000e 05D0     		beq	.L12
  18:motorlib/foc.cpp ****     delete pi_iq_;
 701              		.loc 1 18 12 discriminator 1
 702 0010 2046     		mov	r0, r4
 703 0012 FFF7FEFF 		bl	_ZN12PIControllerD1Ev
 704 0016 2046     		mov	r0, r4
 705 0018 FFF7FEFF 		bl	_ZdlPv
 706              	.L12:
  19:motorlib/foc.cpp **** }
 707              		.loc 1 19 12
 708 001c 7B68     		ldr	r3, [r7, #4]
 709 001e 9C68     		ldr	r4, [r3, #8]
  19:motorlib/foc.cpp **** }
 710              		.loc 1 19 18
 711 0020 002C     		cmp	r4, #0
 712 0022 05D0     		beq	.L13
  19:motorlib/foc.cpp **** }
 713              		.loc 1 19 12 discriminator 1
 714 0024 2046     		mov	r0, r4
 715 0026 FFF7FEFF 		bl	_ZN12PIControllerD1Ev
 716 002a 2046     		mov	r0, r4
 717 002c FFF7FEFF 		bl	_ZdlPv
 718              	.L13:
 719              	.LBE4:
  20:motorlib/foc.cpp **** 
 720              		.loc 1 20 1
 721 0030 7B68     		ldr	r3, [r7, #4]
 722 0032 1846     		mov	r0, r3
 723 0034 0C37     		adds	r7, r7, #12
 724              	.LCFI25:
 725              		.cfi_def_cfa_offset 12
 726 0036 BD46     		mov	sp, r7
 727              	.LCFI26:
 728              		.cfi_def_cfa_register 13
 729              		@ sp needed
 730 0038 90BD     		pop	{r4, r7, pc}
 731              		.cfi_endproc
 732              	.LFE239:
 733              		.cantunwind
 734              		.fnend
 736              		.global	_ZN3FOCD1Ev
 737              		.thumb_set _ZN3FOCD1Ev,_ZN3FOCD2Ev
 738              		.global	_ZN3FOC2KcE
 739              		.section	.rodata._ZN3FOC2KcE,"a"
 740              		.align	2
 743              	_ZN3FOC2KcE:
 744 0000 ABAA2A3F 		.word	1059760811
 745 0004 ABAAAABE 		.word	3198855851
 746 0008 ABAAAABE 		.word	3198855851
 747 000c 00000000 		.word	0
 748 0010 3ACD133F 		.word	1058262330
 749 0014 3ACD13BF 		.word	3205745978
ARM GAS  /tmp/cclhizz5.s 			page 16


 750              		.global	alpha_current
 751              		.section	.data.alpha_current,"aw"
 752              		.align	2
 755              	alpha_current:
 756 0000 0AD7233C 		.word	1008981770
 757              		.global	id_last
 758              		.section	.bss.id_last,"aw",%nobits
 759              		.align	2
 762              	id_last:
 763 0000 00000000 		.space	4
 764              		.global	iq_last
 765              		.section	.bss.iq_last,"aw",%nobits
 766              		.align	2
 769              	iq_last:
 770 0000 00000000 		.space	4
 771              		.global	alpha_current2
 772              		.section	.data.alpha_current2,"aw"
 773              		.align	2
 776              	alpha_current2:
 777 0000 0000803F 		.word	1065353216
 778              		.global	ial_last
 779              		.section	.bss.ial_last,"aw",%nobits
 780              		.align	2
 783              	ial_last:
 784 0000 00000000 		.space	4
 785              		.global	ibe_last
 786              		.section	.bss.ibe_last,"aw",%nobits
 787              		.align	2
 790              	ibe_last:
 791 0000 00000000 		.space	4
 792              		.section	.text._ZN3FOC9set_paramERK8FOCParam,"ax",%progbits
 793              		.align	1
 794              		.global	_ZN3FOC9set_paramERK8FOCParam
 795              		.syntax unified
 796              		.thumb
 797              		.thumb_func
 798              		.fpu fpv4-sp-d16
 800              	_ZN3FOC9set_paramERK8FOCParam:
 801              		.fnstart
 802              	.LFB373:
  90:motorlib/foc.cpp **** 
  91:motorlib/foc.cpp **** void FOC::set_param(const FOCParam &param) {
 803              		.loc 1 91 44
 804              		.cfi_startproc
 805              		@ args = 0, pretend = 0, frame = 8
 806              		@ frame_needed = 1, uses_anonymous_args = 0
 807 0000 80B5     		push	{r7, lr}
 808              		.save {r7, lr}
 809              	.LCFI27:
 810              		.cfi_def_cfa_offset 8
 811              		.cfi_offset 7, -8
 812              		.cfi_offset 14, -4
 813              		.pad #8
 814 0002 82B0     		sub	sp, sp, #8
 815              	.LCFI28:
 816              		.cfi_def_cfa_offset 16
 817              		.setfp r7, sp, #0
ARM GAS  /tmp/cclhizz5.s 			page 17


 818 0004 00AF     		add	r7, sp, #0
 819              	.LCFI29:
 820              		.cfi_def_cfa_register 7
 821 0006 7860     		str	r0, [r7, #4]
 822 0008 3960     		str	r1, [r7]
  92:motorlib/foc.cpp ****     pi_id_->set_param(param.pi_d);
 823              		.loc 1 92 5
 824 000a 7B68     		ldr	r3, [r7, #4]
 825 000c 5B68     		ldr	r3, [r3, #4]
 826              		.loc 1 92 29
 827 000e 3A68     		ldr	r2, [r7]
 828              		.loc 1 92 22
 829 0010 1146     		mov	r1, r2
 830 0012 1846     		mov	r0, r3
 831 0014 FFF7FEFF 		bl	_ZN12PIController9set_paramERK7PIParam
  93:motorlib/foc.cpp ****     pi_iq_->set_param(param.pi_q);
 832              		.loc 1 93 5
 833 0018 7B68     		ldr	r3, [r7, #4]
 834 001a 9A68     		ldr	r2, [r3, #8]
 835              		.loc 1 93 29
 836 001c 3B68     		ldr	r3, [r7]
 837 001e 1033     		adds	r3, r3, #16
 838              		.loc 1 93 22
 839 0020 1946     		mov	r1, r3
 840 0022 1046     		mov	r0, r2
 841 0024 FFF7FEFF 		bl	_ZN12PIController9set_paramERK7PIParam
  94:motorlib/foc.cpp ****     num_poles_ = param.num_poles;
 842              		.loc 1 94 24
 843 0028 3B68     		ldr	r3, [r7]
 844 002a D3ED097A 		vldr.32	s15, [r3, #36]
 845              		.loc 1 94 16
 846 002e FCEEE77A 		vcvt.u32.f32	s15, s15
 847 0032 17EE903A 		vmov	r3, s15	@ int
 848 0036 9AB2     		uxth	r2, r3
 849 0038 7B68     		ldr	r3, [r7, #4]
 850 003a 1A80     		strh	r2, [r3]	@ movhi
  95:motorlib/foc.cpp **** }...
 851              		.loc 1 95 1
 852 003c 00BF     		nop
 853 003e 0837     		adds	r7, r7, #8
 854              	.LCFI30:
 855              		.cfi_def_cfa_offset 8
 856 0040 BD46     		mov	sp, r7
 857              	.LCFI31:
 858              		.cfi_def_cfa_register 13
 859              		@ sp needed
 860 0042 80BD     		pop	{r7, pc}
 861              		.cfi_endproc
 862              	.LFE373:
 863              		.fnend
 865              		.text
 866              	.Letext0:
 867              		.file 3 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 868              		.file 4 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 869              		.file 5 "motorlib/messages.h"
 870              		.file 6 "motorlib/foc.h"
 871              		.file 7 "motorlib/sincos.h"
ARM GAS  /tmp/cclhizz5.s 			page 18


 872              		.file 8 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/cpp_type_tra
 873              		.file 9 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cmath"
 874              		.file 10 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/th
 875              		.file 11 "/opt/gcc-arm-none-eabi-8-2019-q3-update/lib/gcc/arm-none-eabi/8.3.1/include/stddef.h"
 876              		.file 12 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_types.h"
 877              		.file 13 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/reent.h"
 878              		.file 14 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/lock.h"
 879              		.file 15 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/math.h"
 880              		.file 16 "Drivers/CMSIS/Include/core_cm4.h"
 881              		.file 17 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/system_stm32g4xx.h"
 882              		.file 18 "<built-in>"
ARM GAS  /tmp/cclhizz5.s 			page 19


DEFINED SYMBOLS
                            *ABS*:0000000000000000 foc.cpp
     /tmp/cclhizz5.s:22     .ccmram:0000000000000000 $t
     /tmp/cclhizz5.s:30     .ccmram:0000000000000000 _ZN3FOC4stepERK10FOCCommand
     /tmp/cclhizz5.s:445    .ccmram:0000000000000340 $d
     /tmp/cclhizz5.s:776    .data.alpha_current2:0000000000000000 alpha_current2
     /tmp/cclhizz5.s:783    .bss.ial_last:0000000000000000 ial_last
     /tmp/cclhizz5.s:790    .bss.ibe_last:0000000000000000 ibe_last
     /tmp/cclhizz5.s:755    .data.alpha_current:0000000000000000 alpha_current
     /tmp/cclhizz5.s:762    .bss.id_last:0000000000000000 id_last
     /tmp/cclhizz5.s:769    .bss.iq_last:0000000000000000 iq_last
                .ARM.extab.ccmram:0000000000000000 $d
                .ARM.exidx.ccmram:0000000000000000 $d
     /tmp/cclhizz5.s:463    .text._ZN12PIControllerD2Ev:0000000000000000 $t
     /tmp/cclhizz5.s:470    .text._ZN12PIControllerD2Ev:0000000000000000 _ZN12PIControllerD2Ev
.ARM.exidx.text._ZN12PIControllerD2Ev:0000000000000000 $d
     /tmp/cclhizz5.s:470    .text._ZN12PIControllerD2Ev:0000000000000000 _ZN12PIControllerD1Ev
     /tmp/cclhizz5.s:513    .text._ZN12PIControllerC2Ev:0000000000000000 $t
     /tmp/cclhizz5.s:520    .text._ZN12PIControllerC2Ev:0000000000000000 _ZN12PIControllerC2Ev
.ARM.exidx.text._ZN12PIControllerC2Ev:0000000000000000 $d
     /tmp/cclhizz5.s:520    .text._ZN12PIControllerC2Ev:0000000000000000 _ZN12PIControllerC1Ev
     /tmp/cclhizz5.s:579    .text._ZN3FOCC2Ev:0000000000000000 $t
     /tmp/cclhizz5.s:586    .text._ZN3FOCC2Ev:0000000000000000 _ZN3FOCC2Ev
      .ARM.extab.text._ZN3FOCC2Ev:0000000000000000 $d
      .ARM.exidx.text._ZN3FOCC2Ev:0000000000000000 $d
     /tmp/cclhizz5.s:586    .text._ZN3FOCC2Ev:0000000000000000 _ZN3FOCC1Ev
     /tmp/cclhizz5.s:667    .text._ZN3FOCD2Ev:0000000000000000 $t
     /tmp/cclhizz5.s:674    .text._ZN3FOCD2Ev:0000000000000000 _ZN3FOCD2Ev
      .ARM.exidx.text._ZN3FOCD2Ev:0000000000000000 $d
     /tmp/cclhizz5.s:674    .text._ZN3FOCD2Ev:0000000000000000 _ZN3FOCD1Ev
     /tmp/cclhizz5.s:743    .rodata._ZN3FOC2KcE:0000000000000000 _ZN3FOC2KcE
     /tmp/cclhizz5.s:740    .rodata._ZN3FOC2KcE:0000000000000000 $d
     /tmp/cclhizz5.s:752    .data.alpha_current:0000000000000000 $d
     /tmp/cclhizz5.s:759    .bss.id_last:0000000000000000 $d
     /tmp/cclhizz5.s:766    .bss.iq_last:0000000000000000 $d
     /tmp/cclhizz5.s:773    .data.alpha_current2:0000000000000000 $d
     /tmp/cclhizz5.s:780    .bss.ial_last:0000000000000000 $d
     /tmp/cclhizz5.s:787    .bss.ibe_last:0000000000000000 $d
     /tmp/cclhizz5.s:793    .text._ZN3FOC9set_paramERK8FOCParam:0000000000000000 $t
     /tmp/cclhizz5.s:800    .text._ZN3FOC9set_paramERK8FOCParam:0000000000000000 _ZN3FOC9set_paramERK8FOCParam
.ARM.extab.text._ZN3FOC9set_paramERK8FOCParam:0000000000000000 $d
.ARM.exidx.text._ZN3FOC9set_paramERK8FOCParam:0000000000000000 $d
                           .group:0000000000000000 _ZN12PIControllerD5Ev
                           .group:0000000000000000 _ZN12PIControllerC5Ev

UNDEFINED SYMBOLS
__aeabi_f2d
__aeabi_dsub
__aeabi_dmul
__aeabi_d2f
_Z7sincos1f
rand
__aeabi_unwind_cpp_pr1
_Znwj
_ZdlPv
_ZN12PIController9set_paramERK7PIParam
