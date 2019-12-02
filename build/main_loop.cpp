ARM GAS  /tmp/ccOJrCQ8.s 			page 1


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
  13              		.file	"main_loop.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.text._ZN3LED6updateEv,"axG",%progbits,_ZN3LED6updateEv,comdat
  18              		.align	1
  19              		.weak	_ZN3LED6updateEv
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN3LED6updateEv:
  27              		.fnstart
  28              	.LFB234:
  29              		.file 1 "motorlib/led.h"
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
  11:motorlib/led.h ****         : red_reg_(red_reg), green_reg_(green_reg), blue_reg_(blue_reg) {
  12:motorlib/led.h ****             invert_ = invert ? 1 : 0;
  13:motorlib/led.h ****         }
  14:motorlib/led.h ****     enum Mode {OFF, ON, BLINKING, PULSING};
  15:motorlib/led.h ****     enum Color {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, WHITE};
  16:motorlib/led.h ****     void set_mode(Mode mode) {}
  17:motorlib/led.h ****     void set_color(Color color) {}
  18:motorlib/led.h ****     void set_rate(float frequency) {}
  19:motorlib/led.h ****     void update() {
  30              		.loc 1 19 10
  31              		.cfi_startproc
  32              		@ args = 0, pretend = 0, frame = 24
  33              		@ frame_needed = 1, uses_anonymous_args = 0
  34              		@ link register save eliminated.
  35 0000 80B4     		push	{r7}
  36              	.LCFI0:
  37              		.cfi_def_cfa_offset 4
  38              		.cfi_offset 7, -4
  39 0002 87B0     		sub	sp, sp, #28
ARM GAS  /tmp/ccOJrCQ8.s 			page 2


  40              	.LCFI1:
  41              		.cfi_def_cfa_offset 32
  42 0004 00AF     		add	r7, sp, #0
  43              	.LCFI2:
  44              		.cfi_def_cfa_register 7
  45 0006 7860     		str	r0, [r7, #4]
  20:motorlib/led.h ****         // TODO figure out where to put 10000
  21:motorlib/led.h ****         if (i++ >= 10000) {
  46              		.loc 1 21 13
  47 0008 7B68     		ldr	r3, [r7, #4]
  48 000a 1B88     		ldrh	r3, [r3]
  49              		.loc 1 21 14
  50 000c 5A1C     		adds	r2, r3, #1
  51 000e 91B2     		uxth	r1, r2
  52 0010 7A68     		ldr	r2, [r7, #4]
  53 0012 1180     		strh	r1, [r2]	@ movhi
  54              		.loc 1 21 17
  55 0014 42F20F72 		movw	r2, #9999
  56 0018 9342     		cmp	r3, r2
  57 001a 8CBF     		ite	hi
  58 001c 0123     		movhi	r3, #1
  59 001e 0023     		movls	r3, #0
  60 0020 DBB2     		uxtb	r3, r3
  61              		.loc 1 21 9
  62 0022 002B     		cmp	r3, #0
  63 0024 02D0     		beq	.L2
  22:motorlib/led.h ****             i = 0;
  64              		.loc 1 22 15
  65 0026 7B68     		ldr	r3, [r7, #4]
  66 0028 0022     		movs	r2, #0
  67 002a 1A80     		strh	r2, [r3]	@ movhi
  68              	.L2:
  23:motorlib/led.h ****         }
  24:motorlib/led.h ****         float t = i*(1.f/10000.f);
  69              		.loc 1 24 19
  70 002c 7B68     		ldr	r3, [r7, #4]
  71 002e 1B88     		ldrh	r3, [r3]
  72 0030 07EE903A 		vmov	s15, r3	@ int
  73              		.loc 1 24 20
  74 0034 F8EEE77A 		vcvt.f32.s32	s15, s15
  75              		.loc 1 24 15
  76 0038 9FED247A 		vldr.32	s14, .L8
  77 003c 67EE877A 		vmul.f32	s15, s15, s14
  78 0040 C7ED057A 		vstr.32	s15, [r7, #20]
  25:motorlib/led.h ****         float intensity = t < 0.5 ? 2*t : 2-2*t;
  79              		.loc 1 25 35
  80 0044 D7ED057A 		vldr.32	s15, [r7, #20]
  81 0048 B6EE007A 		vmov.f32	s14, #5.0e-1
  82 004c F4EEC77A 		vcmpe.f32	s15, s14
  83 0050 F1EE10FA 		vmrs	APSR_nzcv, FPSCR
  84 0054 04D5     		bpl	.L7
  85              		.loc 1 25 35 is_stmt 0 discriminator 1
  86 0056 D7ED057A 		vldr.32	s15, [r7, #20]
  87 005a 77EEA77A 		vadd.f32	s15, s15, s15
  88 005e 07E0     		b	.L5
  89              	.L7:
  90              		.loc 1 25 46 is_stmt 1 discriminator 2
ARM GAS  /tmp/ccOJrCQ8.s 			page 3


  91 0060 D7ED057A 		vldr.32	s15, [r7, #20]
  92 0064 77EEA77A 		vadd.f32	s15, s15, s15
  93              		.loc 1 25 35 discriminator 2
  94 0068 B0EE007A 		vmov.f32	s14, #2.0e+0
  95 006c 77EE677A 		vsub.f32	s15, s14, s15
  96              	.L5:
  97              		.loc 1 25 15 discriminator 4
  98 0070 C7ED047A 		vstr.32	s15, [r7, #16]
  26:motorlib/led.h ****         uint16_t intensity_period = 65535*fabsf(invert_ - intensity*intensity*intensity);
  99              		.loc 1 26 49 discriminator 4
 100 0074 7B68     		ldr	r3, [r7, #4]
 101 0076 93ED017A 		vldr.32	s14, [r3, #4]
 102              		.loc 1 26 68 discriminator 4
 103 007a D7ED046A 		vldr.32	s13, [r7, #16]
 104 007e D7ED047A 		vldr.32	s15, [r7, #16]
 105 0082 66EEA76A 		vmul.f32	s13, s13, s15
 106              		.loc 1 26 78 discriminator 4
 107 0086 D7ED047A 		vldr.32	s15, [r7, #16]
 108 008a 66EEA77A 		vmul.f32	s15, s13, s15
 109              		.loc 1 26 57 discriminator 4
 110 008e 77EE677A 		vsub.f32	s15, s14, s15
 111              		.loc 1 26 48 discriminator 4
 112 0092 F0EEE77A 		vabs.f32	s15, s15
 113              		.loc 1 26 42 discriminator 4
 114 0096 9FED0E7A 		vldr.32	s14, .L8+4
 115 009a 67EE877A 		vmul.f32	s15, s15, s14
 116              		.loc 1 26 18 discriminator 4
 117 009e FCEEE77A 		vcvt.u32.f32	s15, s15
 118 00a2 17EE903A 		vmov	r3, s15	@ int
 119 00a6 FB81     		strh	r3, [r7, #14]	@ movhi
  27:motorlib/led.h ****         *red_reg_ = intensity_period;
 120              		.loc 1 27 10 discriminator 4
 121 00a8 7B68     		ldr	r3, [r7, #4]
 122 00aa 9B68     		ldr	r3, [r3, #8]
 123              		.loc 1 27 19 discriminator 4
 124 00ac FA89     		ldrh	r2, [r7, #14]	@ movhi
 125 00ae 1A80     		strh	r2, [r3]	@ movhi
  28:motorlib/led.h ****         *green_reg_ = intensity_period;
 126              		.loc 1 28 10 discriminator 4
 127 00b0 7B68     		ldr	r3, [r7, #4]
 128 00b2 DB68     		ldr	r3, [r3, #12]
 129              		.loc 1 28 21 discriminator 4
 130 00b4 FA89     		ldrh	r2, [r7, #14]	@ movhi
 131 00b6 1A80     		strh	r2, [r3]	@ movhi
  29:motorlib/led.h ****         *blue_reg_ = intensity_period;
 132              		.loc 1 29 10 discriminator 4
 133 00b8 7B68     		ldr	r3, [r7, #4]
 134 00ba 1B69     		ldr	r3, [r3, #16]
 135              		.loc 1 29 20 discriminator 4
 136 00bc FA89     		ldrh	r2, [r7, #14]	@ movhi
 137 00be 1A80     		strh	r2, [r3]	@ movhi
  30:motorlib/led.h ****     }
 138              		.loc 1 30 5 discriminator 4
 139 00c0 00BF     		nop
 140 00c2 1C37     		adds	r7, r7, #28
 141              	.LCFI3:
 142              		.cfi_def_cfa_offset 4
ARM GAS  /tmp/ccOJrCQ8.s 			page 4


 143 00c4 BD46     		mov	sp, r7
 144              	.LCFI4:
 145              		.cfi_def_cfa_register 13
 146              		@ sp needed
 147 00c6 5DF8047B 		ldr	r7, [sp], #4
 148              	.LCFI5:
 149              		.cfi_restore 7
 150              		.cfi_def_cfa_offset 0
 151 00ca 7047     		bx	lr
 152              	.L9:
 153              		.align	2
 154              	.L8:
 155 00cc 17B7D138 		.word	953267991
 156 00d0 00FF7F47 		.word	1199570688
 157              		.cfi_endproc
 158              	.LFE234:
 159              		.cantunwind
 160              		.fnend
 162              		.section	.text._ZN8MainLoop4initEv,"ax",%progbits
 163              		.align	1
 164              		.global	_ZN8MainLoop4initEv
 165              		.syntax unified
 166              		.thumb
 167              		.thumb_func
 168              		.fpu fpv4-sp-d16
 170              	_ZN8MainLoop4initEv:
 171              		.fnstart
 172              	.LFB242:
 173              		.file 2 "motorlib/main_loop.cpp"
   1:motorlib/main_loop.cpp **** 
   2:motorlib/main_loop.cpp **** #include "main_loop.h"
   3:motorlib/main_loop.cpp **** #include "led.h"
   4:motorlib/main_loop.cpp **** #include "control_fun.h"
   5:motorlib/main_loop.cpp **** 
   6:motorlib/main_loop.cpp **** //#include "../Src/pin_config.h"
   7:motorlib/main_loop.cpp **** #include <cmath>
   8:motorlib/main_loop.cpp **** //#include "stm32f4xx_hal.h"
   9:motorlib/main_loop.cpp **** #include "usb_communication.h"
  10:motorlib/main_loop.cpp **** #include "foc_i.h"
  11:motorlib/main_loop.cpp **** 
  12:motorlib/main_loop.cpp **** void MainLoop::init() {
 174              		.loc 2 12 23
 175              		.cfi_startproc
 176              		@ args = 0, pretend = 0, frame = 8
 177              		@ frame_needed = 1, uses_anonymous_args = 0
 178 0000 80B5     		push	{r7, lr}
 179              		.save {r7, lr}
 180              	.LCFI6:
 181              		.cfi_def_cfa_offset 8
 182              		.cfi_offset 7, -8
 183              		.cfi_offset 14, -4
 184              		.pad #8
 185 0002 82B0     		sub	sp, sp, #8
 186              	.LCFI7:
 187              		.cfi_def_cfa_offset 16
 188              		.setfp r7, sp, #0
 189 0004 00AF     		add	r7, sp, #0
ARM GAS  /tmp/ccOJrCQ8.s 			page 5


 190              	.LCFI8:
 191              		.cfi_def_cfa_register 7
 192 0006 7860     		str	r0, [r7, #4]
  13:motorlib/main_loop.cpp ****     communication_.init();
 193              		.loc 2 13 5
 194 0008 7B68     		ldr	r3, [r7, #4]
 195 000a DA6A     		ldr	r2, [r3, #44]
 196 000c 7B68     		ldr	r3, [r7, #4]
 197 000e DB6A     		ldr	r3, [r3, #44]
 198              		.loc 2 13 25
 199 0010 1B68     		ldr	r3, [r3]
 200 0012 1B68     		ldr	r3, [r3]
 201              		.loc 2 13 24
 202 0014 1046     		mov	r0, r2
 203 0016 9847     		blx	r3
 204              	.LVL0:
  14:motorlib/main_loop.cpp **** }
 205              		.loc 2 14 1
 206 0018 00BF     		nop
 207 001a 0837     		adds	r7, r7, #8
 208              	.LCFI9:
 209              		.cfi_def_cfa_offset 8
 210 001c BD46     		mov	sp, r7
 211              	.LCFI10:
 212              		.cfi_def_cfa_register 13
 213              		@ sp needed
 214 001e 80BD     		pop	{r7, pc}
 215              		.cfi_endproc
 216              	.LFE242:
 217              		.fnend
 219              		.section	.text._ZN8MainLoop6updateEv,"ax",%progbits
 220              		.align	1
 221              		.global	_ZN8MainLoop6updateEv
 222              		.syntax unified
 223              		.thumb
 224              		.thumb_func
 225              		.fpu fpv4-sp-d16
 227              	_ZN8MainLoop6updateEv:
 228              		.fnstart
 229              	.LFB571:
  15:motorlib/main_loop.cpp **** 
  16:motorlib/main_loop.cpp **** #include "../Src/usb/usbd_rt_if.h"
  17:motorlib/main_loop.cpp **** 
  18:motorlib/main_loop.cpp **** void MainLoop::update() {
 230              		.loc 2 18 25
 231              		.cfi_startproc
 232              		@ args = 0, pretend = 0, frame = 48
 233              		@ frame_needed = 1, uses_anonymous_args = 0
 234 0000 90B5     		push	{r4, r7, lr}
 235              		.save {r4, r7, lr}
 236              	.LCFI11:
 237              		.cfi_def_cfa_offset 12
 238              		.cfi_offset 4, -12
 239              		.cfi_offset 7, -8
 240              		.cfi_offset 14, -4
 241              		.pad #52
 242 0002 8DB0     		sub	sp, sp, #52
ARM GAS  /tmp/ccOJrCQ8.s 			page 6


 243              	.LCFI12:
 244              		.cfi_def_cfa_offset 64
 245              		.setfp r7, sp, #0
 246 0004 00AF     		add	r7, sp, #0
 247              	.LCFI13:
 248              		.cfi_def_cfa_register 7
 249 0006 7860     		str	r0, [r7, #4]
  19:motorlib/main_loop.cpp ****   count_++;
 250              		.loc 2 19 3
 251 0008 7B68     		ldr	r3, [r7, #4]
 252 000a D3E91234 		ldrd	r3, [r3, #72]
 253              		.loc 2 19 9
 254 000e 591C     		adds	r1, r3, #1
 255 0010 44F10002 		adc	r2, r4, #0
 256 0014 7B68     		ldr	r3, [r7, #4]
 257 0016 C3E91212 		strd	r1, [r3, #72]
  20:motorlib/main_loop.cpp ****   // int count_received = communication_.receive_data(&receive_data_);
  21:motorlib/main_loop.cpp ****   // if (count_received) {
  22:motorlib/main_loop.cpp ****   //   if (mode_ != static_cast<MainControlMode>(receive_data_.mode_desired)) {
  23:motorlib/main_loop.cpp ****   //     mode_ = static_cast<MainControlMode>(receive_data_.mode_desired);
  24:motorlib/main_loop.cpp ****   //     switch (mode_) {
  25:motorlib/main_loop.cpp ****   //       case OPEN:
  26:motorlib/main_loop.cpp ****   //       default:
  27:motorlib/main_loop.cpp ****   //         fast_loop_open_mode();
  28:motorlib/main_loop.cpp ****   //         break;
  29:motorlib/main_loop.cpp ****   //       case BRAKE:
  30:motorlib/main_loop.cpp ****   //         fast_loop_brake_mode();
  31:motorlib/main_loop.cpp ****   //         break;
  32:motorlib/main_loop.cpp ****   //       case NORMAL_CONTROL:
  33:motorlib/main_loop.cpp ****   //         fast_loop_current_mode();
  34:motorlib/main_loop.cpp ****   //         break;
  35:motorlib/main_loop.cpp ****   //     }
  36:motorlib/main_loop.cpp ****   //   }
  37:motorlib/main_loop.cpp ****   // }
  38:motorlib/main_loop.cpp ****   
  39:motorlib/main_loop.cpp ****   fast_loop_get_status(&fast_loop_status_);
 258              		.loc 2 39 23
 259 001a 7B68     		ldr	r3, [r7, #4]
 260 001c 5033     		adds	r3, r3, #80
 261 001e 1846     		mov	r0, r3
 262 0020 FFF7FEFF 		bl	fast_loop_get_status
  40:motorlib/main_loop.cpp **** 
  41:motorlib/main_loop.cpp ****   float iq_des = controller_.step(receive_data_.position_desired, fast_loop_status_.motor_position.
 263              		.loc 2 41 18
 264 0024 7B68     		ldr	r3, [r7, #4]
 265 0026 996A     		ldr	r1, [r3, #40]
 266 0028 7B68     		ldr	r3, [r7, #4]
 267 002a 9B6A     		ldr	r3, [r3, #40]
 268              		.loc 2 41 108
 269 002c 1B68     		ldr	r3, [r3]
 270 002e 0833     		adds	r3, r3, #8
 271 0030 1B68     		ldr	r3, [r3]
 272              		.loc 2 41 34
 273 0032 7A68     		ldr	r2, [r7, #4]
 274 0034 D2ED107A 		vldr.32	s15, [r2, #64]
 275 0038 7A68     		ldr	r2, [r7, #4]
 276 003a 92ED217A 		vldr.32	s14, [r2, #132]
ARM GAS  /tmp/ccOJrCQ8.s 			page 7


 277 003e F0EE470A 		vmov.f32	s1, s14
 278 0042 B0EE670A 		vmov.f32	s0, s15
 279 0046 0846     		mov	r0, r1
 280 0048 9847     		blx	r3
 281              	.LVL1:
 282 004a B0EE407A 		vmov.f32	s14, s0
  42:motorlib/main_loop.cpp ****               receive_data_.current_desired;
 283              		.loc 2 42 29
 284 004e 7B68     		ldr	r3, [r7, #4]
 285 0050 D3ED0F7A 		vldr.32	s15, [r3, #60]
 286 0054 77EE277A 		vadd.f32	s15, s14, s15
 287 0058 C7ED0B7A 		vstr.32	s15, [r7, #44]
  43:motorlib/main_loop.cpp **** 
  44:motorlib/main_loop.cpp ****   fast_loop_set_iq_des(iq_des);
 288              		.loc 2 44 23
 289 005c 97ED0B0A 		vldr.32	s0, [r7, #44]
 290 0060 FFF7FEFF 		bl	fast_loop_set_iq_des
  45:motorlib/main_loop.cpp ****   SendData send_data;
  46:motorlib/main_loop.cpp ****   send_data.iq = fast_loop_status_.foc_status.measured.i_q;
 291              		.loc 2 46 56
 292 0064 7B68     		ldr	r3, [r7, #4]
 293 0066 5B6E     		ldr	r3, [r3, #100]	@ float
 294              		.loc 2 46 16
 295 0068 FB61     		str	r3, [r7, #28]	@ float
  47:motorlib/main_loop.cpp ****   send_data.host_timestamp_received = receive_data_.host_timestamp;
 296              		.loc 2 47 53
 297 006a 7B68     		ldr	r3, [r7, #4]
 298 006c 5B6B     		ldr	r3, [r3, #52]
 299              		.loc 2 47 37
 300 006e 3B61     		str	r3, [r7, #16]
  48:motorlib/main_loop.cpp ****   send_data.mcu_timestamp = fast_loop_status_.timestamp;
 301              		.loc 2 48 47
 302 0070 7B68     		ldr	r3, [r7, #4]
 303 0072 1B6D     		ldr	r3, [r3, #80]
 304              		.loc 2 48 27
 305 0074 FB60     		str	r3, [r7, #12]
  49:motorlib/main_loop.cpp ****   send_data.motor_encoder = fast_loop_status_.motor_position.raw;
 306              		.loc 2 49 62
 307 0076 7B68     		ldr	r3, [r7, #4]
 308 0078 D3F88030 		ldr	r3, [r3, #128]
 309              		.loc 2 49 27
 310 007c 3B62     		str	r3, [r7, #32]
  50:motorlib/main_loop.cpp ****   send_data.motor_position = fast_loop_status_.motor_position.position;
 311              		.loc 2 50 63
 312 007e 7B68     		ldr	r3, [r7, #4]
 313 0080 D3F88430 		ldr	r3, [r3, #132]	@ float
 314              		.loc 2 50 28
 315 0084 7B61     		str	r3, [r7, #20]	@ float
  51:motorlib/main_loop.cpp ****   send_data.joint_position = 0;
 316              		.loc 2 51 28
 317 0086 4FF00003 		mov	r3, #0
 318 008a BB61     		str	r3, [r7, #24]	@ float
  52:motorlib/main_loop.cpp ****   send_data.reserved[0] = fast_loop_status_.foc_status.measured.i_0;
 319              		.loc 2 52 65
 320 008c 7B68     		ldr	r3, [r7, #4]
 321 008e 9B6E     		ldr	r3, [r3, #104]	@ float
 322              		.loc 2 52 25
ARM GAS  /tmp/ccOJrCQ8.s 			page 8


 323 0090 7B62     		str	r3, [r7, #36]	@ float
  53:motorlib/main_loop.cpp ****     USB_send(reinterpret_cast<uint8_t *>(&count_),sizeof(count_));
 324              		.loc 2 53 42
 325 0092 7B68     		ldr	r3, [r7, #4]
 326 0094 4833     		adds	r3, r3, #72
 327              		.loc 2 53 13
 328 0096 0821     		movs	r1, #8
 329 0098 1846     		mov	r0, r3
 330 009a FFF7FEFF 		bl	USB_send
  54:motorlib/main_loop.cpp ****  // communication_.send_data(send_data);
  55:motorlib/main_loop.cpp ****   led_.update();
 331              		.loc 2 55 3
 332 009e 7B68     		ldr	r3, [r7, #4]
 333 00a0 1B6B     		ldr	r3, [r3, #48]
 334              		.loc 2 55 14
 335 00a2 1846     		mov	r0, r3
 336 00a4 FFF7FEFF 		bl	_ZN3LED6updateEv
  56:motorlib/main_loop.cpp **** }
 337              		.loc 2 56 1
 338 00a8 00BF     		nop
 339 00aa 3437     		adds	r7, r7, #52
 340              	.LCFI14:
 341              		.cfi_def_cfa_offset 12
 342 00ac BD46     		mov	sp, r7
 343              	.LCFI15:
 344              		.cfi_def_cfa_register 13
 345              		@ sp needed
 346 00ae 90BD     		pop	{r4, r7, pc}
 347              		.cfi_endproc
 348              	.LFE571:
 349              		.fnend
 351              		.section	.text._ZN8MainLoop9set_paramER13MainLoopParam,"ax",%progbits
 352              		.align	1
 353              		.global	_ZN8MainLoop9set_paramER13MainLoopParam
 354              		.syntax unified
 355              		.thumb
 356              		.thumb_func
 357              		.fpu fpv4-sp-d16
 359              	_ZN8MainLoop9set_paramER13MainLoopParam:
 360              		.fnstart
 361              	.LFB572:
  57:motorlib/main_loop.cpp **** 
  58:motorlib/main_loop.cpp **** void MainLoop::set_param(MainLoopParam &param) {
 362              		.loc 2 58 48
 363              		.cfi_startproc
 364              		@ args = 0, pretend = 0, frame = 8
 365              		@ frame_needed = 1, uses_anonymous_args = 0
 366 0000 B0B5     		push	{r4, r5, r7, lr}
 367              		.save {r4, r5, r7, lr}
 368              	.LCFI16:
 369              		.cfi_def_cfa_offset 16
 370              		.cfi_offset 4, -16
 371              		.cfi_offset 5, -12
 372              		.cfi_offset 7, -8
 373              		.cfi_offset 14, -4
 374              		.pad #8
 375 0002 82B0     		sub	sp, sp, #8
ARM GAS  /tmp/ccOJrCQ8.s 			page 9


 376              	.LCFI17:
 377              		.cfi_def_cfa_offset 24
 378              		.setfp r7, sp, #0
 379 0004 00AF     		add	r7, sp, #0
 380              	.LCFI18:
 381              		.cfi_def_cfa_register 7
 382 0006 7860     		str	r0, [r7, #4]
 383 0008 3960     		str	r1, [r7]
  59:motorlib/main_loop.cpp ****     controller_.set_param(param.controller_param);
 384              		.loc 2 59 5
 385 000a 7B68     		ldr	r3, [r7, #4]
 386 000c 9A6A     		ldr	r2, [r3, #40]
 387              		.loc 2 59 33
 388 000e 3B68     		ldr	r3, [r7]
 389 0010 0433     		adds	r3, r3, #4
 390              		.loc 2 59 26
 391 0012 1946     		mov	r1, r3
 392 0014 1046     		mov	r0, r2
 393 0016 FFF7FEFF 		bl	_ZN13PIDController9set_paramERK8PIDParam
  60:motorlib/main_loop.cpp ****     param_ = param;
 394              		.loc 2 60 12
 395 001a 7A68     		ldr	r2, [r7, #4]
 396 001c 3B68     		ldr	r3, [r7]
 397 001e 1446     		mov	r4, r2
 398 0020 1D46     		mov	r5, r3
 399 0022 0FCD     		ldmia	r5!, {r0, r1, r2, r3}
 400 0024 0FC4     		stmia	r4!, {r0, r1, r2, r3}
 401 0026 0FCD     		ldmia	r5!, {r0, r1, r2, r3}
 402 0028 0FC4     		stmia	r4!, {r0, r1, r2, r3}
 403 002a 95E80300 		ldm	r5, {r0, r1}
 404 002e 84E80300 		stm	r4, {r0, r1}
  61:motorlib/main_loop.cpp **** }
 405              		.loc 2 61 1
 406 0032 00BF     		nop
 407 0034 0837     		adds	r7, r7, #8
 408              	.LCFI19:
 409              		.cfi_def_cfa_offset 16
 410 0036 BD46     		mov	sp, r7
 411              	.LCFI20:
 412              		.cfi_def_cfa_register 13
 413              		@ sp needed
 414 0038 B0BD     		pop	{r4, r5, r7, pc}
 415              		.cfi_endproc
 416              	.LFE572:
 417              		.fnend
 419              		.section	.text._ZNK8MainLoop10get_statusEP14MainLoopStatus,"ax",%progbits
 420              		.align	1
 421              		.global	_ZNK8MainLoop10get_statusEP14MainLoopStatus
 422              		.syntax unified
 423              		.thumb
 424              		.thumb_func
 425              		.fpu fpv4-sp-d16
 427              	_ZNK8MainLoop10get_statusEP14MainLoopStatus:
 428              		.fnstart
 429              	.LFB573:
  62:motorlib/main_loop.cpp **** 
  63:motorlib/main_loop.cpp **** void MainLoop::get_status(MainLoopStatus * const main_loop_status) const {
ARM GAS  /tmp/ccOJrCQ8.s 			page 10


 430              		.loc 2 63 74
 431              		.cfi_startproc
 432              		@ args = 0, pretend = 0, frame = 8
 433              		@ frame_needed = 1, uses_anonymous_args = 0
 434              		@ link register save eliminated.
 435 0000 80B4     		push	{r7}
 436              	.LCFI21:
 437              		.cfi_def_cfa_offset 4
 438              		.cfi_offset 7, -4
 439 0002 83B0     		sub	sp, sp, #12
 440              	.LCFI22:
 441              		.cfi_def_cfa_offset 16
 442 0004 00AF     		add	r7, sp, #0
 443              	.LCFI23:
 444              		.cfi_def_cfa_register 7
 445 0006 7860     		str	r0, [r7, #4]
 446 0008 3960     		str	r1, [r7]
  64:motorlib/main_loop.cpp ****   //main_loop_status->torque = torque;
  65:motorlib/main_loop.cpp **** }
 447              		.loc 2 65 1
 448 000a 00BF     		nop
 449 000c 0C37     		adds	r7, r7, #12
 450              	.LCFI24:
 451              		.cfi_def_cfa_offset 4
 452 000e BD46     		mov	sp, r7
 453              	.LCFI25:
 454              		.cfi_def_cfa_register 13
 455              		@ sp needed
 456 0010 5DF8047B 		ldr	r7, [sp], #4
 457              	.LCFI26:
 458              		.cfi_restore 7
 459              		.cfi_def_cfa_offset 0
 460 0014 7047     		bx	lr
 461              		.cfi_endproc
 462              	.LFE573:
 463              		.cantunwind
 464              		.fnend
 466              		.text
 467              	.Letext0:
 468              		.file 3 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 469              		.file 4 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 470              		.file 5 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdint.h"
 471              		.file 6 "motorlib/messages.h"
 472              		.file 7 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cmath"
 473              		.file 8 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdint"
 474              		.file 9 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdlib"
 475              		.file 10 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_abs.h"
 476              		.file 11 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/th
 477              		.file 12 "/opt/gcc-arm-none-eabi-8-2019-q3-update/lib/gcc/arm-none-eabi/8.3.1/include/stddef.h"
 478              		.file 13 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_types.h"
 479              		.file 14 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/reent.h"
 480              		.file 15 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/lock.h"
 481              		.file 16 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/math.h"
 482              		.file 17 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdlib.h"
 483              		.file 18 "motorlib/main_loop.h"
 484              		.file 19 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/stdlib.h"
 485              		.file 20 "Drivers/CMSIS/Include/core_cm4.h"
ARM GAS  /tmp/ccOJrCQ8.s 			page 11


 486              		.file 21 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/system_stm32g4xx.h"
 487              		.file 22 "Drivers/STM32G4xx_HAL_Driver/Inc/stm32g4xx_hal_def.h"
 488              		.file 23 "Drivers/STM32G4xx_HAL_Driver/Inc/stm32g4xx_hal_flash.h"
 489              		.file 24 "Drivers/STM32G4xx_HAL_Driver/Inc/stm32g4xx_hal.h"
 490              		.file 25 "Middlewares/ST/STM32_USB_Device_Library/Core/Inc/usbd_def.h"
 491              		.file 26 "motorlib/../Src/usb/usbd_rt.h"
 492              		.file 27 "<built-in>"
ARM GAS  /tmp/ccOJrCQ8.s 			page 12


DEFINED SYMBOLS
                            *ABS*:0000000000000000 main_loop.cpp
     /tmp/ccOJrCQ8.s:18     .text._ZN3LED6updateEv:0000000000000000 $t
     /tmp/ccOJrCQ8.s:26     .text._ZN3LED6updateEv:0000000000000000 _ZN3LED6updateEv
     /tmp/ccOJrCQ8.s:155    .text._ZN3LED6updateEv:00000000000000cc $d
 .ARM.exidx.text._ZN3LED6updateEv:0000000000000000 $d
     /tmp/ccOJrCQ8.s:163    .text._ZN8MainLoop4initEv:0000000000000000 $t
     /tmp/ccOJrCQ8.s:170    .text._ZN8MainLoop4initEv:0000000000000000 _ZN8MainLoop4initEv
.ARM.extab.text._ZN8MainLoop4initEv:0000000000000000 $d
.ARM.exidx.text._ZN8MainLoop4initEv:0000000000000000 $d
     /tmp/ccOJrCQ8.s:220    .text._ZN8MainLoop6updateEv:0000000000000000 $t
     /tmp/ccOJrCQ8.s:227    .text._ZN8MainLoop6updateEv:0000000000000000 _ZN8MainLoop6updateEv
.ARM.extab.text._ZN8MainLoop6updateEv:0000000000000000 $d
.ARM.exidx.text._ZN8MainLoop6updateEv:0000000000000000 $d
     /tmp/ccOJrCQ8.s:352    .text._ZN8MainLoop9set_paramER13MainLoopParam:0000000000000000 $t
     /tmp/ccOJrCQ8.s:359    .text._ZN8MainLoop9set_paramER13MainLoopParam:0000000000000000 _ZN8MainLoop9set_paramER13MainLoopParam
.ARM.extab.text._ZN8MainLoop9set_paramER13MainLoopParam:0000000000000000 $d
.ARM.exidx.text._ZN8MainLoop9set_paramER13MainLoopParam:0000000000000000 $d
     /tmp/ccOJrCQ8.s:420    .text._ZNK8MainLoop10get_statusEP14MainLoopStatus:0000000000000000 $t
     /tmp/ccOJrCQ8.s:427    .text._ZNK8MainLoop10get_statusEP14MainLoopStatus:0000000000000000 _ZNK8MainLoop10get_statusEP14MainLoopStatus
.ARM.exidx.text._ZNK8MainLoop10get_statusEP14MainLoopStatus:0000000000000000 $d

UNDEFINED SYMBOLS
__aeabi_unwind_cpp_pr1
fast_loop_get_status
fast_loop_set_iq_des
USB_send
_ZN13PIDController9set_paramERK8PIDParam
