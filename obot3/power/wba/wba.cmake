# The radio firmware of the power board and of the e-stop remote (STM32WBA5MMG), on ST's
# STM32CubeWBA package: see ../README.md. Included from obot3/CMakeLists.txt when STM32CUBE_WBA
# points at the package (fetch_stm32cubewba.sh gets it).
#
# Nothing of ST's is copied into this repository. Their example BLE_BeaconHCI (the Bluetooth LE
# link layer used through HCI commands, no host stack) is compiled from the package as it is,
# all but the four files that are its beacons; ours take their place (port.c, the applications).
# Three of its headers are extended, not replaced: overlay/ is first on the include path and
# each file there includes ST's with #include_next. The example is for ST's Nucleo board with
# the bare STM32WBA55; the module's start-up file, linker script and device name are those of
# ST's own project for its module board (B-WBA5M-WPAN), and the link layer library is the one
# that can also listen (ST's beacon only sends).
set(WBA ${STM32CUBE_WBA})
set(WBA_APP ${WBA}/Projects/NUCLEO-WBA55CG/Applications/BLE/BLE_BeaconHCI)
set(WBA_MODULE ${WBA}/Projects/B-WBA5M-WPAN/Applications/BLE/BLE_HR_P2PServer/STM32CubeIDE)
set(WBA_WPAN ${WBA}/Middlewares/ST/STM32_WPAN)
get_filename_component(POWER_DIR ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)   # no ".." left in it: cmake 3.28 needs that for module files

add_library(power_wba_platform OBJECT
    ${CMAKE_CURRENT_LIST_DIR}/port.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_spi.c
    ${WBA_MODULE}/Application/User/Startup/startup_stm32wba5mjghx.s
    ${WBA_APP}/STM32CubeIDE/Application/User/Core/syscalls.c
    ${WBA_APP}/STM32CubeIDE/Application/User/Core/sysmem.c
    ${WBA_APP}/Core/Src/app_entry.c
    ${WBA_APP}/Core/Src/main.c
    ${WBA_APP}/Core/Src/stm32wbaxx_hal_msp.c
    ${WBA_APP}/Core/Src/stm32wbaxx_it.c
    ${WBA_APP}/Core/Src/system_stm32wbaxx.c
    ${WBA_APP}/STM32_WPAN/App/app_ble.c
    ${WBA_APP}/STM32_WPAN/Target/bleplat.c
    ${WBA_APP}/STM32_WPAN/Target/host_stack_if.c
    ${WBA_APP}/STM32_WPAN/Target/linklayer_plat.c
    ${WBA_APP}/STM32_WPAN/Target/ll_sys_if.c
    ${WBA_APP}/STM32_WPAN/Target/power_table.c
    ${WBA_APP}/System/Config/ADC_Ctrl/adc_ctrl_conf.c
    ${WBA_APP}/System/Config/Debug_GPIO/app_debug.c
    ${WBA_APP}/System/Config/LowPower/peripheral_init.c
    ${WBA_APP}/System/Config/LowPower/user_low_power_config.c
    ${WBA_APP}/System/Interfaces/stm32_lpm_if.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_adc.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_adc_ex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_cortex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_crc.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_crc_ex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_cryp.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_cryp_ex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma_ex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_exti.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash_ex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_gpio.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr_ex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_ramcfg.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc_ex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rng.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rng_ex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rtc.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rtc_ex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart_ex.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_ll_adc.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_ll_exti.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_ll_gpio.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_ll_icache.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_ll_pka.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_ll_tim.c
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_ll_utils.c
    ${WBA}/Middlewares/ST/STM32_WPAN/ble/svc/Src/svc_ctl.c
    ${WBA}/Middlewares/ST/STM32_WPAN/link_layer/ll_sys/src/ll_sys_cs.c
    ${WBA}/Middlewares/ST/STM32_WPAN/link_layer/ll_sys/src/ll_sys_dp_slp.c
    ${WBA}/Middlewares/ST/STM32_WPAN/link_layer/ll_sys/src/ll_sys_intf.c
    ${WBA}/Middlewares/ST/STM32_WPAN/link_layer/ll_sys/src/ll_sys_startup.c
    ${WBA}/Projects/Common/WPAN/Interfaces/adv_trace_usart_if.c
    ${WBA}/Projects/Common/WPAN/Interfaces/hw_aes.c
    ${WBA}/Projects/Common/WPAN/Interfaces/hw_otp.c
    ${WBA}/Projects/Common/WPAN/Interfaces/hw_rng.c
    ${WBA}/Projects/Common/WPAN/Interfaces/timer_if.c
    ${WBA}/Projects/Common/WPAN/Modules/BasicAES/baes_ccm.c
    ${WBA}/Projects/Common/WPAN/Modules/BasicAES/baes_cmac.c
    ${WBA}/Projects/Common/WPAN/Modules/BasicAES/baes_ecb.c
    ${WBA}/Projects/Common/WPAN/Modules/Log/log_module.c
    ${WBA}/Projects/Common/WPAN/Modules/MemoryManager/advanced_memory_manager.c
    ${WBA}/Projects/Common/WPAN/Modules/MemoryManager/stm32_mm.c
    ${WBA}/Projects/Common/WPAN/Modules/RTDebug/RTDebug.c
    ${WBA}/Projects/Common/WPAN/Modules/RTDebug/RTDebug_dtb.c
    ${WBA}/Projects/Common/WPAN/Modules/SerialCmdInterpreter/serial_cmd_interpreter.c
    ${WBA}/Projects/Common/WPAN/Modules/adc_ctrl.c
    ${WBA}/Projects/Common/WPAN/Modules/app_sys.c
    ${WBA}/Projects/Common/WPAN/Modules/otp.c
    ${WBA}/Projects/Common/WPAN/Modules/pka_ctrl.c
    ${WBA}/Projects/Common/WPAN/Modules/scm.c
    ${WBA}/Projects/Common/WPAN/Modules/stm_list.c
    ${WBA}/Projects/Common/WPAN/Modules/stm_queue.c
    ${WBA}/Projects/Common/WPAN/Modules/temp_measurement.c
    ${WBA}/Projects/Common/WPAN/Modules/wpan_utils.c
    ${WBA}/Projects/Common/WPAN/Startup/stm32wbaxx_ResetHandler_GCC.s
    ${WBA}/Utilities/lpm/tiny_lpm/stm32_lpm.c
    ${WBA}/Utilities/misc/stm32_mem.c
    ${WBA}/Utilities/misc/stm32_systime.c
    ${WBA}/Utilities/misc/stm32_tiny_sscanf.c
    ${WBA}/Utilities/misc/stm32_tiny_vsnprintf.c
    ${WBA}/Utilities/sequencer/stm32_seq.c
    ${WBA}/Utilities/tim_serv/stm32_timer.c
    ${WBA}/Utilities/trace/adv_trace/stm32_adv_trace.c)
