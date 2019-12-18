

// Save param to a file for uploading

// build with gcc param.c ../parameters/param_ec16.c param_gen.cpp -lstdc++ -o param_gen

#include "../../motorlib/param.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include "CLI11.hpp"

void print_param(Param *p);

int main(int argc, char** argv) {
    init_param_from_flash();
    Param p = *param();
    p.fast_loop_param.foc_param.current_filter_frequency_hz = 5000;

    CLI::App app("Manage parameters for ST project");
    app.add_option("-n,--name", p.name, "Name for the board", true);
    app.add_option("-p,--num_motor_pole_pairs", p.fast_loop_param.foc_param.num_poles, \
        "Encoder number of pole pairs", true);
    app.add_option("-e,--encoder_cpr", p.fast_loop_param.motor_encoder.cpr, "Encoder counts per revolution." \
        "Note this is 4x lines per revolution for quadrature", true);
    app.add_option("-d,--encoder_dir", p.fast_loop_param.motor_encoder.dir, "Encoder direction, 0 or 1", true);
    app.add_option("-o,--phase_order", p.fast_loop_param.phase_mode, "Phase mode, 0 or 1", true);
    app.add_option("--pwm_frequency", p.fast_loop_param.pwm_frequency, "PWM frequency in Hz", true);
    app.set_config("-c,--config");

    CLI::App *fast_loop_param = app.add_subcommand("fast_loop_param");
    float current_sensor_V_per_A = 3.3/4096/p.fast_loop_param.adc1_gain;
    fast_loop_param->add_option("--current_sensor_gain", current_sensor_V_per_A, "Current sensor V/A", true);

    CLI::App *foc_param = fast_loop_param->add_subcommand("foc_param");
    foc_param->add_option("--kp", p.fast_loop_param.foc_param.pi_q.kp, "current control kp", true);
    foc_param->add_option("--ki", p.fast_loop_param.foc_param.pi_q.ki, "current control ki", true);
    foc_param->add_option("--ki_limit", p.fast_loop_param.foc_param.pi_q.ki_limit, "current control ki_limit", true);
    foc_param->add_option("--command_max", p.fast_loop_param.foc_param.pi_q.command_max, "current control max command", true);
 
    CLI::App *main_loop_param = app.add_subcommand("main_loop_param");
    main_loop_param->add_option("--update_frequency", p.main_loop_param.update_frequency, "Update frequency Hz", true);
    main_loop_param->add_option("--gear_ratio", p.main_loop_param.gear_ratio, "Gear ratio", true);
    main_loop_param->add_option("--kt", p.main_loop_param.kt, "Motor kt", true);
    main_loop_param->add_option("--kp", p.main_loop_param.controller_param.kp, "position control kp", true);
    main_loop_param->add_option("--kd", p.main_loop_param.controller_param.kd, "position control kd", true);
    main_loop_param->add_option("--command_max", p.main_loop_param.controller_param.command_max, "position control max command", true);
    main_loop_param->add_option("--velocity_filter", p.main_loop_param.controller_param.velocity_filter_frequency_hz, "velocity filter frequency hz", true);

    CLI::App *output_encoder_param = main_loop_param->add_subcommand("output_encoder");
    output_encoder_param->add_option("--cpr", p.main_loop_param.output_encoder.cpr, "Output encoder CPR", true);

    CLI::App *startup_param = app.add_subcommand("startup_param");
    startup_param->add_option("--do_phase_lock", p.startup_param.do_phase_lock = 1, "Do phase lock at startup", true);
    startup_param->add_option("--phase_lock_duration",p.startup_param.phase_lock_duration, "phase lock duration seconds", true);
    startup_param->add_option("--phase_lock_current",p.startup_param.phase_lock_current, "phase lock current A", true);

    CLI11_PARSE(app, argc, argv);

    // TODO maybe option for pi_d
    p.fast_loop_param.foc_param.pi_d = p.fast_loop_param.foc_param.pi_q;

    p.fast_loop_param.adc1_gain = 3.3/4096/current_sensor_V_per_A;
    p.fast_loop_param.adc2_gain = p.fast_loop_param.adc1_gain;
    p.fast_loop_param.adc3_gain = p.fast_loop_param.adc1_gain;

    std::ofstream myfile;
    myfile.open ("param.bin");
    auto pos = myfile.tellp();
    myfile.write((char *) &p, sizeof(p));
    std::cout << "Wrote " << myfile.tellp()-pos << " bytes" << std::endl;
    myfile.close();

    print_param(&p);
}

void print_param(Param *p) {
    std::cout << "Name: " << p->name << std::endl;
    std::cout << "Number of pole pairs: " << p->fast_loop_param.foc_param.num_poles << std::endl;
    std::cout << "Encoder direction: " << p->fast_loop_param.motor_encoder.dir << std::endl;
    std::cout << "Phase order: " << (p->fast_loop_param.phase_mode ? "switched" : "standard") << std::endl;
    std::cout << "Encoder cpr: " << p->fast_loop_param.motor_encoder.cpr << std::endl;
    std::cout << "Current sensor 1 V/A: " << 3.3/4096/p->fast_loop_param.adc1_gain << std::endl;
    std::cout << "Do phase lock: " << (p->startup_param.do_phase_lock ? "yes" : "no") << std::endl;
    std::cout << "Phase lock_duration: " << p->startup_param.phase_lock_duration << " s" << std::endl;
    std::cout << "Phase lock_current: " << p->startup_param.phase_lock_current << " A" << std::endl;
    std::cout << "PWM frequency: " << p->fast_loop_param.pwm_frequency << " Hz" << std::endl;

}