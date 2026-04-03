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

function(add_obot_param_target SOURCE_FILE)
    # Get the filename without extension (e.g., "motor_resolute")
    get_filename_component(FILE_BASE ${SOURCE_FILE} NAME_WLE)
    
    set(TGT_NAME "${FILE_BASE}")
    set(BIN_OUT  "${CMAKE_CURRENT_BINARY_DIR}/${FILE_BASE}.bin")
    
    # 1. Create the data-only executable
    add_executable(${TGT_NAME} EXCLUDE_FROM_ALL ${SOURCE_FILE})
    target_compile_definitions(${TGT_NAME}
        PUBLIC
            OBOT_HASH=\"${OBOT_HASH}\"
    )
    # 3. Strip LTO and disable standard startup
    set_target_properties(${TGT_NAME} PROPERTIES 
        INTERPROCEDURAL_OPTIMIZATION FALSE
        INTERPROCEDURAL_OPTIMIZATION_RELEASE FALSE
        INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO FALSE
        INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL FALSE
    )

    target_link_options(${TGT_NAME} PRIVATE 
        "-nostartfiles" 
        "-nodefaultlibs" 
        "-Wl,--entry=0"
    )

    # 4. Binary Extraction
    add_custom_command(
        TARGET ${TGT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary -S -j flash_param $<TARGET_FILE:${TGT_NAME}> ${BIN_OUT}
        COMMENT "Generated Parameter Binary: ${BIN_OUT}"
        VERBATIM
    )
endfunction()

function(add_obot_firmware_target obot_config)
   set(tgt ${obot_config}.elf)
    
    # 1. Use standard variables for the OUTPUT paths
    # This puts the bins in your build folder where CMake can track them
    set(bin_full ${CMAKE_CURRENT_BINARY_DIR}/${obot_config}.bin)
    set(bin_no_param ${CMAKE_CURRENT_BINARY_DIR}/${obot_config}_noparam.bin)
    set(tgz_file ${CMAKE_CURRENT_BINARY_DIR}/${obot_config}.tgz)

    # 2. Define the target FIRST
    add_executable(${tgt} EXCLUDE_FROM_ALL config_obot_g474_${obot_config}.cpp ../param/param_obot_g474_${obot_config}.c ${CALIBRATION_FILE})
    target_link_libraries(${tgt} PRIVATE motorlib)
    target_compile_definitions(${tgt} PRIVATE CONFIG=\"${obot_config}\")

    set_source_files_properties(
        ../param/param_obot_g474_${obot_config}.c
        ${CALIBRATION_FILE}
        PROPERTIES COMPILE_OPTIONS "-fno-lto"
    )
    add_obot_param_target(../param/param_obot_g474_${obot_config}.c)

    # 3. Generate binary variations
    add_custom_command(OUTPUT ${bin_full} ${bin_no_param}
        DEPENDS ${tgt}
        # Generator expression is only used HERE in the command
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${tgt}> ${bin_full}
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${tgt}> 
                --remove-section flash_param 
                --remove-section calibration_data 
                ${bin_no_param}
        COMMENT "Generating bin variations for ${obot_config}..."
    )

    # 4. Create the TGZ package
    set(pkg_dir ${CMAKE_CURRENT_BINARY_DIR}/${obot_config})
    set(load_script ${pkg_dir}/load_${obot_config}.sh)

    add_custom_command(OUTPUT ${tgz_file}
        DEPENDS ${bin_no_param}
        
        # 1. Directory Setup
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${pkg_dir}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${pkg_dir}
        COMMAND ${CMAKE_COMMAND} -E copy ${bin_no_param} ${pkg_dir}/
        
        # 2. Script Generation (Using quotes to protect the #)
        COMMAND ${CMAKE_COMMAND} -E echo "#!/bin/bash -e" > ${load_script}
        COMMAND ${CMAKE_COMMAND} -E echo "dfu-util -a0 -s 0x8000000 -D ${obot_config}_noparam.bin" >> ${load_script}
        COMMAND ${CMAKE_COMMAND} -E echo "dfu-util -a0 -s 0x8000000:8:leave -U /tmp/dfu_tmp || true" >> ${load_script}
        COMMAND chmod +x ${load_script}
        
        # 3. Packaging
        COMMAND ${CMAKE_COMMAND} -E tar czf ${tgz_file} --format=gnutar ${pkg_dir}/
        
        # CRITICAL: This ensures the shell receives the quotes correctly
        VERBATIM 
        COMMENT "Packaging ${obot_config} into tarball..."
    )

    # 5. Final Targets
    add_custom_target(obot_g474_${obot_config}_pkg DEPENDS ${tgz_file})

    add_custom_target(load_${obot_config}
        COMMAND ${load_script}
        DEPENDS ${tgz_file}
        COMMENT "Flashing ${obot_config}..."
    )
endfunction()


foreach(cfg IN LISTS OBOT_CONFIGS)
    add_obot_firmware_target(${cfg})
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
