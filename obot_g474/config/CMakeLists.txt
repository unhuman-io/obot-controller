
add_executable(h2.elf config_obot_g474_h2.cpp)
target_link_libraries(h2.elf motorlib)
target_compile_definitions(h2.elf
    PRIVATE
        CONFIG="h2"
)