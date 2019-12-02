ARM GAS  /tmp/ccRVqFpy.s 			page 1


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
  13              		.file	"main2.cpp"
  14              		.text
  15              	.Ltext0:
  16              		.cfi_sections	.debug_frame
  17              		.section	.rodata._ZStL19piecewise_construct,"a"
  18              		.align	2
  21              	_ZStL19piecewise_construct:
  22 0000 00       		.space	1
  23              		.section	.text._ZL20__gthread_key_deletei,"ax",%progbits
  24              		.align	1
  25              		.arch armv7e-m
  26              		.syntax unified
  27              		.thumb
  28              		.thumb_func
  29              		.fpu fpv4-sp-d16
  31              	_ZL20__gthread_key_deletei:
  32              		.fnstart
  33              	.LFB587:
  34              		.file 1 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thu
   1:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Threads compatibility routines for libgcc2 and libobjc.  */
   2:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Compile this one with gcc.  */
   3:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Copyright (C) 1997-2018 Free Software Foundation, Inc.
   4:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
   5:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** This file is part of GCC.
   6:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
   7:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** GCC is free software; you can redistribute it and/or modify it under
   8:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** the terms of the GNU General Public License as published by the Free
   9:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** Software Foundation; either version 3, or (at your option) any later
  10:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** version.
  11:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  12:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** GCC is distributed in the hope that it will be useful, but WITHOUT ANY
  13:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** WARRANTY; without even the implied warranty of MERCHANTABILITY or
  14:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
  15:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** for more details.
  16:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  17:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** Under Section 7 of GPL version 3, you are granted additional
  18:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** permissions described in the GCC Runtime Library Exception, version
  19:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 3.1, as published by the Free Software Foundation.
  20:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  21:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** You should have received a copy of the GNU General Public License and
  22:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** a copy of the GCC Runtime Library Exception along with this program;
  23:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
  24:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** <http://www.gnu.org/licenses/>.  */
  25:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  26:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** #ifndef _GLIBCXX_GCC_GTHR_SINGLE_H
ARM GAS  /tmp/ccRVqFpy.s 			page 2


  27:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** #define _GLIBCXX_GCC_GTHR_SINGLE_H
  28:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  29:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Just provide compatibility for mutex handling.  */
  30:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  31:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** typedef int __gthread_key_t;
  32:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** typedef int __gthread_once_t;
  33:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** typedef int __gthread_mutex_t;
  34:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** typedef int __gthread_recursive_mutex_t;
  35:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  36:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** #define __GTHREAD_ONCE_INIT 0
  37:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** #define __GTHREAD_MUTEX_INIT 0
  38:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** #define __GTHREAD_MUTEX_INIT_FUNCTION(mx) do {} while (0)
  39:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** #define __GTHREAD_RECURSIVE_MUTEX_INIT 0
  40:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  41:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** #define _GLIBCXX_UNUSED __attribute__((__unused__))
  42:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  43:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** #ifdef _LIBOBJC
  44:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  45:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Thread local storage for a single thread */
  46:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static void *thread_local_storage = NULL;
  47:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  48:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Backend initialization functions */
  49:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  50:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Initialize the threads subsystem.  */
  51:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
  52:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_init_thread_system (void)
  53:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
  54:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   /* No thread support available */
  55:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return -1;
  56:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
  57:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  58:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Close the threads subsystem.  */
  59:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
  60:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_close_thread_system (void)
  61:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
  62:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   /* No thread support available */
  63:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return -1;
  64:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
  65:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  66:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Backend thread functions */
  67:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  68:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Create a new thread of execution.  */
  69:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline objc_thread_t
  70:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_thread_detach (void (* func)(void *), void * arg _GLIBCXX_UNUSED)
  71:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
  72:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   /* No thread support available */
  73:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return NULL;
  74:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
  75:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  76:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Set the current thread's priority.  */
  77:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
  78:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_thread_set_priority (int priority _GLIBCXX_UNUSED)
  79:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
  80:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   /* No thread support available */
  81:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return -1;
  82:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
  83:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
ARM GAS  /tmp/ccRVqFpy.s 			page 3


  84:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Return the current thread's priority.  */
  85:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
  86:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_thread_get_priority (void)
  87:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
  88:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return OBJC_THREAD_INTERACTIVE_PRIORITY;
  89:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
  90:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  91:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Yield our process time to another thread.  */
  92:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline void
  93:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_thread_yield (void)
  94:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
  95:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return;
  96:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
  97:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
  98:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Terminate the current thread.  */
  99:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 100:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_thread_exit (void)
 101:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 102:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   /* No thread support available */
 103:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   /* Should we really exit the program */
 104:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   /* exit (&__objc_thread_exit_status); */
 105:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return -1;
 106:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 107:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 108:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Returns an integer value which uniquely describes a thread.  */
 109:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline objc_thread_t
 110:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_thread_id (void)
 111:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 112:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   /* No thread support, use 1.  */
 113:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return (objc_thread_t) 1;
 114:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 115:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 116:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Sets the thread's local storage pointer.  */
 117:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 118:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_thread_set_data (void *value)
 119:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 120:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   thread_local_storage = value;
 121:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 122:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 123:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 124:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Returns the thread's local storage pointer.  */
 125:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline void *
 126:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_thread_get_data (void)
 127:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 128:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return thread_local_storage;
 129:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 130:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 131:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Backend mutex functions */
 132:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 133:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Allocate a mutex.  */
 134:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 135:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_mutex_allocate (objc_mutex_t mutex _GLIBCXX_UNUSED)
 136:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 137:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 138:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 139:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 140:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Deallocate a mutex.  */
ARM GAS  /tmp/ccRVqFpy.s 			page 4


 141:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 142:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_mutex_deallocate (objc_mutex_t mutex _GLIBCXX_UNUSED)
 143:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 144:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 145:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 146:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 147:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Grab a lock on a mutex.  */
 148:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 149:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_mutex_lock (objc_mutex_t mutex _GLIBCXX_UNUSED)
 150:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 151:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   /* There can only be one thread, so we always get the lock */
 152:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 153:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 154:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 155:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Try to grab a lock on a mutex.  */
 156:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 157:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_mutex_trylock (objc_mutex_t mutex _GLIBCXX_UNUSED)
 158:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 159:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   /* There can only be one thread, so we always get the lock */
 160:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 161:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 162:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 163:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Unlock the mutex */
 164:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 165:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_mutex_unlock (objc_mutex_t mutex _GLIBCXX_UNUSED)
 166:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 167:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 168:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 169:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 170:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Backend condition mutex functions */
 171:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 172:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Allocate a condition.  */
 173:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 174:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_condition_allocate (objc_condition_t condition _GLIBCXX_UNUSED)
 175:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 176:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 177:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 178:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 179:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Deallocate a condition.  */
 180:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 181:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_condition_deallocate (objc_condition_t condition _GLIBCXX_UNUSED)
 182:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 183:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 184:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 185:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 186:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Wait on the condition */
 187:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 188:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_condition_wait (objc_condition_t condition _GLIBCXX_UNUSED,
 189:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 			       objc_mutex_t mutex _GLIBCXX_UNUSED)
 190:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 191:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 192:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 193:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 194:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Wake up all threads waiting on this condition.  */
 195:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 196:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_condition_broadcast (objc_condition_t condition _GLIBCXX_UNUSED)
 197:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
