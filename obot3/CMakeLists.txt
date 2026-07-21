
add_executable(config1-1.elf main.cpp config1/config1.cpp)
target_include_directories(config1-1.elf PRIVATE . config1/config1-1)