target_link_libraries(power_wba_platform PUBLIC arch_cortex_m33f)
target_compile_definitions(power_wba_platform PUBLIC USE_HAL_DRIVER USE_FULL_LL_DRIVER STM32WBA5Mxx BLE_LL)
target_include_directories(power_wba_platform PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/overlay       # first: app_conf.h, ble_conf.h and the HAL's conf, each on top of ST's
    ${CMAKE_CURRENT_LIST_DIR}
    ${WBA_APP}/Core/Inc
    ${WBA_APP}/STM32_WPAN/App
    ${WBA_APP}/STM32_WPAN/Target
    ${WBA_APP}/System/Config
    ${WBA_APP}/System/Config/ADC_Ctrl
    ${WBA_APP}/System/Config/Debug_GPIO
    ${WBA_APP}/System/Config/Log
    ${WBA_APP}/System/Config/LowPower
    ${WBA_APP}/System/Interfaces
    ${WBA}/Drivers/CMSIS/Device/ST/STM32WBAxx/Include
    ${WBA}/Drivers/CMSIS/Include
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Inc
    ${WBA}/Drivers/STM32WBAxx_HAL_Driver/Inc/Legacy
    ${WBA_WPAN}
    ${WBA_WPAN}/ble/stack/include
    ${WBA_WPAN}/ble/stack/include/auto
    ${WBA_WPAN}/ble/svc/Inc
    ${WBA_WPAN}/ble/svc/Src
    ${WBA_WPAN}/link_layer/ll_cmd_lib/config/ble_basic
    ${WBA_WPAN}/link_layer/ll_cmd_lib/inc
    ${WBA_WPAN}/link_layer/ll_cmd_lib/inc/_40nm_reg_files
    ${WBA_WPAN}/link_layer/ll_cmd_lib/inc/ot_inc
    ${WBA_WPAN}/link_layer/ll_sys/inc
    ${WBA}/Projects/Common/WPAN/Interfaces
    ${WBA}/Projects/Common/WPAN/Modules
    ${WBA}/Projects/Common/WPAN/Modules/BasicAES
    ${WBA}/Projects/Common/WPAN/Modules/Log
    ${WBA}/Projects/Common/WPAN/Modules/MemoryManager
    ${WBA}/Projects/Common/WPAN/Modules/RTDebug
    ${WBA}/Projects/Common/WPAN/Modules/SerialCmdInterpreter
    ${WBA}/Utilities/lpm/tiny_lpm
    ${WBA}/Utilities/misc
    ${WBA}/Utilities/sequencer
    ${WBA}/Utilities/tim_serv
    ${WBA}/Utilities/trace/adv_trace)