ARM GAS  /tmp/ccRVqFpy.s 			page 5


 198:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 199:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 200:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 201:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** /* Wake up one thread waiting on this condition.  */
 202:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 203:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_objc_condition_signal (objc_condition_t condition _GLIBCXX_UNUSED)
 204:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 205:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 206:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 207:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 208:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** #else /* _LIBOBJC */
 209:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 210:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 211:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_active_p (void)
 212:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 213:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 214:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 215:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 216:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int
 217:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_once (__gthread_once_t *__once _GLIBCXX_UNUSED, void (*__func) (void) _GLIBCXX_UNUSED)
 218:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 219:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 220:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 221:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 222:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static inline int _GLIBCXX_UNUSED
 223:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_key_create (__gthread_key_t *__key _GLIBCXX_UNUSED, void (*__func) (void *) _GLIBCXX_UNUS
 224:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
 225:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
 226:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
 227:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** 
 228:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** static int _GLIBCXX_UNUSED
 229:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** __gthread_key_delete (__gthread_key_t __key _GLIBCXX_UNUSED)
 230:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** {
  35              		.loc 1 230 1
  36              		.cfi_startproc
  37              		@ args = 0, pretend = 0, frame = 8
  38              		@ frame_needed = 1, uses_anonymous_args = 0
  39              		@ link register save eliminated.
  40 0000 80B4     		push	{r7}
  41              	.LCFI0:
  42              		.cfi_def_cfa_offset 4
  43              		.cfi_offset 7, -4
  44 0002 83B0     		sub	sp, sp, #12
  45              	.LCFI1:
  46              		.cfi_def_cfa_offset 16
  47 0004 00AF     		add	r7, sp, #0
  48              	.LCFI2:
  49              		.cfi_def_cfa_register 7
  50 0006 7860     		str	r0, [r7, #4]
 231:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h ****   return 0;
  51              		.loc 1 231 10
  52 0008 0023     		movs	r3, #0
 232:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thumb/v7e-m+fp/hard/bits/gthr-default.h **** }
  53              		.loc 1 232 1
  54 000a 1846     		mov	r0, r3
  55 000c 0C37     		adds	r7, r7, #12
  56              	.LCFI3:
ARM GAS  /tmp/ccRVqFpy.s 			page 6


  57              		.cfi_def_cfa_offset 4
  58 000e BD46     		mov	sp, r7
  59              	.LCFI4:
  60              		.cfi_def_cfa_register 13
  61              		@ sp needed
  62 0010 5DF8047B 		ldr	r7, [sp], #4
  63              	.LCFI5:
  64              		.cfi_restore 7
  65              		.cfi_def_cfa_offset 0
  66 0014 7047     		bx	lr
  67              		.cfi_endproc
  68              	.LFE587:
  69              		.cantunwind
  70              		.fnend
  72              		.section	.rodata._ZStL13allocator_arg,"a"
  73              		.align	2
  76              	_ZStL13allocator_arg:
  77 0000 00       		.space	1
  78              		.section	.rodata._ZStL6ignore,"a"
  79              		.align	2
  82              	_ZStL6ignore:
  83 0000 00       		.space	1
  84              		.section	.text._ZNSt14_Function_baseC2Ev,"axG",%progbits,_ZNSt14_Function_baseC5Ev,comdat
  85              		.align	1
  86              		.weak	_ZNSt14_Function_baseC2Ev
  87              		.syntax unified
  88              		.thumb
  89              		.thumb_func
  90              		.fpu fpv4-sp-d16
  92              	_ZNSt14_Function_baseC2Ev:
  93              		.fnstart
  94              	.LFB1190:
  95              		.file 2 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function
   1:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // Implementation of std::function -*- C++ -*-
   2:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
   3:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // Copyright (C) 2004-2018 Free Software Foundation, Inc.
   4:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** //
   5:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // This file is part of the GNU ISO C++ Library.  This library is free
   6:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // software; you can redistribute it and/or modify it under the
   7:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // terms of the GNU General Public License as published by the
   8:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // Free Software Foundation; either version 3, or (at your option)
   9:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // any later version.
  10:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  11:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // This library is distributed in the hope that it will be useful,
  12:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // but WITHOUT ANY WARRANTY; without even the implied warranty of
  13:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  14:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // GNU General Public License for more details.
  15:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  16:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // Under Section 7 of GPL version 3, you are granted additional
  17:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // permissions described in the GCC Runtime Library Exception, version
  18:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // 3.1, as published by the Free Software Foundation.
  19:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  20:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // You should have received a copy of the GNU General Public License and
  21:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // a copy of the GCC Runtime Library Exception along with this program;
  22:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
  23:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** // <http://www.gnu.org/licenses/>.
  24:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
ARM GAS  /tmp/ccRVqFpy.s 			page 7


  25:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** /** @file include/bits/std_function.h
  26:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****  *  This is an internal header file, included by other library headers.
  27:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****  *  Do not attempt to use it directly. @headername{functional}
  28:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****  */
  29:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  30:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #ifndef _GLIBCXX_STD_FUNCTION_H
  31:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #define _GLIBCXX_STD_FUNCTION_H 1
  32:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  33:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #pragma GCC system_header
  34:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  35:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #if __cplusplus < 201103L
  36:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** # include <bits/c++0x_warning.h>
  37:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #else
  38:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  39:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #if __cpp_rtti
  40:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** # include <typeinfo>
  41:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #endif
  42:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #include <bits/stl_function.h>
  43:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #include <bits/invoke.h>
  44:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #include <bits/refwrap.h>
  45:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #include <bits/functexcept.h>
  46:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  47:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** namespace std _GLIBCXX_VISIBILITY(default)
  48:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** {
  49:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** _GLIBCXX_BEGIN_NAMESPACE_VERSION
  50:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  51:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   /**
  52:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *  @brief Exception class thrown when class template function's
  53:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *  operator() is called with an empty target.
  54:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *  @ingroup exceptions
  55:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    */
  56:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   class bad_function_call : public std::exception
  57:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   {
  58:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   public:
  59:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     virtual ~bad_function_call() noexcept;
  60:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  61:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     const char* what() const noexcept;
  62:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   };
  63:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  64:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   /**
  65:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *  Trait identifying "location-invariant" types, meaning that the
  66:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *  address of the object (or any of its members) will not escape.
  67:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *  Trivially copyable types are location-invariant and users can
  68:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *  specialize this trait for other types.
  69:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    */
  70:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _Tp>
  71:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     struct __is_location_invariant
  72:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     : is_trivially_copyable<_Tp>::type
  73:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     { };
  74:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  75:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   class _Undefined_class;
  76:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  77:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   union _Nocopy_types
  78:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   {
  79:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     void*       _M_object;
  80:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     const void* _M_const_object;
  81:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     void (*_M_function_pointer)();
ARM GAS  /tmp/ccRVqFpy.s 			page 8


  82:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     void (_Undefined_class::*_M_member_pointer)();
  83:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   };
  84:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  85:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   union [[gnu::may_alias]] _Any_data
  86:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   {
  87:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     void*       _M_access()       { return &_M_pod_data[0]; }
  88:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     const void* _M_access() const { return &_M_pod_data[0]; }
  89:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  90:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     template<typename _Tp>
  91:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       _Tp&
  92:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       _M_access()
  93:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       { return *static_cast<_Tp*>(_M_access()); }
  94:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
  95:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     template<typename _Tp>
  96:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       const _Tp&
  97:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       _M_access() const
  98:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       { return *static_cast<const _Tp*>(_M_access()); }
  99:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 100:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     _Nocopy_types _M_unused;
 101:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     char _M_pod_data[sizeof(_Nocopy_types)];
 102:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   };
 103:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 104:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   enum _Manager_operation
 105:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   {
 106:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     __get_type_info,
 107:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     __get_functor_ptr,
 108:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     __clone_functor,
 109:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     __destroy_functor
 110:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   };
 111:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 112:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   // Simple type wrapper that helps avoid annoying const problems
 113:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   // when casting between void pointers and pointers-to-pointers.
 114:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _Tp>
 115:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     struct _Simple_type_wrapper
 116:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     {
 117:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       _Simple_type_wrapper(_Tp __value) : __value(__value) { }
 118:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 119:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       _Tp __value;
 120:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     };
 121:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 122:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _Tp>
 123:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     struct __is_location_invariant<_Simple_type_wrapper<_Tp> >
 124:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     : __is_location_invariant<_Tp>
 125:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     { };
 126:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 127:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _Signature>
 128:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     class function;
 129:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 130:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   /// Base class of all polymorphic function object wrappers.
 131:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   class _Function_base
 132:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   {
 133:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   public:
 134:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     static const std::size_t _M_max_size = sizeof(_Nocopy_types);
 135:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     static const std::size_t _M_max_align = __alignof__(_Nocopy_types);
 136:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 137:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     template<typename _Functor>
 138:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       class _Base_manager
ARM GAS  /tmp/ccRVqFpy.s 			page 9


 139:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       {
 140:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       protected:
 141:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	static const bool __stored_locally =
 142:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	(__is_location_invariant<_Functor>::value
 143:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	 && sizeof(_Functor) <= _M_max_size
 144:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	 && __alignof__(_Functor) <= _M_max_align
 145:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	 && (_M_max_align % __alignof__(_Functor) == 0));
 146:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 147:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	typedef integral_constant<bool, __stored_locally> _Local_storage;
 148:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 149:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	// Retrieve a pointer to the function object
 150:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	static _Functor*
 151:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_M_get_pointer(const _Any_data& __source)
 152:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	{
 153:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  const _Functor* __ptr =
 154:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    __stored_locally? std::__addressof(__source._M_access<_Functor>())
 155:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    /* have stored a pointer */ : __source._M_access<_Functor*>();
 156:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  return const_cast<_Functor*>(__ptr);
 157:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	}
 158:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 159:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	// Clone a location-invariant function object that fits within
 160:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	// an _Any_data structure.
 161:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	static void
 162:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_M_clone(_Any_data& __dest, const _Any_data& __source, true_type)
 163:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	{
 164:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  ::new (__dest._M_access()) _Functor(__source._M_access<_Functor>());
 165:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	}
 166:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 167:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	// Clone a function object that is not location-invariant or
 168:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	// that cannot fit into an _Any_data structure.
 169:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	static void
 170:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_M_clone(_Any_data& __dest, const _Any_data& __source, false_type)
 171:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	{
 172:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  __dest._M_access<_Functor*>() =
 173:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    new _Functor(*__source._M_access<_Functor*>());
 174:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	}
 175:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 176:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	// Destroying a location-invariant object may still require
 177:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	// destruction.
 178:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	static void
 179:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_M_destroy(_Any_data& __victim, true_type)
 180:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	{
 181:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  __victim._M_access<_Functor>().~_Functor();
 182:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	}
 183:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 184:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	// Destroying an object located on the heap.
 185:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	static void
 186:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_M_destroy(_Any_data& __victim, false_type)
 187:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	{
 188:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  delete __victim._M_access<_Functor*>();
 189:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	}
 190:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 191:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       public:
 192:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	static bool
 193:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_M_manager(_Any_data& __dest, const _Any_data& __source,
 194:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 		   _Manager_operation __op)
 195:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	{
ARM GAS  /tmp/ccRVqFpy.s 			page 10


 196:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  switch (__op)
 197:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    {
 198:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #if __cpp_rtti
 199:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    case __get_type_info:
 200:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	      __dest._M_access<const type_info*>() = &typeid(_Functor);
 201:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	      break;
 202:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #endif
 203:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    case __get_functor_ptr:
 204:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	      __dest._M_access<_Functor*>() = _M_get_pointer(__source);
 205:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	      break;
 206:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 207:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    case __clone_functor:
 208:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	      _M_clone(__dest, __source, _Local_storage());
 209:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	      break;
 210:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 211:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    case __destroy_functor:
 212:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	      _M_destroy(__dest, _Local_storage());
 213:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	      break;
 214:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    }
 215:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  return false;
 216:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	}
 217:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 218:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	static void
 219:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_M_init_functor(_Any_data& __functor, _Functor&& __f)
 220:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	{ _M_init_functor(__functor, std::move(__f), _Local_storage()); }
 221:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 222:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	template<typename _Signature>
 223:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  static bool
 224:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  _M_not_empty_function(const function<_Signature>& __f)
 225:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  { return static_cast<bool>(__f); }
 226:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 227:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	template<typename _Tp>
 228:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  static bool
 229:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  _M_not_empty_function(_Tp* __fp)
 230:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  { return __fp != nullptr; }
 231:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 232:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	template<typename _Class, typename _Tp>
 233:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  static bool
 234:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  _M_not_empty_function(_Tp _Class::* __mp)
 235:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  { return __mp != nullptr; }
 236:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 237:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	template<typename _Tp>
 238:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  static bool
 239:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  _M_not_empty_function(const _Tp&)
 240:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  { return true; }
 241:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 242:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       private:
 243:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	static void
 244:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_M_init_functor(_Any_data& __functor, _Functor&& __f, true_type)
 245:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	{ ::new (__functor._M_access()) _Functor(std::move(__f)); }
 246:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 247:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	static void
 248:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_M_init_functor(_Any_data& __functor, _Functor&& __f, false_type)
 249:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	{ __functor._M_access<_Functor*>() = new _Functor(std::move(__f)); }
 250:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       };
 251:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 252:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     _Function_base() : _M_manager(nullptr) { }
ARM GAS  /tmp/ccRVqFpy.s 			page 11


  96              		.loc 2 252 5
  97              		.cfi_startproc
  98              		@ args = 0, pretend = 0, frame = 8
  99              		@ frame_needed = 1, uses_anonymous_args = 0
 100              		@ link register save eliminated.
 101 0000 80B4     		push	{r7}
 102              	.LCFI6:
 103              		.cfi_def_cfa_offset 4
 104              		.cfi_offset 7, -4
 105 0002 83B0     		sub	sp, sp, #12
 106              	.LCFI7:
 107              		.cfi_def_cfa_offset 16
 108 0004 00AF     		add	r7, sp, #0
 109              	.LCFI8:
 110              		.cfi_def_cfa_register 7
 111 0006 7860     		str	r0, [r7, #4]
 112              	.LBB2:
 113              		.loc 2 252 42
 114 0008 7B68     		ldr	r3, [r7, #4]
 115 000a 0022     		movs	r2, #0
 116 000c 9A60     		str	r2, [r3, #8]
 117              	.LBE2:
 118              		.loc 2 252 46
 119 000e 7B68     		ldr	r3, [r7, #4]
 120 0010 1846     		mov	r0, r3
 121 0012 0C37     		adds	r7, r7, #12
 122              	.LCFI9:
 123              		.cfi_def_cfa_offset 4
 124 0014 BD46     		mov	sp, r7
 125              	.LCFI10:
 126              		.cfi_def_cfa_register 13
 127              		@ sp needed
 128 0016 5DF8047B 		ldr	r7, [sp], #4
 129              	.LCFI11:
 130              		.cfi_restore 7
 131              		.cfi_def_cfa_offset 0
 132 001a 7047     		bx	lr
 133              		.cfi_endproc
 134              	.LFE1190:
 135              		.cantunwind
 136              		.fnend
 138              		.weak	_ZNSt14_Function_baseC1Ev
 139              		.thumb_set _ZNSt14_Function_baseC1Ev,_ZNSt14_Function_baseC2Ev
 140              		.section	.text._ZNSt14_Function_baseD2Ev,"axG",%progbits,_ZNSt14_Function_baseD5Ev,comdat
 141              		.align	1
 142              		.weak	_ZNSt14_Function_baseD2Ev
 143              		.syntax unified
 144              		.thumb
 145              		.thumb_func
 146              		.fpu fpv4-sp-d16
 148              	_ZNSt14_Function_baseD2Ev:
 149              		.fnstart
 150              	.LFB1193:
 253:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 254:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     ~_Function_base()
 151              		.loc 2 254 5
 152              		.cfi_startproc
ARM GAS  /tmp/ccRVqFpy.s 			page 12


 153              		@ args = 0, pretend = 0, frame = 8
 154              		@ frame_needed = 1, uses_anonymous_args = 0
 155 0000 80B5     		push	{r7, lr}
 156              		.save {r7, lr}
 157              	.LCFI12:
 158              		.cfi_def_cfa_offset 8
 159              		.cfi_offset 7, -8
 160              		.cfi_offset 14, -4
 161              		.pad #8
 162 0002 82B0     		sub	sp, sp, #8
 163              	.LCFI13:
 164              		.cfi_def_cfa_offset 16
 165              		.setfp r7, sp, #0
 166 0004 00AF     		add	r7, sp, #0
 167              	.LCFI14:
 168              		.cfi_def_cfa_register 7
 169 0006 7860     		str	r0, [r7, #4]
 170              	.LBB3:
 255:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     {
 256:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       if (_M_manager)
 171              		.loc 2 256 11
 172 0008 7B68     		ldr	r3, [r7, #4]
 173 000a 9B68     		ldr	r3, [r3, #8]
 174              		.loc 2 256 7
 175 000c 002B     		cmp	r3, #0
 176 000e 05D0     		beq	.L6
 257:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_M_manager(_M_functor, _M_functor, __destroy_functor);
 177              		.loc 2 257 2
 178 0010 7B68     		ldr	r3, [r7, #4]
 179 0012 9B68     		ldr	r3, [r3, #8]
 180              		.loc 2 257 13
 181 0014 7868     		ldr	r0, [r7, #4]
 182              		.loc 2 257 25
 183 0016 7968     		ldr	r1, [r7, #4]
 184              		.loc 2 257 12
 185 0018 0322     		movs	r2, #3
 186 001a 9847     		blx	r3
 187              	.LVL0:
 188              	.L6:
 189              	.LBE3:
 258:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     }
 190              		.loc 2 258 5
 191 001c 7B68     		ldr	r3, [r7, #4]
 192 001e 1846     		mov	r0, r3
 193 0020 0837     		adds	r7, r7, #8
 194              	.LCFI15:
 195              		.cfi_def_cfa_offset 8
 196 0022 BD46     		mov	sp, r7
 197              	.LCFI16:
 198              		.cfi_def_cfa_register 13
 199              		@ sp needed
 200 0024 80BD     		pop	{r7, pc}
 201              		.cfi_endproc
 202              	.LFE1193:
 203              		.global	__gxx_personality_v0
 204              		.personality	__gxx_personality_v0
 205              		.handlerdata
ARM GAS  /tmp/ccRVqFpy.s 			page 13


 206              	.LLSDA1193:
 207 000c FF       		.byte	0xff
 208 000d FF       		.byte	0xff
 209 000e 01       		.byte	0x1
 210 000f 00       		.uleb128 .LLSDACSE1193-.LLSDACSB1193
 211              	.LLSDACSB1193:
 212              	.LLSDACSE1193:
 213              		.section	.text._ZNSt14_Function_baseD2Ev,"axG",%progbits,_ZNSt14_Function_baseD5Ev,comdat
 214              		.fnend
 216              		.weak	_ZNSt14_Function_baseD1Ev
 217              		.thumb_set _ZNSt14_Function_baseD1Ev,_ZNSt14_Function_baseD2Ev
 218              		.global	usb
 219              		.section	.bss.usb,"aw",%nobits
 220              		.align	2
 223              	usb:
 224 0000 00       		.space	1
 225              		.global	f
 226              		.section	.bss.f,"aw",%nobits
 227              		.align	2
 230              	f:
 231 0000 00000000 		.space	16
 231      00000000 
 231      00000000 
 231      00000000 
 232              		.section	.text.init,"ax",%progbits
 233              		.align	1
 234              		.global	init
 235              		.syntax unified
 236              		.thumb
 237              		.thumb_func
 238              		.fpu fpv4-sp-d16
 240              	init:
 241              		.fnstart
 242              	.LFB1262:
 243              		.file 3 "Src/main2.cpp"
   1:Src/main2.cpp **** #include "usb.h"
   2:Src/main2.cpp **** #include <functional>
   3:Src/main2.cpp **** #include "main2.h"
   4:Src/main2.cpp **** 
   5:Src/main2.cpp **** USBD usb;
   6:Src/main2.cpp **** std::function<void ()> f;
   7:Src/main2.cpp **** 
   8:Src/main2.cpp **** void init() {
 244              		.loc 3 8 13
 245              		.cfi_startproc
 246              		@ args = 0, pretend = 0, frame = 0
 247              		@ frame_needed = 1, uses_anonymous_args = 0
 248 0000 80B5     		push	{r7, lr}
 249              		.save {r7, lr}
 250              	.LCFI17:
 251              		.cfi_def_cfa_offset 8
 252              		.cfi_offset 7, -8
 253              		.cfi_offset 14, -4
 254              		.setfp r7, sp, #0
 255 0002 00AF     		add	r7, sp, #0
 256              	.LCFI18:
 257              		.cfi_def_cfa_register 7
ARM GAS  /tmp/ccRVqFpy.s 			page 14


   9:Src/main2.cpp ****     usb.init();
 258              		.loc 3 9 13
 259 0004 0248     		ldr	r0, .L9
 260 0006 FFF7FEFF 		bl	_ZN4USBD4initEv
  10:Src/main2.cpp ****  //   f = std::bind(&USBD::usb_interrupt, &usb);
  11:Src/main2.cpp **** }
 261              		.loc 3 11 1
 262 000a 00BF     		nop
 263 000c 80BD     		pop	{r7, pc}
 264              	.L10:
 265 000e 00BF     		.align	2
 266              	.L9:
 267 0010 00000000 		.word	usb
 268              		.cfi_endproc
 269              	.LFE1262:
 270              		.fnend
 272              		.section	.text.usb_interrupt,"ax",%progbits
 273              		.align	1
 274              		.global	usb_interrupt
 275              		.syntax unified
 276              		.thumb
 277              		.thumb_func
 278              		.fpu fpv4-sp-d16
 280              	usb_interrupt:
 281              		.fnstart
 282              	.LFB1263:
  12:Src/main2.cpp **** 
  13:Src/main2.cpp **** void usb_interrupt() {
 283              		.loc 3 13 22
 284              		.cfi_startproc
 285              		@ args = 0, pretend = 0, frame = 0
 286              		@ frame_needed = 1, uses_anonymous_args = 0
 287              		@ link register save eliminated.
 288 0000 80B4     		push	{r7}
 289              	.LCFI19:
 290              		.cfi_def_cfa_offset 4
 291              		.cfi_offset 7, -4
 292 0002 00AF     		add	r7, sp, #0
 293              	.LCFI20:
 294              		.cfi_def_cfa_register 7
  14:Src/main2.cpp ****  //   f();
  15:Src/main2.cpp **** }...
 295              		.loc 3 15 1
 296 0004 00BF     		nop
 297 0006 BD46     		mov	sp, r7
 298              	.LCFI21:
 299              		.cfi_def_cfa_register 13
 300              		@ sp needed
 301 0008 5DF8047B 		ldr	r7, [sp], #4
 302              	.LCFI22:
 303              		.cfi_restore 7
 304              		.cfi_def_cfa_offset 0
 305 000c 7047     		bx	lr
 306              		.cfi_endproc
 307              	.LFE1263:
 308              		.cantunwind
 309              		.fnend
ARM GAS  /tmp/ccRVqFpy.s 			page 15


 311              		.section	.text._ZNSt8functionIFvvEEC2Ev,"axG",%progbits,_ZNSt8functionIFvvEEC5Ev,comdat
 312              		.align	1
 313              		.weak	_ZNSt8functionIFvvEEC2Ev
 314              		.syntax unified
 315              		.thumb
 316              		.thumb_func
 317              		.fpu fpv4-sp-d16
 319              	_ZNSt8functionIFvvEEC2Ev:
 320              		.fnstart
 321              	.LFB1441:
 259:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 260:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     bool _M_empty() const { return !_M_manager; }
 261:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 262:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     typedef bool (*_Manager_type)(_Any_data&, const _Any_data&,
 263:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 				  _Manager_operation);
 264:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 265:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     _Any_data     _M_functor;
 266:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     _Manager_type _M_manager;
 267:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   };
 268:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 269:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _Signature, typename _Functor>
 270:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     class _Function_handler;
 271:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 272:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _Res, typename _Functor, typename... _ArgTypes>
 273:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     class _Function_handler<_Res(_ArgTypes...), _Functor>
 274:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     : public _Function_base::_Base_manager<_Functor>
 275:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     {
 276:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       typedef _Function_base::_Base_manager<_Functor> _Base;
 277:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 278:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     public:
 279:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       static _Res
 280:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       _M_invoke(const _Any_data& __functor, _ArgTypes&&... __args)
 281:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       {
 282:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	return (*_Base::_M_get_pointer(__functor))(
 283:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    std::forward<_ArgTypes>(__args)...);
 284:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       }
 285:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     };
 286:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 287:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _Functor, typename... _ArgTypes>
 288:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     class _Function_handler<void(_ArgTypes...), _Functor>
 289:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     : public _Function_base::_Base_manager<_Functor>
 290:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     {
 291:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       typedef _Function_base::_Base_manager<_Functor> _Base;
 292:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 293:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****      public:
 294:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       static void
 295:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       _M_invoke(const _Any_data& __functor, _ArgTypes&&... __args)
 296:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       {
 297:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	(*_Base::_M_get_pointer(__functor))(
 298:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    std::forward<_ArgTypes>(__args)...);
 299:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       }
 300:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     };
 301:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 302:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _Class, typename _Member, typename _Res,
 303:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	   typename... _ArgTypes>
 304:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     class _Function_handler<_Res(_ArgTypes...), _Member _Class::*>
 305:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     : public _Function_handler<void(_ArgTypes...), _Member _Class::*>
ARM GAS  /tmp/ccRVqFpy.s 			page 16


 306:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     {
 307:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       typedef _Function_handler<void(_ArgTypes...), _Member _Class::*>
 308:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	_Base;
 309:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 310:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****      public:
 311:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       static _Res
 312:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       _M_invoke(const _Any_data& __functor, _ArgTypes&&... __args)
 313:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       {
 314:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	return std::__invoke(_Base::_M_get_pointer(__functor)->__value,
 315:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 			     std::forward<_ArgTypes>(__args)...);
 316:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       }
 317:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     };
 318:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 319:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _Class, typename _Member, typename... _ArgTypes>
 320:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     class _Function_handler<void(_ArgTypes...), _Member _Class::*>
 321:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     : public _Function_base::_Base_manager<
 322:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 		 _Simple_type_wrapper< _Member _Class::* > >
 323:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     {
 324:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       typedef _Member _Class::* _Functor;
 325:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       typedef _Simple_type_wrapper<_Functor> _Wrapper;
 326:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       typedef _Function_base::_Base_manager<_Wrapper> _Base;
 327:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 328:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     public:
 329:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       static bool
 330:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       _M_manager(_Any_data& __dest, const _Any_data& __source,
 331:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 		 _Manager_operation __op)
 332:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       {
 333:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	switch (__op)
 334:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  {
 335:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #if __cpp_rtti
 336:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  case __get_type_info:
 337:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    __dest._M_access<const type_info*>() = &typeid(_Functor);
 338:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    break;
 339:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** #endif
 340:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  case __get_functor_ptr:
 341:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    __dest._M_access<_Functor*>() =
 342:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	      &_Base::_M_get_pointer(__source)->__value;
 343:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    break;
 344:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 345:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  default:
 346:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	    _Base::_M_manager(__dest, __source, __op);
 347:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	  }
 348:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	return false;
 349:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       }
 350:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 351:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       static void
 352:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       _M_invoke(const _Any_data& __functor, _ArgTypes&&... __args)
 353:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       {
 354:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	std::__invoke(_Base::_M_get_pointer(__functor)->__value,
 355:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 		      std::forward<_ArgTypes>(__args)...);
 356:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       }
 357:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     };
 358:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 359:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _From, typename _To>
 360:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     using __check_func_return_type
 361:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       = __or_<is_void<_To>, is_same<_From, _To>, is_convertible<_From, _To>>;
 362:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
ARM GAS  /tmp/ccRVqFpy.s 			page 17


 363:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   /**
 364:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *  @brief Primary class template for std::function.
 365:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *  @ingroup functors
 366:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *
 367:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    *  Polymorphic function wrapper.
 368:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****    */
 369:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****   template<typename _Res, typename... _ArgTypes>
 370:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     class function<_Res(_ArgTypes...)>
 371:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     : public _Maybe_unary_or_binary_function<_Res, _ArgTypes...>,
 372:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       private _Function_base
 373:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     {
 374:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       template<typename _Func,
 375:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	       typename _Res2 = typename result_of<_Func&(_ArgTypes...)>::type>
 376:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	struct _Callable : __check_func_return_type<_Res2, _Res> { };
 377:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 378:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       // Used so the return type convertibility checks aren't done when
 379:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       // performing overload resolution for copy construction/assignment.
 380:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       template<typename _Tp>
 381:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	struct _Callable<function, _Tp> : false_type { };
 382:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 383:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       template<typename _Cond, typename _Tp>
 384:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 	using _Requires = typename enable_if<_Cond::value, _Tp>::type;
 385:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 386:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     public:
 387:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       typedef _Res result_type;
 388:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 389:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       // [3.7.2.1] construct/copy/destroy
 390:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h **** 
 391:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       /**
 392:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****        *  @brief Default construct creates an empty function call wrapper.
 393:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****        *  @post @c !(bool)*this
 394:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****        */
 395:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       function() noexcept
 322              		.loc 2 395 7
 323              		.cfi_startproc
 324              		@ args = 0, pretend = 0, frame = 8
 325              		@ frame_needed = 1, uses_anonymous_args = 0
 326 0000 80B5     		push	{r7, lr}
 327              	.LCFI23:
 328              		.cfi_def_cfa_offset 8
 329              		.cfi_offset 7, -8
 330              		.cfi_offset 14, -4
 331 0002 82B0     		sub	sp, sp, #8
 332              	.LCFI24:
 333              		.cfi_def_cfa_offset 16
 334 0004 00AF     		add	r7, sp, #0
 335              	.LCFI25:
 336              		.cfi_def_cfa_register 7
 337 0006 7860     		str	r0, [r7, #4]
 338              	.LBB4:
 396:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****       : _Function_base() { }
 339              		.loc 2 396 24
 340 0008 7B68     		ldr	r3, [r7, #4]
 341 000a 1846     		mov	r0, r3
 342 000c FFF7FEFF 		bl	_ZNSt14_Function_baseC2Ev
 343              	.LBE4:
 344              		.loc 2 396 28
ARM GAS  /tmp/ccRVqFpy.s 			page 18


 345 0010 7B68     		ldr	r3, [r7, #4]
 346 0012 1846     		mov	r0, r3
 347 0014 0837     		adds	r7, r7, #8
 348              	.LCFI26:
 349              		.cfi_def_cfa_offset 8
 350 0016 BD46     		mov	sp, r7
 351              	.LCFI27:
 352              		.cfi_def_cfa_register 13
 353              		@ sp needed
 354 0018 80BD     		pop	{r7, pc}
 355              		.cfi_endproc
 356              	.LFE1441:
 357              		.cantunwind
 358              		.fnend
 360              		.weak	_ZNSt8functionIFvvEEC1Ev
 361              		.thumb_set _ZNSt8functionIFvvEEC1Ev,_ZNSt8functionIFvvEEC2Ev
 362              		.section	.text._ZNSt8functionIFvvEED2Ev,"axG",%progbits,_ZNSt8functionIFvvEED5Ev,comdat
 363              		.align	1
 364              		.weak	_ZNSt8functionIFvvEED2Ev
 365              		.syntax unified
 366              		.thumb
 367              		.thumb_func
 368              		.fpu fpv4-sp-d16
 370              	_ZNSt8functionIFvvEED2Ev:
 371              		.fnstart
 372              	.LFB1559:
 370:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     : public _Maybe_unary_or_binary_function<_Res, _ArgTypes...>,
 373              		.loc 2 370 11
 374              		.cfi_startproc
 375              		@ args = 0, pretend = 0, frame = 8
 376              		@ frame_needed = 1, uses_anonymous_args = 0
 377 0000 80B5     		push	{r7, lr}
 378              	.LCFI28:
 379              		.cfi_def_cfa_offset 8
 380              		.cfi_offset 7, -8
 381              		.cfi_offset 14, -4
 382 0002 82B0     		sub	sp, sp, #8
 383              	.LCFI29:
 384              		.cfi_def_cfa_offset 16
 385 0004 00AF     		add	r7, sp, #0
 386              	.LCFI30:
 387              		.cfi_def_cfa_register 7
 388 0006 7860     		str	r0, [r7, #4]
 389              	.LBB5:
 370:/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/std_function.h ****     : public _Maybe_unary_or_binary_function<_Res, _ArgTypes...>,
 390              		.loc 2 370 11
 391 0008 7B68     		ldr	r3, [r7, #4]
 392 000a 1846     		mov	r0, r3
 393 000c FFF7FEFF 		bl	_ZNSt14_Function_baseD2Ev
 394              	.LBE5:
 395 0010 7B68     		ldr	r3, [r7, #4]
 396 0012 1846     		mov	r0, r3
 397 0014 0837     		adds	r7, r7, #8
 398              	.LCFI31:
 399              		.cfi_def_cfa_offset 8
 400 0016 BD46     		mov	sp, r7
 401              	.LCFI32:
ARM GAS  /tmp/ccRVqFpy.s 			page 19


 402              		.cfi_def_cfa_register 13
 403              		@ sp needed
 404 0018 80BD     		pop	{r7, pc}
 405              		.cfi_endproc
 406              	.LFE1559:
 407              		.cantunwind
 408              		.fnend
 410              		.weak	_ZNSt8functionIFvvEED1Ev
 411              		.thumb_set _ZNSt8functionIFvvEED1Ev,_ZNSt8functionIFvvEED2Ev
 412              		.section	.text._Z41__static_initialization_and_destruction_0ii,"ax",%progbits
 413              		.align	1
 414              		.syntax unified
 415              		.thumb
 416              		.thumb_func
 417              		.fpu fpv4-sp-d16
 419              	_Z41__static_initialization_and_destruction_0ii:
 420              		.fnstart
 421              	.LFB1557:
 422              		.loc 3 15 1
 423              		.cfi_startproc
 424              		@ args = 0, pretend = 0, frame = 8
 425              		@ frame_needed = 1, uses_anonymous_args = 0
 426 0000 80B5     		push	{r7, lr}
 427              	.LCFI33:
 428              		.cfi_def_cfa_offset 8
 429              		.cfi_offset 7, -8
 430              		.cfi_offset 14, -4
 431 0002 82B0     		sub	sp, sp, #8
 432              	.LCFI34:
 433              		.cfi_def_cfa_offset 16
 434 0004 00AF     		add	r7, sp, #0
 435              	.LCFI35:
 436              		.cfi_def_cfa_register 7
 437 0006 7860     		str	r0, [r7, #4]
 438 0008 3960     		str	r1, [r7]
 439              		.loc 3 15 1
 440 000a 7B68     		ldr	r3, [r7, #4]
 441 000c 012B     		cmp	r3, #1
 442 000e 0CD1     		bne	.L18
 443              		.loc 3 15 1 is_stmt 0 discriminator 1
 444 0010 3B68     		ldr	r3, [r7]
 445 0012 4FF6FF72 		movw	r2, #65535
 446 0016 9342     		cmp	r3, r2
 447 0018 07D1     		bne	.L18
   6:Src/main2.cpp **** std::function<void ()> f;
 448              		.loc 3 6 24 is_stmt 1
 449 001a 0648     		ldr	r0, .L19
 450 001c FFF7FEFF 		bl	_ZNSt8functionIFvvEEC1Ev
 451 0020 054A     		ldr	r2, .L19+4
 452 0022 0649     		ldr	r1, .L19+8
 453 0024 0348     		ldr	r0, .L19
 454 0026 FFF7FEFF 		bl	__aeabi_atexit
 455              	.L18:
 456              		.loc 3 15 1
 457 002a 00BF     		nop
 458 002c 0837     		adds	r7, r7, #8
 459              	.LCFI36:
ARM GAS  /tmp/ccRVqFpy.s 			page 20


 460              		.cfi_def_cfa_offset 8
 461 002e BD46     		mov	sp, r7
 462              	.LCFI37:
 463              		.cfi_def_cfa_register 13
 464              		@ sp needed
 465 0030 80BD     		pop	{r7, pc}
 466              	.L20:
 467 0032 00BF     		.align	2
 468              	.L19:
 469 0034 00000000 		.word	f
 470 0038 00000000 		.word	__dso_handle
 471 003c 00000000 		.word	_ZNSt8functionIFvvEED1Ev
 472              		.cfi_endproc
 473              	.LFE1557:
 474              		.cantunwind
 475              		.fnend
 477              		.section	.text._GLOBAL__sub_I_usb,"ax",%progbits
 478              		.align	1
 479              		.syntax unified
 480              		.thumb
 481              		.thumb_func
 482              		.fpu fpv4-sp-d16
 484              	_GLOBAL__sub_I_usb:
 485              		.fnstart
 486              	.LFB1561:
 487              		.loc 3 15 1
 488              		.cfi_startproc
 489              		@ args = 0, pretend = 0, frame = 0
 490              		@ frame_needed = 1, uses_anonymous_args = 0
 491 0000 80B5     		push	{r7, lr}
 492              	.LCFI38:
 493              		.cfi_def_cfa_offset 8
 494              		.cfi_offset 7, -8
 495              		.cfi_offset 14, -4
 496 0002 00AF     		add	r7, sp, #0
 497              	.LCFI39:
 498              		.cfi_def_cfa_register 7
 499              		.loc 3 15 1
 500 0004 4FF6FF71 		movw	r1, #65535
 501 0008 0120     		movs	r0, #1
 502 000a FFF7FEFF 		bl	_Z41__static_initialization_and_destruction_0ii
 503 000e 80BD     		pop	{r7, pc}
 504              		.cfi_endproc
 505              	.LFE1561:
 506              		.cantunwind
 507              		.fnend
 509              		.section	.init_array,"aw",%init_array
 510              		.align	2
 511 0000 00000000 		.word	_GLOBAL__sub_I_usb(target1)
 512              		.text
 513              	.Letext0:
 514              		.file 4 "Src/usb.h"
 515              		.file 5 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/exception_pt
 516              		.file 6 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/arm-none-eabi/thu
 517              		.file 7 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/type_traits"
 518              		.file 8 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/new"
 519              		.file 9 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/stl_pair.h"
ARM GAS  /tmp/ccRVqFpy.s 			page 21


 520              		.file 10 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/debug/debug.h"
 521              		.file 11 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cwchar"
 522              		.file 12 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdint"
 523              		.file 13 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/clocale"
 524              		.file 14 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdlib"
 525              		.file 15 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/cstdio"
 526              		.file 16 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/uses_alloca
 527              		.file 17 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/tuple"
 528              		.file 18 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/functional"
 529              		.file 19 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/refwrap.h"
 530              		.file 20 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/bits/predefined_
 531              		.file 21 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/ext/new_allocato
 532              		.file 22 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/c++/8.3.1/ext/numeric_trai
 533              		.file 23 "/opt/gcc-arm-none-eabi-8-2019-q3-update/lib/gcc/arm-none-eabi/8.3.1/include/stddef.h"
 534              		.file 24 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/machine/_default_types.h"
 535              		.file 25 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_types.h"
 536              		.file 26 "<built-in>"
 537              		.file 27 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/reent.h"
 538              		.file 28 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/lock.h"
 539              		.file 29 "/opt/gcc-arm-none-eabi-8-2019-q3-update/lib/gcc/arm-none-eabi/8.3.1/include/stdarg.h"
 540              		.file 30 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/wchar.h"
 541              		.file 31 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/_stdint.h"
 542              		.file 32 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdint.h"
 543              		.file 33 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/locale.h"
 544              		.file 34 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/ctype.h"
 545              		.file 35 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdlib.h"
 546              		.file 36 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/stdio.h"
 547              		.file 37 "/opt/gcc-arm-none-eabi-8-2019-q3-update/arm-none-eabi/include/sys/errno.h"
ARM GAS  /tmp/ccRVqFpy.s 			page 22


DEFINED SYMBOLS
                            *ABS*:0000000000000000 main2.cpp
     /tmp/ccRVqFpy.s:18     .rodata._ZStL19piecewise_construct:0000000000000000 $d
     /tmp/ccRVqFpy.s:21     .rodata._ZStL19piecewise_construct:0000000000000000 _ZStL19piecewise_construct
     /tmp/ccRVqFpy.s:24     .text._ZL20__gthread_key_deletei:0000000000000000 $t
     /tmp/ccRVqFpy.s:31     .text._ZL20__gthread_key_deletei:0000000000000000 _ZL20__gthread_key_deletei
.ARM.exidx.text._ZL20__gthread_key_deletei:0000000000000000 $d
     /tmp/ccRVqFpy.s:73     .rodata._ZStL13allocator_arg:0000000000000000 $d
     /tmp/ccRVqFpy.s:76     .rodata._ZStL13allocator_arg:0000000000000000 _ZStL13allocator_arg
     /tmp/ccRVqFpy.s:79     .rodata._ZStL6ignore:0000000000000000 $d
     /tmp/ccRVqFpy.s:82     .rodata._ZStL6ignore:0000000000000000 _ZStL6ignore
     /tmp/ccRVqFpy.s:85     .text._ZNSt14_Function_baseC2Ev:0000000000000000 $t
     /tmp/ccRVqFpy.s:92     .text._ZNSt14_Function_baseC2Ev:0000000000000000 _ZNSt14_Function_baseC2Ev
.ARM.exidx.text._ZNSt14_Function_baseC2Ev:0000000000000000 $d
     /tmp/ccRVqFpy.s:92     .text._ZNSt14_Function_baseC2Ev:0000000000000000 _ZNSt14_Function_baseC1Ev
     /tmp/ccRVqFpy.s:141    .text._ZNSt14_Function_baseD2Ev:0000000000000000 $t
     /tmp/ccRVqFpy.s:148    .text._ZNSt14_Function_baseD2Ev:0000000000000000 _ZNSt14_Function_baseD2Ev
.ARM.extab.text._ZNSt14_Function_baseD2Ev:0000000000000000 $d
.ARM.exidx.text._ZNSt14_Function_baseD2Ev:0000000000000000 $d
     /tmp/ccRVqFpy.s:148    .text._ZNSt14_Function_baseD2Ev:0000000000000000 _ZNSt14_Function_baseD1Ev
     /tmp/ccRVqFpy.s:223    .bss.usb:0000000000000000 usb
     /tmp/ccRVqFpy.s:220    .bss.usb:0000000000000000 $d
     /tmp/ccRVqFpy.s:230    .bss.f:0000000000000000 f
     /tmp/ccRVqFpy.s:227    .bss.f:0000000000000000 $d
     /tmp/ccRVqFpy.s:233    .text.init:0000000000000000 $t
     /tmp/ccRVqFpy.s:240    .text.init:0000000000000000 init
     /tmp/ccRVqFpy.s:267    .text.init:0000000000000010 $d
             .ARM.exidx.text.init:0000000000000000 $d
     /tmp/ccRVqFpy.s:273    .text.usb_interrupt:0000000000000000 $t
     /tmp/ccRVqFpy.s:280    .text.usb_interrupt:0000000000000000 usb_interrupt
    .ARM.exidx.text.usb_interrupt:0000000000000000 $d
     /tmp/ccRVqFpy.s:312    .text._ZNSt8functionIFvvEEC2Ev:0000000000000000 $t
     /tmp/ccRVqFpy.s:319    .text._ZNSt8functionIFvvEEC2Ev:0000000000000000 _ZNSt8functionIFvvEEC2Ev
.ARM.exidx.text._ZNSt8functionIFvvEEC2Ev:0000000000000000 $d
     /tmp/ccRVqFpy.s:319    .text._ZNSt8functionIFvvEEC2Ev:0000000000000000 _ZNSt8functionIFvvEEC1Ev
     /tmp/ccRVqFpy.s:363    .text._ZNSt8functionIFvvEED2Ev:0000000000000000 $t
     /tmp/ccRVqFpy.s:370    .text._ZNSt8functionIFvvEED2Ev:0000000000000000 _ZNSt8functionIFvvEED2Ev
.ARM.exidx.text._ZNSt8functionIFvvEED2Ev:0000000000000000 $d
     /tmp/ccRVqFpy.s:370    .text._ZNSt8functionIFvvEED2Ev:0000000000000000 _ZNSt8functionIFvvEED1Ev
     /tmp/ccRVqFpy.s:413    .text._Z41__static_initialization_and_destruction_0ii:0000000000000000 $t
     /tmp/ccRVqFpy.s:419    .text._Z41__static_initialization_and_destruction_0ii:0000000000000000 _Z41__static_initialization_and_destruction_0ii
     /tmp/ccRVqFpy.s:469    .text._Z41__static_initialization_and_destruction_0ii:0000000000000034 $d
.ARM.exidx.text._Z41__static_initialization_and_destruction_0ii:0000000000000000 $d
     /tmp/ccRVqFpy.s:478    .text._GLOBAL__sub_I_usb:0000000000000000 $t
     /tmp/ccRVqFpy.s:484    .text._GLOBAL__sub_I_usb:0000000000000000 _GLOBAL__sub_I_usb
.ARM.exidx.text._GLOBAL__sub_I_usb:0000000000000000 $d
     /tmp/ccRVqFpy.s:510    .init_array:0000000000000000 $d
                           .group:0000000000000000 _ZNSt14_Function_baseC5Ev
                           .group:0000000000000000 _ZNSt14_Function_baseD5Ev
                           .group:0000000000000000 _ZNSt8functionIFvvEEC5Ev
                           .group:0000000000000000 _ZNSt8functionIFvvEED5Ev

UNDEFINED SYMBOLS
__gxx_personality_v0
_ZN4USBD4initEv
__aeabi_unwind_cpp_pr0
__aeabi_atexit
ARM GAS  /tmp/ccRVqFpy.s 			page 23


__dso_handle
