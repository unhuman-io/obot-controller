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
   # motor_qia128_spi
    motor_resolute_max11158
    motor_resolute
   # motor_resolute2
    motor_resolute3
    motor_template
    motor_timing
    motor_wheel
    nucleo_spi
    nucleo_uart
    nucleo_uart_obot
    #osa_enc
    #osa14_jl1
    #osa14_standalone
    osa14
    #template
    tmotor_driver
    trace_6enc
    trace_13enc
    trace_can
    trace_spidebug
    #trace_test
    trace_usartdebug
    trace

)

foreach(cfg IN LISTS OBOT_CONFIGS)
    add_obot_firmware_target(${cfg})
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        target_sources(${cfg}.elf PRIVATE ../memopt.c)
    endif()
endforeach()

# 1. Prepend the family name
list(TRANSFORM OBOT_CONFIGS PREPEND "obot_g474_")

# 2. Append the target suffix and output to the new variable
list(TRANSFORM OBOT_CONFIGS APPEND "_pkg" OUTPUT_VARIABLE ALL_OBOT_PKGS)

# 2. Feed that list directly into the master target
add_custom_target(really_build_all
    DEPENDS ${ALL_OBOT_PKGS}
    COMMENT "Triggering build and packaging for: ${ALL_OBOT_PKGS}"
)
