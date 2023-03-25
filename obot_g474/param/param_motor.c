    .fast_loop_param.adc1_gain = -3.3/4096/(.001*10*1.04),  // A/count, 1.04 is a measured factor
    .fast_loop_param.adc2_gain = -3.3/4096/(.001*10*1.04),
    .fast_loop_param.adc3_gain = -3.3/4096/(.001*10*1.04),
    .fast_loop_param.vbus_gain = 1.0/4096*(215+13.7)/13.7, // note adc1 set up with GCOMP, 1V -> 4096 counts
    .drv_regs = {
        (2<<11) | 0x00,  // control_reg 0x00, 6 PWM mode
        //(3<<11) | 0x3AA, // hs_reg      0x3CC, moderate drive current
        (3<<11) | 0x3FF, // hs_reg      0x3CC, moderate drive current
        //(4<<11) | 0x2FF, // ls_reg      0x0CC, no cycle by cycle, 500 ns tdrive
                                        // moderate drive current (.57,1.14A)
        (4<<11) | 0x37F, // ls_reg      0x0CC, no cycle by cycle, 4000 ns tdrive
                                        // moderate drive current (.57,1.14A)
        (5<<11) | 0x004,  // ocp_reg     0x00 -> 50 ns dead time, 
                                    //latched ocp, 2 us ocp deglitch, 0.31 Vds thresh -> .31/.003 = 100A
        //(6<<11) | 0x2C0, // csa_reg     0x2C0 -> bidirectional current, 40V/V
        //(6<<11) | 0x280,
        (6<<11) | 0x240, // csa_reg     0x240 -> bidirectional current, 10V/V
    },
