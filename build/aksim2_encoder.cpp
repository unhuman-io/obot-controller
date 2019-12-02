ARM GAS  /tmp/ccA3Xxom.s 			page 1


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
  13              		.file	"aksim2_encoder.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.ccmram,"ax",%progbits
  18              		.align	1
  19              		.global	_ZN13Aksim2Encoder9get_valueEv
  20              		.arch armv7e-m
  21              		.syntax unified
  22              		.thumb
  23              		.thumb_func
  24              		.fpu fpv4-sp-d16
  26              	_ZN13Aksim2Encoder9get_valueEv:
  27              		.fnstart
  28              	.LFB143:
  29              		.file 1 "motorlib/aksim2_encoder.cpp"
   1:motorlib/aksim2_encoder.cpp **** #include "aksim2_encoder.h"
   2:motorlib/aksim2_encoder.cpp **** 
   3:motorlib/aksim2_encoder.cpp **** void Aksim2Encoder::init() {
   4:motorlib/aksim2_encoder.cpp ****     regs_.CR2 |= SPI_CR2_RXDMAEN;
   5:motorlib/aksim2_encoder.cpp **** 
   6:motorlib/aksim2_encoder.cpp ****     // init dma: dma2 stream2 ch3 spi_rx, dma2 stream3 ch3 spi_tx
   7:motorlib/aksim2_encoder.cpp ****     // no fifo, half word
   8:motorlib/aksim2_encoder.cpp ****     DMA1_Channel1->CCR &= ~DMA_CCR_EN;
   9:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CCR &= ~DMA_CCR_EN;
  10:motorlib/aksim2_encoder.cpp ****     
  11:motorlib/aksim2_encoder.cpp ****     DMA1_Channel1->CMAR = reinterpret_cast<uint32_t>(rx_data_);
  12:motorlib/aksim2_encoder.cpp ****     DMA1_Channel1->CPAR = reinterpret_cast<uint32_t>(&regs_.DR);
  13:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CMAR = reinterpret_cast<uint32_t>(tx_data_);
  14:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CPAR = reinterpret_cast<uint32_t>(&regs_.DR);
  15:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream2->CR &= ~DMA_SxCR_EN;
  16:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream2->CR = (3 << DMA_SxCR_CHSEL_Pos) | DMA_SxCR_MSIZE_0 | DMA_SxCR_PSIZE_0 | DMA_SxC
  17:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream2->PAR = reinterpret_cast<uint32_t>(&regs_.DR);
  18:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream2->M0AR = reinterpret_cast<uint32_t>(rx_data_);
  19:motorlib/aksim2_encoder.cpp **** 
  20:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream3->CR &= ~DMA_SxCR_EN;
  21:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream3->CR = (3 << DMA_SxCR_CHSEL_Pos) | DMA_SxCR_MSIZE_0 | DMA_SxCR_PSIZE_0 | DMA_SxC
  22:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream3->PAR = reinterpret_cast<uint32_t>(&regs_.DR);
  23:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream3->M0AR = reinterpret_cast<uint32_t>(tx_data_);
  24:motorlib/aksim2_encoder.cpp **** 
  25:motorlib/aksim2_encoder.cpp ****     regs_.CR2 |= SPI_CR2_TXDMAEN;
  26:motorlib/aksim2_encoder.cpp **** }
  27:motorlib/aksim2_encoder.cpp **** 
  28:motorlib/aksim2_encoder.cpp **** void Aksim2Encoder::trigger() {
  29:motorlib/aksim2_encoder.cpp ****     if (!init_) {
ARM GAS  /tmp/ccA3Xxom.s 			page 2


  30:motorlib/aksim2_encoder.cpp ****         init();
  31:motorlib/aksim2_encoder.cpp ****         init_ = true;
  32:motorlib/aksim2_encoder.cpp ****     }
  33:motorlib/aksim2_encoder.cpp ****     // EN == 0 if not transferring, TCIF also means transfer complete
  34:motorlib/aksim2_encoder.cpp ****     // Stream 2 is RX, Stream 3 is TX
  35:motorlib/aksim2_encoder.cpp ****     if (!(DMA1_Channel1->CCR & DMA_CCR_EN) || (DMA1->ISR & DMA_ISR_TCIF1)) {
  36:motorlib/aksim2_encoder.cpp ****         if (++count > 2) {  // A delay. TODO time based delay
  37:motorlib/aksim2_encoder.cpp ****             count = 0;
  38:motorlib/aksim2_encoder.cpp ****             DMA1_Channel1->CCR = DMA1_Channel1->CCR & ~DMA_CCR_EN;
  39:motorlib/aksim2_encoder.cpp ****             DMA1_Channel2->CCR = DMA1_Channel2->CCR & ~DMA_CCR_EN;
  40:motorlib/aksim2_encoder.cpp ****           //  DMA1_Channel2->CCR &= DMA_CCR_EN;
  41:motorlib/aksim2_encoder.cpp ****             DMA1->IFCR =DMA_IFCR_CTCIF1 | DMA_IFCR_CHTIF1 | DMA_IFCR_CTCIF2 | DMA_IFCR_CHTIF2;
  42:motorlib/aksim2_encoder.cpp ****             DMA1_Channel1->CNDTR = 4;
  43:motorlib/aksim2_encoder.cpp ****             DMA1_Channel2->CNDTR = 4;
  44:motorlib/aksim2_encoder.cpp ****             DMA1_Channel1->CCR |= DMA_CCR_EN;
  45:motorlib/aksim2_encoder.cpp ****             DMA1_Channel2->CCR |= DMA_CCR_EN;
  46:motorlib/aksim2_encoder.cpp ****            // DMA2_Stream2->NDTR = 4;
  47:motorlib/aksim2_encoder.cpp ****            // DMA2_Stream3->NDTR = 4;
  48:motorlib/aksim2_encoder.cpp ****             //DMA2_Stream2->CR |= DMA_SxCR_EN;
  49:motorlib/aksim2_encoder.cpp ****             //DMA2_Stream3->CR |= DMA_SxCR_EN;
  50:motorlib/aksim2_encoder.cpp **** 
  51:motorlib/aksim2_encoder.cpp ****             // current aksim2 encoders are 18 bit with multiturn, data is in bits 32:49
  52:motorlib/aksim2_encoder.cpp ****             raw_value_ = (static_cast<uint32_t>(rx_data_[2] & 0x7FFF) << 3) | (rx_data_[3] >> 13);
  53:motorlib/aksim2_encoder.cpp ****             
  54:motorlib/aksim2_encoder.cpp ****             int32_t diff = raw_value_ - last_raw_value_;
  55:motorlib/aksim2_encoder.cpp ****             last_raw_value_ = raw_value_;
  56:motorlib/aksim2_encoder.cpp **** 
  57:motorlib/aksim2_encoder.cpp ****             if (diff > (262144 >> 1)) {
  58:motorlib/aksim2_encoder.cpp ****                 diff += -262144;
  59:motorlib/aksim2_encoder.cpp ****             } else if (diff < -(262144 >> 1)) {
  60:motorlib/aksim2_encoder.cpp ****                 diff += 262144;
  61:motorlib/aksim2_encoder.cpp ****             }
  62:motorlib/aksim2_encoder.cpp ****             value_ += diff;
  63:motorlib/aksim2_encoder.cpp ****         }
  64:motorlib/aksim2_encoder.cpp ****     }
  65:motorlib/aksim2_encoder.cpp **** }
  66:motorlib/aksim2_encoder.cpp **** 
  67:motorlib/aksim2_encoder.cpp **** int32_t Aksim2Encoder::get_value() {
  30              		.loc 1 67 36
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
  68:motorlib/aksim2_encoder.cpp ****     return value_;
  46              		.loc 1 68 12
  47 0008 7B68     		ldr	r3, [r7, #4]
ARM GAS  /tmp/ccA3Xxom.s 			page 3


  48 000a DB69     		ldr	r3, [r3, #28]
  69:motorlib/aksim2_encoder.cpp ****  }
  49              		.loc 1 69 2
  50 000c 1846     		mov	r0, r3
  51 000e 0C37     		adds	r7, r7, #12
  52              	.LCFI3:
  53              		.cfi_def_cfa_offset 4
  54 0010 BD46     		mov	sp, r7
  55              	.LCFI4:
  56              		.cfi_def_cfa_register 13
  57              		@ sp needed
  58 0012 5DF8047B 		ldr	r7, [sp], #4
  59              	.LCFI5:
  60              		.cfi_restore 7
  61              		.cfi_def_cfa_offset 0
  62 0016 7047     		bx	lr
  63              		.cfi_endproc
  64              	.LFE143:
  65              		.cantunwind
  66              		.fnend
  68              		.align	1
  69              		.global	_ZN13Aksim2Encoder7triggerEv
  70              		.syntax unified
  71              		.thumb
  72              		.thumb_func
  73              		.fpu fpv4-sp-d16
  75              	_ZN13Aksim2Encoder7triggerEv:
  76              		.fnstart
  77              	.LFB142:
  28:motorlib/aksim2_encoder.cpp ****     if (!init_) {
  78              		.loc 1 28 31
  79              		.cfi_startproc
  80              		@ args = 0, pretend = 0, frame = 16
  81              		@ frame_needed = 1, uses_anonymous_args = 0
  82 0018 80B5     		push	{r7, lr}
  83              	.LCFI6:
  84              		.cfi_def_cfa_offset 8
  85              		.cfi_offset 7, -8
  86              		.cfi_offset 14, -4
  87 001a 84B0     		sub	sp, sp, #16
  88              	.LCFI7:
  89              		.cfi_def_cfa_offset 24
  90 001c 00AF     		add	r7, sp, #0
  91              	.LCFI8:
  92              		.cfi_def_cfa_register 7
  93 001e 7860     		str	r0, [r7, #4]
  29:motorlib/aksim2_encoder.cpp ****         init();
  94              		.loc 1 29 10
  95 0020 7B68     		ldr	r3, [r7, #4]
  96 0022 93F83030 		ldrb	r3, [r3, #48]	@ zero_extendqisi2
  29:motorlib/aksim2_encoder.cpp ****         init();
  97              		.loc 1 29 9
  98 0026 83F00103 		eor	r3, r3, #1
  99 002a DBB2     		uxtb	r3, r3
  29:motorlib/aksim2_encoder.cpp ****         init();
 100              		.loc 1 29 5
 101 002c 002B     		cmp	r3, #0
ARM GAS  /tmp/ccA3Xxom.s 			page 4


 102 002e 06D0     		beq	.L4
  30:motorlib/aksim2_encoder.cpp ****         init_ = true;
 103              		.loc 1 30 13
 104 0030 7868     		ldr	r0, [r7, #4]
 105 0032 FFF7FEFF 		bl	_ZN13Aksim2Encoder4initEv
  31:motorlib/aksim2_encoder.cpp ****     }
 106              		.loc 1 31 15
 107 0036 7B68     		ldr	r3, [r7, #4]
 108 0038 0122     		movs	r2, #1
 109 003a 83F83020 		strb	r2, [r3, #48]
 110              	.L4:
 111              	.LBB2:
  35:motorlib/aksim2_encoder.cpp ****         if (++count > 2) {  // A delay. TODO time based delay
 112              		.loc 1 35 26
 113 003e 394B     		ldr	r3, .L13
 114 0040 1B68     		ldr	r3, [r3]
  35:motorlib/aksim2_encoder.cpp ****         if (++count > 2) {  // A delay. TODO time based delay
 115              		.loc 1 35 30
 116 0042 03F00103 		and	r3, r3, #1
  35:motorlib/aksim2_encoder.cpp ****         if (++count > 2) {  // A delay. TODO time based delay
 117              		.loc 1 35 44
 118 0046 002B     		cmp	r3, #0
 119 0048 05D0     		beq	.L5
  35:motorlib/aksim2_encoder.cpp ****         if (++count > 2) {  // A delay. TODO time based delay
 120              		.loc 1 35 54 discriminator 2
 121 004a 374B     		ldr	r3, .L13+4
 122 004c 1B68     		ldr	r3, [r3]
  35:motorlib/aksim2_encoder.cpp ****         if (++count > 2) {  // A delay. TODO time based delay
 123              		.loc 1 35 58 discriminator 2
 124 004e 03F00203 		and	r3, r3, #2
  35:motorlib/aksim2_encoder.cpp ****         if (++count > 2) {  // A delay. TODO time based delay
 125              		.loc 1 35 44 discriminator 2
 126 0052 002B     		cmp	r3, #0
 127 0054 01D0     		beq	.L6
 128              	.L5:
  35:motorlib/aksim2_encoder.cpp ****         if (++count > 2) {  // A delay. TODO time based delay
 129              		.loc 1 35 44 is_stmt 0 discriminator 3
 130 0056 0123     		movs	r3, #1
 131 0058 00E0     		b	.L7
 132              	.L6:
  35:motorlib/aksim2_encoder.cpp ****         if (++count > 2) {  // A delay. TODO time based delay
 133              		.loc 1 35 44 discriminator 4
 134 005a 0023     		movs	r3, #0
 135              	.L7:
  35:motorlib/aksim2_encoder.cpp ****         if (++count > 2) {  // A delay. TODO time based delay
 136              		.loc 1 35 5 is_stmt 1 discriminator 6
 137 005c 002B     		cmp	r3, #0
 138 005e 5DD0     		beq	.L12
 139              	.LBB3:
 140              	.LBB4:
  36:motorlib/aksim2_encoder.cpp ****             count = 0;
 141              		.loc 1 36 15
 142 0060 7B68     		ldr	r3, [r7, #4]
 143 0062 1B69     		ldr	r3, [r3, #16]
  36:motorlib/aksim2_encoder.cpp ****             count = 0;
 144              		.loc 1 36 13
 145 0064 5A1C     		adds	r2, r3, #1
ARM GAS  /tmp/ccA3Xxom.s 			page 5


  36:motorlib/aksim2_encoder.cpp ****             count = 0;
 146              		.loc 1 36 21
 147 0066 7B68     		ldr	r3, [r7, #4]
 148 0068 1A61     		str	r2, [r3, #16]
  36:motorlib/aksim2_encoder.cpp ****             count = 0;
 149              		.loc 1 36 15
 150 006a 7B68     		ldr	r3, [r7, #4]
 151 006c 1B69     		ldr	r3, [r3, #16]
  36:motorlib/aksim2_encoder.cpp ****             count = 0;
 152              		.loc 1 36 21
 153 006e 022B     		cmp	r3, #2
 154 0070 CCBF     		ite	gt
 155 0072 0123     		movgt	r3, #1
 156 0074 0023     		movle	r3, #0
 157 0076 DBB2     		uxtb	r3, r3
  36:motorlib/aksim2_encoder.cpp ****             count = 0;
 158              		.loc 1 36 9
 159 0078 002B     		cmp	r3, #0
 160 007a 4FD0     		beq	.L12
 161              	.LBB5:
  37:motorlib/aksim2_encoder.cpp ****             DMA1_Channel1->CCR = DMA1_Channel1->CCR & ~DMA_CCR_EN;
 162              		.loc 1 37 19
 163 007c 7B68     		ldr	r3, [r7, #4]
 164 007e 0022     		movs	r2, #0
 165 0080 1A61     		str	r2, [r3, #16]
  38:motorlib/aksim2_encoder.cpp ****             DMA1_Channel2->CCR = DMA1_Channel2->CCR & ~DMA_CCR_EN;
 166              		.loc 1 38 49
 167 0082 284B     		ldr	r3, .L13
 168 0084 1B68     		ldr	r3, [r3]
  38:motorlib/aksim2_encoder.cpp ****             DMA1_Channel2->CCR = DMA1_Channel2->CCR & ~DMA_CCR_EN;
 169              		.loc 1 38 28
 170 0086 274A     		ldr	r2, .L13
  38:motorlib/aksim2_encoder.cpp ****             DMA1_Channel2->CCR = DMA1_Channel2->CCR & ~DMA_CCR_EN;
 171              		.loc 1 38 53
 172 0088 23F00103 		bic	r3, r3, #1
  38:motorlib/aksim2_encoder.cpp ****             DMA1_Channel2->CCR = DMA1_Channel2->CCR & ~DMA_CCR_EN;
 173              		.loc 1 38 32
 174 008c 1360     		str	r3, [r2]
  39:motorlib/aksim2_encoder.cpp ****           //  DMA1_Channel2->CCR &= DMA_CCR_EN;
 175              		.loc 1 39 49
 176 008e 274B     		ldr	r3, .L13+8
 177 0090 1B68     		ldr	r3, [r3]
  39:motorlib/aksim2_encoder.cpp ****           //  DMA1_Channel2->CCR &= DMA_CCR_EN;
 178              		.loc 1 39 28
 179 0092 264A     		ldr	r2, .L13+8
  39:motorlib/aksim2_encoder.cpp ****           //  DMA1_Channel2->CCR &= DMA_CCR_EN;
 180              		.loc 1 39 53
 181 0094 23F00103 		bic	r3, r3, #1
  39:motorlib/aksim2_encoder.cpp ****           //  DMA1_Channel2->CCR &= DMA_CCR_EN;
 182              		.loc 1 39 32
 183 0098 1360     		str	r3, [r2]
  41:motorlib/aksim2_encoder.cpp ****             DMA1_Channel1->CNDTR = 4;
 184              		.loc 1 41 19
 185 009a 234B     		ldr	r3, .L13+4
  41:motorlib/aksim2_encoder.cpp ****             DMA1_Channel1->CNDTR = 4;
 186              		.loc 1 41 24
 187 009c 6622     		movs	r2, #102
ARM GAS  /tmp/ccA3Xxom.s 			page 6


 188 009e 5A60     		str	r2, [r3, #4]
  42:motorlib/aksim2_encoder.cpp ****             DMA1_Channel2->CNDTR = 4;
 189              		.loc 1 42 28
 190 00a0 204B     		ldr	r3, .L13
  42:motorlib/aksim2_encoder.cpp ****             DMA1_Channel2->CNDTR = 4;
 191              		.loc 1 42 34
 192 00a2 0422     		movs	r2, #4
 193 00a4 5A60     		str	r2, [r3, #4]
  43:motorlib/aksim2_encoder.cpp ****             DMA1_Channel1->CCR |= DMA_CCR_EN;
 194              		.loc 1 43 28
 195 00a6 214B     		ldr	r3, .L13+8
  43:motorlib/aksim2_encoder.cpp ****             DMA1_Channel1->CCR |= DMA_CCR_EN;
 196              		.loc 1 43 34
 197 00a8 0422     		movs	r2, #4
 198 00aa 5A60     		str	r2, [r3, #4]
  44:motorlib/aksim2_encoder.cpp ****             DMA1_Channel2->CCR |= DMA_CCR_EN;
 199              		.loc 1 44 32
 200 00ac 1D4B     		ldr	r3, .L13
 201 00ae 1B68     		ldr	r3, [r3]
 202 00b0 1C4A     		ldr	r2, .L13
 203 00b2 43F00103 		orr	r3, r3, #1
 204 00b6 1360     		str	r3, [r2]
  45:motorlib/aksim2_encoder.cpp ****            // DMA2_Stream2->NDTR = 4;
 205              		.loc 1 45 32
 206 00b8 1C4B     		ldr	r3, .L13+8
 207 00ba 1B68     		ldr	r3, [r3]
 208 00bc 1B4A     		ldr	r2, .L13+8
 209 00be 43F00103 		orr	r3, r3, #1
 210 00c2 1360     		str	r3, [r2]
  52:motorlib/aksim2_encoder.cpp ****             
 211              		.loc 1 52 59
 212 00c4 7B68     		ldr	r3, [r7, #4]
 213 00c6 9B8C     		ldrh	r3, [r3, #36]
  52:motorlib/aksim2_encoder.cpp ****             
 214              		.loc 1 52 71
 215 00c8 DA00     		lsls	r2, r3, #3
 216 00ca 194B     		ldr	r3, .L13+12
 217 00cc 1340     		ands	r3, r3, r2
  52:motorlib/aksim2_encoder.cpp ****             
 218              		.loc 1 52 90
 219 00ce 7A68     		ldr	r2, [r7, #4]
 220 00d0 D28C     		ldrh	r2, [r2, #38]
  52:motorlib/aksim2_encoder.cpp ****             
 221              		.loc 1 52 92
 222 00d2 5213     		asrs	r2, r2, #13
  52:motorlib/aksim2_encoder.cpp ****             
 223              		.loc 1 52 77
 224 00d4 1A43     		orrs	r2, r2, r3
  52:motorlib/aksim2_encoder.cpp ****             
 225              		.loc 1 52 24
 226 00d6 7B68     		ldr	r3, [r7, #4]
 227 00d8 5A61     		str	r2, [r3, #20]
  54:motorlib/aksim2_encoder.cpp ****             last_raw_value_ = raw_value_;
 228              		.loc 1 54 28
 229 00da 7B68     		ldr	r3, [r7, #4]
 230 00dc 5A69     		ldr	r2, [r3, #20]
  54:motorlib/aksim2_encoder.cpp ****             last_raw_value_ = raw_value_;
ARM GAS  /tmp/ccA3Xxom.s 			page 7


 231              		.loc 1 54 41
 232 00de 7B68     		ldr	r3, [r7, #4]
 233 00e0 9B69     		ldr	r3, [r3, #24]
  54:motorlib/aksim2_encoder.cpp ****             last_raw_value_ = raw_value_;
 234              		.loc 1 54 39
 235 00e2 D31A     		subs	r3, r2, r3
  54:motorlib/aksim2_encoder.cpp ****             last_raw_value_ = raw_value_;
 236              		.loc 1 54 21
 237 00e4 FB60     		str	r3, [r7, #12]
  55:motorlib/aksim2_encoder.cpp **** 
 238              		.loc 1 55 31
 239 00e6 7B68     		ldr	r3, [r7, #4]
 240 00e8 5A69     		ldr	r2, [r3, #20]
  55:motorlib/aksim2_encoder.cpp **** 
 241              		.loc 1 55 29
 242 00ea 7B68     		ldr	r3, [r7, #4]
 243 00ec 9A61     		str	r2, [r3, #24]
  57:motorlib/aksim2_encoder.cpp ****                 diff += -262144;
 244              		.loc 1 57 13
 245 00ee FB68     		ldr	r3, [r7, #12]
 246 00f0 B3F5003F 		cmp	r3, #131072
 247 00f4 04DD     		ble	.L10
  58:motorlib/aksim2_encoder.cpp ****             } else if (diff < -(262144 >> 1)) {
 248              		.loc 1 58 22
 249 00f6 FB68     		ldr	r3, [r7, #12]
 250 00f8 A3F58023 		sub	r3, r3, #262144
 251 00fc FB60     		str	r3, [r7, #12]
 252 00fe 07E0     		b	.L11
 253              	.L10:
  59:motorlib/aksim2_encoder.cpp ****                 diff += 262144;
 254              		.loc 1 59 20
 255 0100 FB68     		ldr	r3, [r7, #12]
 256 0102 13F5003F 		cmn	r3, #131072
 257 0106 03DA     		bge	.L11
  60:motorlib/aksim2_encoder.cpp ****             }
 258              		.loc 1 60 22
 259 0108 FB68     		ldr	r3, [r7, #12]
 260 010a 03F58023 		add	r3, r3, #262144
 261 010e FB60     		str	r3, [r7, #12]
 262              	.L11:
  62:motorlib/aksim2_encoder.cpp ****         }
 263              		.loc 1 62 20
 264 0110 7B68     		ldr	r3, [r7, #4]
 265 0112 DA69     		ldr	r2, [r3, #28]
 266 0114 FB68     		ldr	r3, [r7, #12]
 267 0116 1A44     		add	r2, r2, r3
 268 0118 7B68     		ldr	r3, [r7, #4]
 269 011a DA61     		str	r2, [r3, #28]
 270              	.L12:
 271              	.LBE5:
 272              	.LBE4:
 273              	.LBE3:
 274              	.LBE2:
  65:motorlib/aksim2_encoder.cpp **** 
 275              		.loc 1 65 1
 276 011c 00BF     		nop
 277 011e 1037     		adds	r7, r7, #16
ARM GAS  /tmp/ccA3Xxom.s 			page 8


 278              	.LCFI9:
 279              		.cfi_def_cfa_offset 8
 280 0120 BD46     		mov	sp, r7
 281              	.LCFI10:
 282              		.cfi_def_cfa_register 13
 283              		@ sp needed
 284 0122 80BD     		pop	{r7, pc}
 285              	.L14:
 286              		.align	2
 287              	.L13:
 288 0124 08000240 		.word	1073872904
 289 0128 00000240 		.word	1073872896
 290 012c 1C000240 		.word	1073872924
 291 0130 F8FF0300 		.word	262136
 292              		.cfi_endproc
 293              	.LFE142:
 294              		.cantunwind
 295              		.fnend
 297              		.section	.text._ZN13Aksim2Encoder4initEv,"ax",%progbits
 298              		.align	1
 299              		.global	_ZN13Aksim2Encoder4initEv
 300              		.syntax unified
 301              		.thumb
 302              		.thumb_func
 303              		.fpu fpv4-sp-d16
 305              	_ZN13Aksim2Encoder4initEv:
 306              		.fnstart
 307              	.LFB141:
   3:motorlib/aksim2_encoder.cpp ****     regs_.CR2 |= SPI_CR2_RXDMAEN;
 308              		.loc 1 3 28
 309              		.cfi_startproc
 310              		@ args = 0, pretend = 0, frame = 8
 311              		@ frame_needed = 1, uses_anonymous_args = 0
 312              		@ link register save eliminated.
 313 0000 80B4     		push	{r7}
 314              	.LCFI11:
 315              		.cfi_def_cfa_offset 4
 316              		.cfi_offset 7, -4
 317 0002 83B0     		sub	sp, sp, #12
 318              	.LCFI12:
 319              		.cfi_def_cfa_offset 16
 320 0004 00AF     		add	r7, sp, #0
 321              	.LCFI13:
 322              		.cfi_def_cfa_register 7
 323 0006 7860     		str	r0, [r7, #4]
   4:motorlib/aksim2_encoder.cpp **** 
 324              		.loc 1 4 5
 325 0008 7B68     		ldr	r3, [r7, #4]
 326 000a 9B68     		ldr	r3, [r3, #8]
   4:motorlib/aksim2_encoder.cpp **** 
 327              		.loc 1 4 15
 328 000c 5A68     		ldr	r2, [r3, #4]
   4:motorlib/aksim2_encoder.cpp **** 
 329              		.loc 1 4 5
 330 000e 7B68     		ldr	r3, [r7, #4]
 331 0010 9B68     		ldr	r3, [r3, #8]
   4:motorlib/aksim2_encoder.cpp **** 
ARM GAS  /tmp/ccA3Xxom.s 			page 9


 332              		.loc 1 4 15
 333 0012 42F00102 		orr	r2, r2, #1
 334 0016 5A60     		str	r2, [r3, #4]
   8:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CCR &= ~DMA_CCR_EN;
 335              		.loc 1 8 24
 336 0018 174B     		ldr	r3, .L16
 337 001a 1B68     		ldr	r3, [r3]
 338 001c 164A     		ldr	r2, .L16
 339 001e 23F00103 		bic	r3, r3, #1
 340 0022 1360     		str	r3, [r2]
   9:motorlib/aksim2_encoder.cpp ****     
 341              		.loc 1 9 24
 342 0024 154B     		ldr	r3, .L16+4
 343 0026 1B68     		ldr	r3, [r3]
 344 0028 144A     		ldr	r2, .L16+4
 345 002a 23F00103 		bic	r3, r3, #1
 346 002e 1360     		str	r3, [r2]
  11:motorlib/aksim2_encoder.cpp ****     DMA1_Channel1->CPAR = reinterpret_cast<uint32_t>(&regs_.DR);
 347              		.loc 1 11 54
 348 0030 7B68     		ldr	r3, [r7, #4]
 349 0032 03F12002 		add	r2, r3, #32
  11:motorlib/aksim2_encoder.cpp ****     DMA1_Channel1->CPAR = reinterpret_cast<uint32_t>(&regs_.DR);
 350              		.loc 1 11 20
 351 0036 104B     		ldr	r3, .L16
  11:motorlib/aksim2_encoder.cpp ****     DMA1_Channel1->CPAR = reinterpret_cast<uint32_t>(&regs_.DR);
 352              		.loc 1 11 25
 353 0038 DA60     		str	r2, [r3, #12]
  12:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CMAR = reinterpret_cast<uint32_t>(tx_data_);
 354              		.loc 1 12 55
 355 003a 7B68     		ldr	r3, [r7, #4]
 356 003c 9B68     		ldr	r3, [r3, #8]
  12:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CMAR = reinterpret_cast<uint32_t>(tx_data_);
 357              		.loc 1 12 54
 358 003e 03F10C02 		add	r2, r3, #12
  12:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CMAR = reinterpret_cast<uint32_t>(tx_data_);
 359              		.loc 1 12 20
 360 0042 0D4B     		ldr	r3, .L16
  12:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CMAR = reinterpret_cast<uint32_t>(tx_data_);
 361              		.loc 1 12 25
 362 0044 9A60     		str	r2, [r3, #8]
  13:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CPAR = reinterpret_cast<uint32_t>(&regs_.DR);
 363              		.loc 1 13 54
 364 0046 7B68     		ldr	r3, [r7, #4]
 365 0048 03F12802 		add	r2, r3, #40
  13:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CPAR = reinterpret_cast<uint32_t>(&regs_.DR);
 366              		.loc 1 13 20
 367 004c 0B4B     		ldr	r3, .L16+4
  13:motorlib/aksim2_encoder.cpp ****     DMA1_Channel2->CPAR = reinterpret_cast<uint32_t>(&regs_.DR);
 368              		.loc 1 13 25
 369 004e DA60     		str	r2, [r3, #12]
  14:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream2->CR &= ~DMA_SxCR_EN;
 370              		.loc 1 14 55
 371 0050 7B68     		ldr	r3, [r7, #4]
 372 0052 9B68     		ldr	r3, [r3, #8]
  14:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream2->CR &= ~DMA_SxCR_EN;
 373              		.loc 1 14 54
 374 0054 03F10C02 		add	r2, r3, #12
ARM GAS  /tmp/ccA3Xxom.s 			page 10


  14:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream2->CR &= ~DMA_SxCR_EN;
 375              		.loc 1 14 20
 376 0058 084B     		ldr	r3, .L16+4
  14:motorlib/aksim2_encoder.cpp ****     // DMA2_Stream2->CR &= ~DMA_SxCR_EN;
 377              		.loc 1 14 25
 378 005a 9A60     		str	r2, [r3, #8]
  25:motorlib/aksim2_encoder.cpp **** }
 379              		.loc 1 25 5
 380 005c 7B68     		ldr	r3, [r7, #4]
 381 005e 9B68     		ldr	r3, [r3, #8]
  25:motorlib/aksim2_encoder.cpp **** }
 382              		.loc 1 25 15
 383 0060 5A68     		ldr	r2, [r3, #4]
  25:motorlib/aksim2_encoder.cpp **** }
 384              		.loc 1 25 5
 385 0062 7B68     		ldr	r3, [r7, #4]
 386 0064 9B68     		ldr	r3, [r3, #8]
  25:motorlib/aksim2_encoder.cpp **** }
 387              		.loc 1 25 15
 388 0066 42F00202 		orr	r2, r2, #2
 389 006a 5A60     		str	r2, [r3, #4]
  26:motorlib/aksim2_encoder.cpp **** 
 390              		.loc 1 26 1
 391 006c 00BF     		nop
 392 006e 0C37     		adds	r7, r7, #12
 393              	.LCFI14:
 394              		.cfi_def_cfa_offset 4
 395 0070 BD46     		mov	sp, r7
 396              	.LCFI15:
 397              		.cfi_def_cfa_register 13
 398              		@ sp needed
 399 0072 5DF8047B 		ldr	r7, [sp], #4
 400              	.LCFI16:
 401              		.cfi_restore 7
 402              		.cfi_def_cfa_offset 0
 403 0076 7047     		bx	lr
 404              	.L17:
 405              		.align	2
 406              	.L16:
 407 0078 08000240 		.word	1073872904
 408 007c 1C000240 		.word	1073872924
 409              		.cfi_endproc
 410              	.LFE141:
 411              		.cantunwind
 412              		.fnend
 414              		.global	_ZTV13Aksim2Encoder
 415              		.section	.rodata._ZTV13Aksim2Encoder,"a"
 416              		.align	2
 419              	_ZTV13Aksim2Encoder:
 420 0000 00000000 		.word	0
 421 0004 00000000 		.word	_ZTI13Aksim2Encoder
 422 0008 00000000 		.word	_ZN13Aksim2Encoder9get_valueEv
 423 000c 00000000 		.word	_ZN13Aksim2Encoder7triggerEv
 424              		.global	_ZTI13Aksim2Encoder
 425              		.section	.rodata._ZTI13Aksim2Encoder,"a"
 426              		.align	2
 429              	_ZTI13Aksim2Encoder:
ARM GAS  /tmp/ccA3Xxom.s 			page 11


 430 0000 08000000 		.word	_ZTVN10__cxxabiv120__si_class_type_infoE+8
 431 0004 00000000 		.word	_ZTS13Aksim2Encoder
 432 0008 00000000 		.word	_ZTI10SPIEncoder
 433              		.global	_ZTS13Aksim2Encoder
 434              		.section	.rodata._ZTS13Aksim2Encoder,"a"
 435              		.align	2
 438              	_ZTS13Aksim2Encoder:
 439 0000 3133416B 		.ascii	"13Aksim2Encoder\000"
 439      73696D32 
 439      456E636F 
 439      64657200 
 440              		.text
 441              	.Letext0:
 442              		.file 2 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdint"
 443              		.file 3 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thu
 444              		.file 4 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 445              		.file 5 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 446              		.file 6 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdint.h"
 447              		.file 7 "Drivers/CMSIS/Include/core_cm4.h"
 448              		.file 8 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/system_stm32g4xx.h"
 449              		.file 9 "Drivers/CMSIS/Device/ST/STM32G4xx/Include/stm32g474xx.h"
 450              		.file 10 "motorlib/aksim2_encoder.h"
 451              		.file 11 "motorlib/encoder.h"
 452              		.file 12 "<built-in>"
ARM GAS  /tmp/ccA3Xxom.s 			page 12


DEFINED SYMBOLS
                            *ABS*:0000000000000000 aksim2_encoder.cpp
     /tmp/ccA3Xxom.s:18     .ccmram:0000000000000000 $t
     /tmp/ccA3Xxom.s:26     .ccmram:0000000000000000 _ZN13Aksim2Encoder9get_valueEv
                .ARM.exidx.ccmram:0000000000000000 $d
     /tmp/ccA3Xxom.s:75     .ccmram:0000000000000018 _ZN13Aksim2Encoder7triggerEv
     /tmp/ccA3Xxom.s:305    .text._ZN13Aksim2Encoder4initEv:0000000000000000 _ZN13Aksim2Encoder4initEv
     /tmp/ccA3Xxom.s:288    .ccmram:0000000000000124 $d
     /tmp/ccA3Xxom.s:298    .text._ZN13Aksim2Encoder4initEv:0000000000000000 $t
     /tmp/ccA3Xxom.s:407    .text._ZN13Aksim2Encoder4initEv:0000000000000078 $d
.ARM.exidx.text._ZN13Aksim2Encoder4initEv:0000000000000000 $d
     /tmp/ccA3Xxom.s:419    .rodata._ZTV13Aksim2Encoder:0000000000000000 _ZTV13Aksim2Encoder
     /tmp/ccA3Xxom.s:416    .rodata._ZTV13Aksim2Encoder:0000000000000000 $d
     /tmp/ccA3Xxom.s:429    .rodata._ZTI13Aksim2Encoder:0000000000000000 _ZTI13Aksim2Encoder
     /tmp/ccA3Xxom.s:426    .rodata._ZTI13Aksim2Encoder:0000000000000000 $d
     /tmp/ccA3Xxom.s:438    .rodata._ZTS13Aksim2Encoder:0000000000000000 _ZTS13Aksim2Encoder
     /tmp/ccA3Xxom.s:435    .rodata._ZTS13Aksim2Encoder:0000000000000000 $d

UNDEFINED SYMBOLS
_ZTVN10__cxxabiv120__si_class_type_infoE
_ZTI10SPIEncoder
