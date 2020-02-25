#include "foc_i.h"
#include "config.h"

void system_init() {
    config.main_loop.init();
}

void fast_loop_update() {
    config.fast_loop.update();
}

void fast_loop_maintenance() {
    config.fast_loop.maintenance();
}

void fast_loop_set_id_des(float id) {
    config.fast_loop.set_id_des(id);
}

void fast_loop_set_iq_des(float iq) {
    config.fast_loop.set_iq_des(iq);
}

void fast_loop_set_tuning_amplitude(float amplitude) {
    config.fast_loop.set_tuning_amplitude(amplitude);
}

void fast_loop_set_tuning_frequency(float frequency){
    config.fast_loop.set_tuning_frequency(frequency);
}

void fast_loop_phase_lock_mode(float id) {
    config.fast_loop.phase_lock_mode(id);
}

void fast_loop_current_mode() {
    config.fast_loop.current_mode();
}

void fast_loop_current_tuning_mode() {
    config.fast_loop.current_tuning_mode();
}

void fast_loop_brake_mode() {
    config.fast_loop.brake_mode();
}

void fast_loop_open_mode() {
    config.fast_loop.open_mode();
}

void fast_loop_set_param(const FastLoopParam *const fast_loop_param) {
    config.fast_loop.set_param(*fast_loop_param);
}

void fast_loop_get_status(FastLoopStatus * const fast_loop_status) {
    config.fast_loop.get_status(fast_loop_status);
}

void fast_loop_voltage_mode() {
    config.fast_loop.voltage_mode();
}

void fast_loop_zero_current_sensors() {
    config.fast_loop.zero_current_sensors();
}


void main_loop_update() {
    config.main_loop.update();
}

void main_loop_set_param(MainLoopParam * const main_loop_param) {
    config.main_loop.set_param(*main_loop_param);
}

void main_loop_get_status(MainLoopStatus * const main_loop_status) {
    config.main_loop.get_status(main_loop_status);
}
