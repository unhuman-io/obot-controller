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
    target_compile_options(${tgt} PRIVATE
        -fno-use-cxa-atexit
        -fno-threadsafe-statics
        -fno-exceptions
        -fno-rtti
        -fno-unwind-tables
    )
endfunction()

add_obot3_target(config1-1    device_stm32g474 config1/config1-1    config1/config1.cpp)
add_obot3_target(config1-g564 device_stm32g564 config1/config1-g564 config1/config1.cpp)
