set(OBOT_CONFIGS
    h2
    motor_enc
)

foreach(cfg IN LISTS OBOT_CONFIGS)
    add_executable(${cfg}.elf config_obot_g474_${cfg}.cpp)
    target_link_libraries(${cfg}.elf PRIVATE motorlib)
    target_compile_definitions(${cfg}.elf 
        PRIVATE 
            CONFIG=\"${cfg}\"
    )
endforeach()
