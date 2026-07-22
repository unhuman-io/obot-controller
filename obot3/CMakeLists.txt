
add_executable(config1-1.elf main.cpp config1/config1.cpp g474_pins.cpp)
target_include_directories(config1-1.elf PRIVATE . config1/config1-1 ../motorlib/device/stm32g4/Include ../motorlib/CMSIS/Include)
target_link_options(config1-1.elf PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wl,--no-warn-rwx-segments>
    -T${CMAKE_CURRENT_LIST_DIR}/g474.ld
)
set_target_properties(config1-1.elf PROPERTIES
    LINK_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/g474.ld
)
