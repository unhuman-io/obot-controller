# add_obot3_target(<name> <device_lib> <config_dir> <extra sources...>)
function(add_obot3_target name device config_dir)
    set(tgt ${name}.elf)
    add_executable(${tgt} main.cpp memcpy.cpp ${ARGN})
    target_link_libraries(${tgt} PRIVATE ${device})
    target_include_directories(${tgt} PRIVATE . ${config_dir})
    # the control modules are built per target, with its device's flags
    target_sources(${tgt} PRIVATE FILE_SET CXX_MODULES BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
        control/latest.cppm control/control_fun.cppm control/sincos.cppm control/foc.cppm control/motor_controller.cppm
        control/clock.cppm)
    target_link_options(${tgt} PRIVATE
        -nostartfiles       # <-- Drops crt0.o completely
        "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${name}.map"
    )
endfunction()

add_obot3_target(config1-1    device_stm32g474 config1/config1-1    config1/config1.cpp)
add_obot3_target(config1-g564 device_stm32g564 config1/config1-g564 config1/config1.cpp)
execute_process(
    COMMAND git describe --long --dirty --always --abbrev=7
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE OBOT3_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
# The same in numbers, for the enum reply (obot_enum.h)
set(OBOT3_VERSION_DEFINITIONS OBOT3_VERSION="${OBOT3_VERSION}"
    OBOT3_VERSION_MAJOR=0 OBOT3_VERSION_MINOR=0 OBOT3_VERSION_COMMITS=0 OBOT3_VERSION_HASH=0 OBOT3_VERSION_DIRTY=0)
if(OBOT3_VERSION MATCHES "^v?([0-9]+)\\.([0-9]+)[^-]*-([0-9]+)-g([0-9a-f]+)(-dirty)?$")
    set(OBOT3_VERSION_DEFINITIONS OBOT3_VERSION="${OBOT3_VERSION}"
        OBOT3_VERSION_MAJOR=${CMAKE_MATCH_1} OBOT3_VERSION_MINOR=${CMAKE_MATCH_2} OBOT3_VERSION_COMMITS=${CMAKE_MATCH_3}
        OBOT3_VERSION_HASH=0x${CMAKE_MATCH_4} OBOT3_VERSION_DIRTY=$<IF:$<BOOL:${CMAKE_MATCH_5}>,1,0>)
endif()

# Two builds of the tmotor driver, behind the can bootloader at 0x08002000
# (scripts/load_can.py loads the .bin there):
#   obot3_tmotor_driver           the text api, its variables and gdb (debug_monitor.h)
#   obot3_tmotor_driver_release   neither: enum, command and status frames only
function(add_tmotor_driver name)
    add_obot3_target(${name} device_stm32g474 tmotor_driver tmotor_driver/tmotor_driver.cpp)
    target_sources(${name}.elf PRIVATE FILE_SET CXX_MODULES BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
        tmotor_driver/tmotor_driver.cppm)
    target_compile_definitions(${name}.elf PRIVATE ${OBOT3_VERSION_DEFINITIONS} ${ARGN})
    target_link_options(${name}.elf PRIVATE -Wl,--defsym=CUSTOM_BOOTLOADER=1)
    add_custom_command(TARGET ${name}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${name}.elf> ${CMAKE_CURRENT_BINARY_DIR}/${name}.bin)
endfunction()
add_tmotor_driver(obot3_tmotor_driver OBOT3_TEXT_API=1)
add_tmotor_driver(obot3_tmotor_driver_release OBOT3_TEXT_API=0)

# The can bootloader in front of it: the first 8K of the flash, see bootloader/bootloader.cpp
add_executable(obot3_bootloader.elf bootloader/bootloader.cpp)
target_link_libraries(obot3_bootloader.elf PRIVATE device_stm32g474)
target_include_directories(obot3_bootloader.elf PRIVATE . bootloader)
target_compile_definitions(obot3_bootloader.elf PRIVATE ${OBOT3_VERSION_DEFINITIONS})
target_compile_options(obot3_bootloader.elf PRIVATE -Os)
target_link_options(obot3_bootloader.elf PRIVATE -nostartfiles -Wl,--defsym=BOOTLOADER=1
    "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/obot3_bootloader.map")
add_custom_command(TARGET obot3_bootloader.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:obot3_bootloader.elf> ${CMAKE_CURRENT_BINARY_DIR}/obot3_bootloader.bin)

# The power board and the e-stop remote of obot-cad/electronics, see power/README.md.
# power_main: the power board's main controller, STM32G564
add_obot3_target(power_main device_stm32g564 power/main power/main/power_main.cpp)
target_sources(power_main.elf PRIVATE FILE_SET CXX_MODULES BASE_DIRS ${CMAKE_CURRENT_LIST_DIR} FILES
    power/common/aes.cppm power/common/messages.cppm power/common/logic.cppm power/common/ina228.cppm
    power/main/uart.cppm power/main/i2c.cppm)
target_compile_definitions(power_main.elf PRIVATE ${OBOT3_VERSION_DEFINITIONS})
add_custom_command(TARGET power_main.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:power_main.elf> ${CMAKE_CURRENT_BINARY_DIR}/power_main.bin)
