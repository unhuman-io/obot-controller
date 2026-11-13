# add_obot3_target(<name> <device_lib> <config_dir> <extra sources...>)
function(add_obot3_target name device config_dir)
    set(tgt ${name}.elf)
    add_executable(${tgt} main.cpp ${ARGN})
    target_link_libraries(${tgt} PRIVATE ${device})
    target_include_directories(${tgt} PRIVATE . ${config_dir})
    target_link_options(${tgt} PRIVATE
        -nostartfiles       # <-- Drops crt0.o completely
        "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${name}.map"
    )
endfunction()

add_obot3_target(config1-1    device_stm32g474 config1/config1-1    config1/config1.cpp)
add_obot3_target(config1-g564 device_stm32g564 config1/config1-g564 config1/config1.cpp)
add_obot3_target(obot3_tmotor_driver device_stm32g474 tmotor_driver tmotor_driver/tmotor_driver.cpp)
# Behind the can bootloader at 0x08002000: obot3_tmotor_driver.bin is what
# scripts/load_can.py loads there, see tmotor_driver/test_can.sh
execute_process(
    COMMAND git describe --long --dirty --always --abbrev=7
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE OBOT3_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
target_compile_definitions(obot3_tmotor_driver.elf PRIVATE OBOT3_VERSION="${OBOT3_VERSION}")
target_link_options(obot3_tmotor_driver.elf PRIVATE -Wl,--defsym=CUSTOM_BOOTLOADER=1)
add_custom_command(TARGET obot3_tmotor_driver.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:obot3_tmotor_driver.elf> ${CMAKE_CURRENT_BINARY_DIR}/obot3_tmotor_driver.bin)
