set(OBOT_CONFIGS
    h2
    hands
    motor_9_pzdma
    motor_9
    motor_40_osa14
   # motor_ads1235_2
   # motor_ads1235
   # motor_aeat9922
    motor_aksim_19_17
    motor_aksim_19_18
    motor_aksim_20_19
    motor_aksim_joint
    motor_aksim
   # motor_bmi270
    motor_can
   # motor_enc_standalone
   # motor_enc_u10
    motor_enc
    motor_hall
   # motor_ma732
    motor_max11158
   # motor_max11270
   # motor_pz2_2
    motor_pz2

    
)

foreach(cfg IN LISTS OBOT_CONFIGS)
    add_executable(${cfg}.elf config_obot_g474_${cfg}.cpp)
    target_link_libraries(${cfg}.elf PRIVATE motorlib)
    target_compile_definitions(${cfg}.elf
        PRIVATE
            CONFIG=\"${cfg}\"
    )
    add_custom_command(TARGET ${cfg}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${cfg}.elf> $<TARGET_FILE_DIR:${cfg}.elf>/${cfg}.bin
    COMMENT "Generating ${cfg}.bin..."
)
endforeach()