# ST's libraries are built with gcc's enums (as small as their values allow); clang has to be told.
# No link time optimisation across code that is not ours.
target_compile_options(power_wba_platform PUBLIC -fno-lto $<$<OR:$<C_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:Clang>>:-fshort-enums>)
target_compile_options(power_wba_platform PRIVATE -w)    # ST's code is not written for -Werror
set_target_properties(power_wba_platform PROPERTIES INTERPROCEDURAL_OPTIMIZATION OFF)

# (ST's sysmem.c gets NULL and ptrdiff_t through gcc's errno.h; other C libraries' do not bring them)
set_source_files_properties(${WBA_APP}/STM32CubeIDE/Application/User/Core/sysmem.c PROPERTIES COMPILE_OPTIONS "-include;stddef.h")

# ST's linker script for the module is written for GNU ld; lld stops at two things in it. It
# uses its RAM region a few lines before it defines it: the build's copy has that line moved to
# the end. And it marks sections (READONLY), which only says what a section in the flash is
# anyway: left out. Its last 16 K of flash are kept free of code ("NVM"); the pairing record
# (port.c) is in the last page.
file(READ ${WBA_MODULE}/STM32WBA5MMGHX_FLASH.ld power_wba_ld)
string(REPLACE "_estack = ORIGIN(RAM) + LENGTH(RAM);" "" power_wba_ld "${power_wba_ld}")
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    string(REPLACE "(READONLY)" "" power_wba_ld "${power_wba_ld}")
endif()
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/power_wba.ld "${power_wba_ld}\n_estack = ORIGIN(RAM) + LENGTH(RAM);\n")

# add_power_wba_target(<name> <application source>)
function(add_power_wba_target name source)
    set(tgt ${name}.elf)
    add_executable(${tgt} ${source})
    # (the two libraries call each other: GNU ld has to be told to look again)
    target_link_libraries(${tgt} PRIVATE power_wba_platform -Wl,--start-group
        ${WBA_WPAN}/link_layer/ll_cmd_lib/lib/LinkLayer_BLE_Basic_V3_0.a
        ${WBA_WPAN}/ble/stack/lib/stm32wba_ble_stack_llo.a -Wl,--end-group)
    target_sources(${tgt} PRIVATE FILE_SET CXX_MODULES BASE_DIRS ${POWER_DIR} FILES
        ${POWER_DIR}/common/frame.cppm ${POWER_DIR}/common/font.cppm ${POWER_DIR}/common/screen.cppm ${POWER_DIR}/common/aes.cppm
        ${POWER_DIR}/common/messages.cppm ${POWER_DIR}/common/logic.cppm ${POWER_DIR}/common/dock.cppm)
    target_compile_options(${tgt} PRIVATE -fno-use-cxa-atexit -fno-threadsafe-statics -fno-exceptions -fno-rtti -fno-unwind-tables)
    set_target_properties(${tgt} PROPERTIES INTERPROCEDURAL_OPTIMIZATION OFF)
    target_link_options(${tgt} PRIVATE -fno-lto -T${CMAKE_CURRENT_BINARY_DIR}/power_wba.ld
        $<$<CXX_COMPILER_ID:GNU>:-Wl,--no-warn-rwx-segments>
        "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${name}.map")
    add_custom_command(TARGET ${tgt} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${tgt}> ${CMAKE_CURRENT_BINARY_DIR}/${name}.bin)
endfunction()

add_power_wba_target(power_radio ${POWER_DIR}/radio/radio_app.cpp)      # the power board's radio module
add_power_wba_target(power_remote ${POWER_DIR}/remote/remote_app.cpp)   # the e-stop remote
