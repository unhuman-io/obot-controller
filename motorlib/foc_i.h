#ifndef FOC_I_H
#define FOC_I_H

#include "messages.h"

#ifdef __cplusplus
extern "C" {
#endif

void system_init();

void fast_loop_update();
void fast_loop_maintenance();
void fast_loop_set_id_des(float id);
void fast_loop_set_iq_des(float iq);
void fast_loop_phase_lock_mode(float id);
void fast_loop_current_mode();
void fast_loop_open_mode();
void fast_loop_brake_mode();
void fast_loop_set_param(const FastLoopParam *const fast_loop_param);
void fast_loop_get_status(FastLoopStatus * const fast_loop_status);
void fast_loop_voltage_mode();
void fast_loop_zero_current_sensors();

void main_loop_update();
void main_loop_set_param(MainLoopParam * const main_loop_param);
void main_loop_get_status(MainLoopStatus * const main_loop_status);

#ifdef __cplusplus
}
#endif

#endif
