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

# foreach(cfg IN LISTS OBOT_CONFIGS)
#     add_executable(${cfg}.elf config_obot_g474_${cfg}.cpp ../param/param_obot_g474_${cfg}.c)
#     target_link_libraries(${cfg}.elf PRIVATE motorlib)
#     target_compile_definitions(${cfg}.elf
#         PRIVATE
#             CONFIG=\"${cfg}\"
#     )
# 
#     add_custom_command(OUTPUT ${cfg}.binp
#         DEPENDS ${cfg}.elf
#         COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${cfg}.elf> $<TARGET_FILE_DIR:${cfg}.elf>/${cfg}.bin
#         COMMENT "Generating ${cfg}.bin..."
#     )
#     add_custom_target(${cfg}_generate_bin ALL
#         DEPENDS ${cfg}.bin
#     )
# endforeach()
foreach(cfg IN LISTS OBOT_CONFIGS)
    set(tgt ${cfg}.elf)
    
    # 1. Use standard variables for the OUTPUT paths
    # This puts the bins in your build folder where CMake can track them
    set(bin_full ${CMAKE_CURRENT_BINARY_DIR}/${cfg}.bin)
    set(bin_no_param ${CMAKE_CURRENT_BINARY_DIR}/${cfg}_noparam.bin)
    set(tgz_file ${CMAKE_CURRENT_BINARY_DIR}/${cfg}.tgz)

    # 2. Define the target FIRST
    add_executable(${tgt} EXCLUDE_FROM_ALL config_obot_g474_${cfg}.cpp ../param/param_obot_g474_${cfg}.c)
    target_link_libraries(${tgt} PRIVATE motorlib)
    target_compile_definitions(${tgt} PRIVATE CONFIG=\"${cfg}\")
    set_source_files_properties(
        ../param/param_obot_g474_${cfg}.c
        PROPERTIES INTERPROCEDURAL_OPTIMIZATION FALSE
    )

    # 3. Generate binary variations
    add_custom_command(OUTPUT ${bin_full} ${bin_no_param}
        DEPENDS ${tgt}
        # Generator expression is only used HERE in the command
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${tgt}> ${bin_full}
        COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${tgt}> 
                --remove-section flash_param 
                --remove-section calibration_data 
                ${bin_no_param}
        COMMENT "Generating bin variations for ${cfg}..."
    )

    # 4. Create the TGZ package
    set(pkg_dir ${CMAKE_CURRENT_BINARY_DIR}/pkg_${cfg})
    set(load_script ${pkg_dir}/load_${cfg}.sh)

    add_custom_command(OUTPUT ${tgz_file}
        DEPENDS ${bin_no_param}
        
        # 1. Directory Setup
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${pkg_dir}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${pkg_dir}
        COMMAND ${CMAKE_COMMAND} -E copy ${bin_no_param} ${pkg_dir}/
        
        # 2. Script Generation (Using quotes to protect the #)
        COMMAND ${CMAKE_COMMAND} -E echo "#!/bin/bash -e" > ${load_script}
        COMMAND ${CMAKE_COMMAND} -E echo "dfu-util -a0 -s 0x8000000 -D ${cfg}_noparam.bin" >> ${load_script}
        COMMAND ${CMAKE_COMMAND} -E echo "dfu-util -a0 -s 0x8000000:8:leave -U /tmp/dfu_tmp || true" >> ${load_script}
        COMMAND chmod +x ${load_script}
        
        # 3. Packaging
        COMMAND ${CMAKE_COMMAND} -E chdir ${pkg_dir} ${CMAKE_COMMAND} -E tar czf ${tgz_file} --format=gnutar . 
        
        # CRITICAL: This ensures the shell receives the quotes correctly
        VERBATIM 
        COMMENT "Packaging ${cfg} into tarball..."
    )

    # 5. Final Targets
    add_custom_target(${cfg}_pkg DEPENDS ${tgz_file})

    add_custom_target(load_${cfg}
        COMMAND ${load_script}
        DEPENDS ${tgz_file}
        COMMENT "Flashing ${cfg}..."
    )

endforeach()

list(TRANSFORM OBOT_CONFIGS APPEND "_pkg" OUTPUT_VARIABLE ALL_OBOT_PKGS)

# 2. Feed that list directly into the master target
add_custom_target(really_build_all
    DEPENDS ${ALL_OBOT_PKGS}
    COMMENT "Triggering build and packaging for: ${ALL_OBOT_PKGS}"
)