# add_obot3_target(<name> <arch_lib> <config_dir> <linker_script> <extra sources...>)
function(add_obot3_target name arch config_dir ld)
    set(tgt ${name}.elf)
    add_executable(${tgt} main.cpp ${ARGN})
    target_link_libraries(${tgt} PRIVATE ${arch})
    target_include_directories(${tgt} PRIVATE . ${config_dir} ../motorlib/device/stm32g4/Include ../motorlib/CMSIS/Include)
    target_link_options(${tgt} PRIVATE
        $<$<CXX_COMPILER_ID:GNU>:-Wl,--no-warn-rwx-segments>
        -T${CMAKE_CURRENT_LIST_DIR}/${ld}
        -nostartfiles       # <-- Drops crt0.o completely
        "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${name}.map"
    )
    set_target_properties(${tgt} PROPERTIES
        LINK_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/${ld}
    )
    target_compile_options(${tgt} PRIVATE
        -fno-use-cxa-atexit
        -fno-threadsafe-statics
        -fno-exceptions
        -fno-rtti
        -fno-unwind-tables
    )
endfunction()

add_obot3_target(config1-1 arch_cortex_m4f config1/config1-1 g474.ld config1/config1.cpp)

# Placeholder Cortex-M33F build: same sources/headers/linker script as the
# G474 target, only the core flags differ. Swap in the real device support
# when it is available.
add_obot3_target(config1-1-m33 arch_cortex_m33f config1/config1-1 g474.ld config1/config1.cpp)
