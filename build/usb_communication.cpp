ARM GAS  /tmp/ccEBsJev.s 			page 1


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
  13              		.file	"usb_communication.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.text._ZN4USB111send_data32EhPKmh,"axG",%progbits,_ZN4USB111send_data32EhPKmh,comdat
  18              		.align	1
  19              		.weak	_ZN4USB111send_data32EhPKmh
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN4USB111send_data32EhPKmh:
  27              		.fnstart
  28              	.LFB0:
  29              		.file 1 "motorlib/usb.h"
   1:motorlib/usb.h **** #ifndef USB_H
   2:motorlib/usb.h **** #define USB_H
   3:motorlib/usb.h **** 
   4:motorlib/usb.h **** #include <cstdint>
   5:motorlib/usb.h **** // #include "stm32f4xx_hal.h"
   6:motorlib/usb.h **** // #include "stm32f4xx_ll_usb.h"
   7:motorlib/usb.h **** 
   8:motorlib/usb.h **** extern uint8_t usb_connected;
   9:motorlib/usb.h **** 
  10:motorlib/usb.h **** #define USBx USB_OTG_FS
  11:motorlib/usb.h **** class USB1 {
  12:motorlib/usb.h **** 
  13:motorlib/usb.h ****  public:
  14:motorlib/usb.h ****     // limited to 64 bytes right now
  15:motorlib/usb.h ****     void send_data32(uint8_t endpoint, const uint32_t *data, uint8_t length32) 
  30              		.loc 1 15 10
  31              		.cfi_startproc
  32              		@ args = 0, pretend = 0, frame = 16
  33              		@ frame_needed = 1, uses_anonymous_args = 0
  34              		@ link register save eliminated.
  35 0000 80B4     		push	{r7}
  36              	.LCFI0:
  37              		.cfi_def_cfa_offset 4
  38              		.cfi_offset 7, -4
  39 0002 85B0     		sub	sp, sp, #20
  40              	.LCFI1:
  41              		.cfi_def_cfa_offset 24
  42 0004 00AF     		add	r7, sp, #0
  43              	.LCFI2:
ARM GAS  /tmp/ccEBsJev.s 			page 2


  44              		.cfi_def_cfa_register 7
  45 0006 F860     		str	r0, [r7, #12]
  46 0008 7A60     		str	r2, [r7, #4]
  47 000a 1A46     		mov	r2, r3
  48 000c 0B46     		mov	r3, r1
  49 000e FB72     		strb	r3, [r7, #11]
  50 0010 1346     		mov	r3, r2
  51 0012 BB72     		strb	r3, [r7, #10]
  16:motorlib/usb.h ****     {
  17:motorlib/usb.h **** //         if (!usb_connected) {
  18:motorlib/usb.h **** //             return;
  19:motorlib/usb.h **** //         }
  20:motorlib/usb.h **** 
  21:motorlib/usb.h **** //         //USBx_DEVICE->DIEPMSK &= ~USB_OTG_DIEPMSK_INEPNEM;
  22:motorlib/usb.h **** //     HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
  23:motorlib/usb.h **** //         if (USBx_INEP(endpoint)->DIEPCTL & USB_OTG_DIEPCTL_EPENA) {
  24:motorlib/usb.h **** //             // TODO it would be nice to timeout/cancel old transmissions, instead just don't all
  25:motorlib/usb.h ****            
  26:motorlib/usb.h **** //             USBx_INEP(endpoint)->DIEPCTL |= USB_OTG_DIEPCTL_SNAK;
  27:motorlib/usb.h **** //             while(!(USBx_INEP(endpoint)->DIEPINT & USB_OTG_DIEPINT_INEPNE));    //wait on nak
  28:motorlib/usb.h **** //             USBx->GRSTCTL = ( USB_OTG_GRSTCTL_TXFFLSH |(uint32_t)( 1 << (USB_OTG_GRSTCTL_TXFNUM_
  29:motorlib/usb.h **** //             while(USBx->GRSTCTL & USB_OTG_GRSTCTL_TXFFLSH);     // wait on flush
  30:motorlib/usb.h **** //     //   //      USBx_INEP(endpoint)->DIEPCTL |= USB_OTG_DIEPCTL_EPDIS;
  31:motorlib/usb.h **** //       //      while(USBx_INEP(endpoint)->DIEPCTL & USB_OTG_DIEPCTL_EPENA);
  32:motorlib/usb.h ****               
  33:motorlib/usb.h **** // //GPIOC->ODR &= ~GPIO_ODR_OD13;
  34:motorlib/usb.h **** //         //   return;
  35:motorlib/usb.h **** //         }
  36:motorlib/usb.h **** 
  37:motorlib/usb.h **** // // __disable_irq();
  38:motorlib/usb.h **** 
  39:motorlib/usb.h **** 
  40:motorlib/usb.h ****    
  41:motorlib/usb.h **** 
  42:motorlib/usb.h **** //         USBx_INEP(endpoint)->DIEPTSIZ = 0;  // TODO necessary?
  43:motorlib/usb.h **** //         USBx_INEP(endpoint)->DIEPTSIZ = sizeof(uint32_t) * length32 | (1 << USB_OTG_DIEPTSIZ_PKT
  44:motorlib/usb.h **** //         USBx_INEP(endpoint)->DIEPCTL |= USB_OTG_DIEPCTL_EPENA | USB_OTG_DIEPCTL_CNAK ;
  45:motorlib/usb.h **** //         for(int i=0; i<length32; i++) {
  46:motorlib/usb.h **** //             USBx_DFIFO(endpoint) = data[i]; 
  47:motorlib/usb.h **** //         }
  48:motorlib/usb.h **** //      //   USBx_INEP(endpoint)->DIEPCTL |= USB_OTG_DIEPCTL_CNAK;  
  49:motorlib/usb.h **** //     //     __enable_irq();
  50:motorlib/usb.h **** //              HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  51:motorlib/usb.h **** 
  52:motorlib/usb.h ****     }
  52              		.loc 1 52 5
  53 0014 00BF     		nop
  54 0016 1437     		adds	r7, r7, #20
  55              	.LCFI3:
  56              		.cfi_def_cfa_offset 4
  57 0018 BD46     		mov	sp, r7
  58              	.LCFI4:
  59              		.cfi_def_cfa_register 13
  60              		@ sp needed
  61 001a 5DF8047B 		ldr	r7, [sp], #4
  62              	.LCFI5:
  63              		.cfi_restore 7
ARM GAS  /tmp/ccEBsJev.s 			page 3


  64              		.cfi_def_cfa_offset 0
  65 001e 7047     		bx	lr
  66              		.cfi_endproc
  67              	.LFE0:
  68              		.cantunwind
  69              		.fnend
  71              		.section	.text._ZN4USB19send_dataEhPKhh,"axG",%progbits,_ZN4USB19send_dataEhPKhh,comdat
  72              		.align	1
  73              		.weak	_ZN4USB19send_dataEhPKhh
  74              		.syntax unified
  75              		.thumb
  76              		.thumb_func
  77              		.fpu fpv4-sp-d16
  79              	_ZN4USB19send_dataEhPKhh:
  80              		.fnstart
  81              	.LFB1:
  53:motorlib/usb.h **** 
  54:motorlib/usb.h ****     void send_data(uint8_t endpoint, const uint8_t *data, uint8_t length) {
  82              		.loc 1 54 10
  83              		.cfi_startproc
  84              		@ args = 0, pretend = 0, frame = 16
  85              		@ frame_needed = 1, uses_anonymous_args = 0
  86 0000 80B5     		push	{r7, lr}
  87              	.LCFI6:
  88              		.cfi_def_cfa_offset 8
  89              		.cfi_offset 7, -8
  90              		.cfi_offset 14, -4
  91 0002 84B0     		sub	sp, sp, #16
  92              	.LCFI7:
  93              		.cfi_def_cfa_offset 24
  94 0004 00AF     		add	r7, sp, #0
  95              	.LCFI8:
  96              		.cfi_def_cfa_register 7
  97 0006 F860     		str	r0, [r7, #12]
  98 0008 7A60     		str	r2, [r7, #4]
  99 000a 1A46     		mov	r2, r3
 100 000c 0B46     		mov	r3, r1
 101 000e FB72     		strb	r3, [r7, #11]
 102 0010 1346     		mov	r3, r2
 103 0012 BB72     		strb	r3, [r7, #10]
  55:motorlib/usb.h ****         send_data32(endpoint, (uint32_t *) data, (length+3)/4);
 104              		.loc 1 55 57
 105 0014 BB7A     		ldrb	r3, [r7, #10]	@ zero_extendqisi2
 106 0016 0333     		adds	r3, r3, #3
 107              		.loc 1 55 60
 108 0018 002B     		cmp	r3, #0
 109 001a 00DA     		bge	.L3
 110 001c 0333     		adds	r3, r3, #3
 111              	.L3:
 112 001e 9B10     		asrs	r3, r3, #2
 113              		.loc 1 55 20
 114 0020 DBB2     		uxtb	r3, r3
 115 0022 F97A     		ldrb	r1, [r7, #11]	@ zero_extendqisi2
 116 0024 7A68     		ldr	r2, [r7, #4]
 117 0026 F868     		ldr	r0, [r7, #12]
 118 0028 FFF7FEFF 		bl	_ZN4USB111send_data32EhPKmh
  56:motorlib/usb.h ****     }
ARM GAS  /tmp/ccEBsJev.s 			page 4


 119              		.loc 1 56 5
 120 002c 00BF     		nop
 121 002e 1037     		adds	r7, r7, #16
 122              	.LCFI9:
 123              		.cfi_def_cfa_offset 8
 124 0030 BD46     		mov	sp, r7
 125              	.LCFI10:
 126              		.cfi_def_cfa_register 13
 127              		@ sp needed
 128 0032 80BD     		pop	{r7, pc}
 129              		.cfi_endproc
 130              	.LFE1:
 131              		.cantunwind
 132              		.fnend
 134              		.section	.text._ZN16USBCommunication4initEv,"ax",%progbits
 135              		.align	1
 136              		.global	_ZN16USBCommunication4initEv
 137              		.syntax unified
 138              		.thumb
 139              		.thumb_func
 140              		.fpu fpv4-sp-d16
 142              	_ZN16USBCommunication4initEv:
 143              		.fnstart
 144              	.LFB8:
 145              		.file 2 "motorlib/usb_communication.cpp"
   1:motorlib/usb_communication.cpp **** #include "usb_communication.h"
   2:motorlib/usb_communication.cpp **** #include "usb.h"
   3:motorlib/usb_communication.cpp **** #include <cstring>
   4:motorlib/usb_communication.cpp **** 
   5:motorlib/usb_communication.cpp **** extern uint32_t data2[16];
   6:motorlib/usb_communication.cpp **** extern uint8_t data2_count;
   7:motorlib/usb_communication.cpp **** 
   8:motorlib/usb_communication.cpp **** void USBCommunication::init() {
 146              		.loc 2 8 31
 147              		.cfi_startproc
 148              		@ args = 0, pretend = 0, frame = 8
 149              		@ frame_needed = 1, uses_anonymous_args = 0
 150 0000 80B5     		push	{r7, lr}
 151              		.save {r7, lr}
 152              	.LCFI11:
 153              		.cfi_def_cfa_offset 8
 154              		.cfi_offset 7, -8
 155              		.cfi_offset 14, -4
 156              		.pad #8
 157 0002 82B0     		sub	sp, sp, #8
 158              	.LCFI12:
 159              		.cfi_def_cfa_offset 16
 160              		.setfp r7, sp, #0
 161 0004 00AF     		add	r7, sp, #0
 162              	.LCFI13:
 163              		.cfi_def_cfa_register 7
 164 0006 7860     		str	r0, [r7, #4]
   9:motorlib/usb_communication.cpp ****     usb_ = new USB1;
 165              		.loc 2 9 16
 166 0008 0120     		movs	r0, #1
 167 000a FFF7FEFF 		bl	_Znwj
 168 000e 0346     		mov	r3, r0
ARM GAS  /tmp/ccEBsJev.s 			page 5


 169 0010 1A46     		mov	r2, r3
 170              		.loc 2 9 10
 171 0012 7B68     		ldr	r3, [r7, #4]
 172 0014 5A60     		str	r2, [r3, #4]
  10:motorlib/usb_communication.cpp **** }
 173              		.loc 2 10 1
 174 0016 00BF     		nop
 175 0018 0837     		adds	r7, r7, #8
 176              	.LCFI14:
 177              		.cfi_def_cfa_offset 8
 178 001a BD46     		mov	sp, r7
 179              	.LCFI15:
 180              		.cfi_def_cfa_register 13
 181              		@ sp needed
 182 001c 80BD     		pop	{r7, pc}
 183              		.cfi_endproc
 184              	.LFE8:
 185              		.fnend
 187              		.section	.text._ZN16USBCommunication12receive_dataEP11ReceiveData,"ax",%progbits
 188              		.align	1
 189              		.global	_ZN16USBCommunication12receive_dataEP11ReceiveData
 190              		.syntax unified
 191              		.thumb
 192              		.thumb_func
 193              		.fpu fpv4-sp-d16
 195              	_ZN16USBCommunication12receive_dataEP11ReceiveData:
 196              		.fnstart
 197              	.LFB9:
  11:motorlib/usb_communication.cpp **** 
  12:motorlib/usb_communication.cpp **** int USBCommunication::receive_data(ReceiveData * const data) {
 198              		.loc 2 12 62
 199              		.cfi_startproc
 200              		@ args = 0, pretend = 0, frame = 8
 201              		@ frame_needed = 1, uses_anonymous_args = 0
 202              		@ link register save eliminated.
 203 0000 80B4     		push	{r7}
 204              	.LCFI16:
 205              		.cfi_def_cfa_offset 4
 206              		.cfi_offset 7, -4
 207 0002 83B0     		sub	sp, sp, #12
 208              	.LCFI17:
 209              		.cfi_def_cfa_offset 16
 210 0004 00AF     		add	r7, sp, #0
 211              	.LCFI18:
 212              		.cfi_def_cfa_register 7
 213 0006 7860     		str	r0, [r7, #4]
 214 0008 3960     		str	r1, [r7]
  13:motorlib/usb_communication.cpp ****     // std::memcpy(data, data2, data2_count);
  14:motorlib/usb_communication.cpp ****     // int count = data2_count;
  15:motorlib/usb_communication.cpp ****     // data2_count = 0;
  16:motorlib/usb_communication.cpp ****     //return count;
  17:motorlib/usb_communication.cpp **** }
 215              		.loc 2 17 1
 216 000a 00BF     		nop
 217 000c 1846     		mov	r0, r3
 218 000e 0C37     		adds	r7, r7, #12
 219              	.LCFI19:
ARM GAS  /tmp/ccEBsJev.s 			page 6


 220              		.cfi_def_cfa_offset 4
 221 0010 BD46     		mov	sp, r7
 222              	.LCFI20:
 223              		.cfi_def_cfa_register 13
 224              		@ sp needed
 225 0012 5DF8047B 		ldr	r7, [sp], #4
 226              	.LCFI21:
 227              		.cfi_restore 7
 228              		.cfi_def_cfa_offset 0
 229 0016 7047     		bx	lr
 230              		.cfi_endproc
 231              	.LFE9:
 232              		.cantunwind
 233              		.fnend
 235              		.section	.text._ZN16USBCommunication9send_dataERK8SendData,"ax",%progbits
 236              		.align	1
 237              		.global	_ZN16USBCommunication9send_dataERK8SendData
 238              		.syntax unified
 239              		.thumb
 240              		.thumb_func
 241              		.fpu fpv4-sp-d16
 243              	_ZN16USBCommunication9send_dataERK8SendData:
 244              		.fnstart
 245              	.LFB10:
  18:motorlib/usb_communication.cpp **** 
  19:motorlib/usb_communication.cpp **** void USBCommunication::send_data(const SendData &data) {
 246              		.loc 2 19 56
 247              		.cfi_startproc
 248              		@ args = 0, pretend = 0, frame = 8
 249              		@ frame_needed = 1, uses_anonymous_args = 0
 250 0000 80B5     		push	{r7, lr}
 251              	.LCFI22:
 252              		.cfi_def_cfa_offset 8
 253              		.cfi_offset 7, -8
 254              		.cfi_offset 14, -4
 255 0002 82B0     		sub	sp, sp, #8
 256              	.LCFI23:
 257              		.cfi_def_cfa_offset 16
 258 0004 00AF     		add	r7, sp, #0
 259              	.LCFI24:
 260              		.cfi_def_cfa_register 7
 261 0006 7860     		str	r0, [r7, #4]
 262 0008 3960     		str	r1, [r7]
  20:motorlib/usb_communication.cpp ****     usb_->send_data(2, reinterpret_cast<const uint8_t *>(&data), sizeof(SendData));
 263              		.loc 2 20 5
 264 000a 7B68     		ldr	r3, [r7, #4]
 265 000c 5868     		ldr	r0, [r3, #4]
 266              		.loc 2 20 20
 267 000e 2023     		movs	r3, #32
 268 0010 3A68     		ldr	r2, [r7]
 269 0012 0221     		movs	r1, #2
 270 0014 FFF7FEFF 		bl	_ZN4USB19send_dataEhPKhh
  21:motorlib/usb_communication.cpp **** }
 271              		.loc 2 21 1
 272 0018 00BF     		nop
 273 001a 0837     		adds	r7, r7, #8
 274              	.LCFI25:
ARM GAS  /tmp/ccEBsJev.s 			page 7


 275              		.cfi_def_cfa_offset 8
 276 001c BD46     		mov	sp, r7
 277              	.LCFI26:
 278              		.cfi_def_cfa_register 13
 279              		@ sp needed
 280 001e 80BD     		pop	{r7, pc}
 281              		.cfi_endproc
 282              	.LFE10:
 283              		.cantunwind
 284              		.fnend
 286              		.global	_ZTV16USBCommunication
 287              		.section	.rodata._ZTV16USBCommunication,"a"
 288              		.align	2
 291              	_ZTV16USBCommunication:
 292 0000 00000000 		.word	0
 293 0004 00000000 		.word	_ZTI16USBCommunication
 294 0008 00000000 		.word	_ZN16USBCommunication4initEv
 295 000c 00000000 		.word	_ZN16USBCommunication12receive_dataEP11ReceiveData
 296 0010 00000000 		.word	_ZN16USBCommunication9send_dataERK8SendData
 297              		.global	_ZTI16USBCommunication
 298              		.section	.rodata._ZTI16USBCommunication,"a"
 299              		.align	2
 302              	_ZTI16USBCommunication:
 303 0000 08000000 		.word	_ZTVN10__cxxabiv120__si_class_type_infoE+8
 304 0004 00000000 		.word	_ZTS16USBCommunication
 305 0008 00000000 		.word	_ZTI13Communication
 306              		.global	_ZTS16USBCommunication
 307              		.section	.rodata._ZTS16USBCommunication,"a"
 308              		.align	2
 311              	_ZTS16USBCommunication:
 312 0000 31365553 		.ascii	"16USBCommunication\000"
 312      42436F6D 
 312      6D756E69 
 312      63617469 
 312      6F6E00
 313              		.weak	_ZTI13Communication
 314              		.section	.rodata._ZTI13Communication,"aG",%progbits,_ZTI13Communication,comdat
 315              		.align	2
 318              	_ZTI13Communication:
 319 0000 08000000 		.word	_ZTVN10__cxxabiv117__class_type_infoE+8
 320 0004 00000000 		.word	_ZTS13Communication
 321              		.weak	_ZTS13Communication
 322              		.section	.rodata._ZTS13Communication,"aG",%progbits,_ZTS13Communication,comdat
 323              		.align	2
 326              	_ZTS13Communication:
 327 0000 3133436F 		.ascii	"13Communication\000"
 327      6D6D756E 
 327      69636174 
 327      696F6E00 
 328              		.text
 329              	.Letext0:
 330              		.file 3 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 331              		.file 4 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 332              		.file 5 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdint.h"
 333              		.file 6 "motorlib/messages.h"
 334              		.file 7 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdint"
 335              		.file 8 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstring"
ARM GAS  /tmp/ccEBsJev.s 			page 8


 336              		.file 9 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thu
 337              		.file 10 "/opt/gcc-arm-none-eabi-8-2019-q3-update/lib/gcc/arm-none-eabi/8.3.1/include/stddef.h"
 338              		.file 11 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_types.h"
 339              		.file 12 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/reent.h"
 340              		.file 13 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/lock.h"
 341              		.file 14 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/string.h"
 342              		.file 15 "motorlib/usb_communication.h"
 343              		.file 16 "motorlib/communication.h"
 344              		.file 17 "<built-in>"
ARM GAS  /tmp/ccEBsJev.s 			page 9


DEFINED SYMBOLS
                            *ABS*:0000000000000000 usb_communication.cpp
     /tmp/ccEBsJev.s:18     .text._ZN4USB111send_data32EhPKmh:0000000000000000 $t
     /tmp/ccEBsJev.s:26     .text._ZN4USB111send_data32EhPKmh:0000000000000000 _ZN4USB111send_data32EhPKmh
.ARM.exidx.text._ZN4USB111send_data32EhPKmh:0000000000000000 $d
     /tmp/ccEBsJev.s:72     .text._ZN4USB19send_dataEhPKhh:0000000000000000 $t
     /tmp/ccEBsJev.s:79     .text._ZN4USB19send_dataEhPKhh:0000000000000000 _ZN4USB19send_dataEhPKhh
.ARM.exidx.text._ZN4USB19send_dataEhPKhh:0000000000000000 $d
     /tmp/ccEBsJev.s:135    .text._ZN16USBCommunication4initEv:0000000000000000 $t
     /tmp/ccEBsJev.s:142    .text._ZN16USBCommunication4initEv:0000000000000000 _ZN16USBCommunication4initEv
.ARM.extab.text._ZN16USBCommunication4initEv:0000000000000000 $d
.ARM.exidx.text._ZN16USBCommunication4initEv:0000000000000000 $d
     /tmp/ccEBsJev.s:188    .text._ZN16USBCommunication12receive_dataEP11ReceiveData:0000000000000000 $t
     /tmp/ccEBsJev.s:195    .text._ZN16USBCommunication12receive_dataEP11ReceiveData:0000000000000000 _ZN16USBCommunication12receive_dataEP11ReceiveData
.ARM.exidx.text._ZN16USBCommunication12receive_dataEP11ReceiveData:0000000000000000 $d
     /tmp/ccEBsJev.s:236    .text._ZN16USBCommunication9send_dataERK8SendData:0000000000000000 $t
     /tmp/ccEBsJev.s:243    .text._ZN16USBCommunication9send_dataERK8SendData:0000000000000000 _ZN16USBCommunication9send_dataERK8SendData
.ARM.exidx.text._ZN16USBCommunication9send_dataERK8SendData:0000000000000000 $d
     /tmp/ccEBsJev.s:291    .rodata._ZTV16USBCommunication:0000000000000000 _ZTV16USBCommunication
     /tmp/ccEBsJev.s:288    .rodata._ZTV16USBCommunication:0000000000000000 $d
     /tmp/ccEBsJev.s:302    .rodata._ZTI16USBCommunication:0000000000000000 _ZTI16USBCommunication
     /tmp/ccEBsJev.s:299    .rodata._ZTI16USBCommunication:0000000000000000 $d
     /tmp/ccEBsJev.s:311    .rodata._ZTS16USBCommunication:0000000000000000 _ZTS16USBCommunication
     /tmp/ccEBsJev.s:318    .rodata._ZTI13Communication:0000000000000000 _ZTI13Communication
     /tmp/ccEBsJev.s:308    .rodata._ZTS16USBCommunication:0000000000000000 $d
     /tmp/ccEBsJev.s:315    .rodata._ZTI13Communication:0000000000000000 $d
     /tmp/ccEBsJev.s:326    .rodata._ZTS13Communication:0000000000000000 _ZTS13Communication
     /tmp/ccEBsJev.s:323    .rodata._ZTS13Communication:0000000000000000 $d

UNDEFINED SYMBOLS
_Znwj
__aeabi_unwind_cpp_pr1
_ZTVN10__cxxabiv120__si_class_type_infoE
_ZTVN10__cxxabiv117__class_type_infoE
