
add_executable(config1-1.elf main.cpp config1/config1.cpp g474_pins.cpp)
target_include_directories(config1-1.elf PRIVATE . config1/config1-1 ../motorlib/device/stm32g4/Include ../motorlib/CMSIS/Include)

