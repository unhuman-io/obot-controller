#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"

#include "../controller/position_controller.h"
#include "../controller/torque_controller.h"
#include "../controller/impedance_controller.h"
#include "../controller/velocity_controller.h"
#include "../controller/state_controller.h"
#include "../controller/joint_position_controller.h"
#include "../controller/admittance_controller.h"


#include "../../motorlib/logger.h"
import config_obot_g474_tracec;
import system2;
import fast_loop;
import main_loop;
import actuator;

const Param * const param = (const Param * const) 0x8060000;
const Calibration * const calibration = (const Calibration * const) 0x8070000;

constexpr uint32_t main_loop_frequency = 10000;    
constexpr uint32_t pwm_frequency = 50000;

class Trace2 : public TraceBoard {
public:
    Trace2() : TraceBoard(pwm_frequency, main_loop_frequency){
        // trace pins enable
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
        GPIO_SETL(E, 2, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 3, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 5, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        GPIO_SETL(E, 6, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 0);
        //ETM->CR |= ETM_CR_ETMEN;
        uint32_t *etmcr = (uint32_t *)0xE0041000;
        *etmcr |= 1 << 11;
        uint32_t *etmteevr = (uint32_t *)0xE0041020;
        *etmteevr = 0x000037ef; // ON
    }

    using TorqueSensor = TorqueSensorBase;
    using MotorEncoder = EncoderBase;
    using OutputEncoder = EncoderBase;

    MotorEncoder motor_encoder;
    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;

    FastLoop<PWM, MotorEncoder, Calibration> fast_loop = {(int32_t) pwm_frequency, motor_pwm, motor_encoder, param->fast_loop_param, *calibration, &I_A_DR, &I_B_DR, &I_C_DR, &V_BUS_DR};

    PositionController position_controller = {(float) (1.0/main_loop_frequency)};
    TorqueController torque_controller = {(float) (1.0/main_loop_frequency)};
    ImpedanceController impedance_controller = {(float) (1.0/main_loop_frequency)};
    VelocityController velocity_controller = {(float) (1.0/main_loop_frequency)};
    StateController state_controller = {(float) (1.0/main_loop_frequency)};
    JointPositionController joint_position_controller{1.0/main_loop_frequency};
    AdmittanceController admittance_controller = {1.0/main_loop_frequency};
    MainLoop<FastLoop<PWM, MotorEncoder, Calibration>,
        Driver,
        PositionController,
        TorqueController,
        ImpedanceController,
        VelocityController,
        StateController,
        JointPositionController,
        AdmittanceController,
        Communication,
        LED,
        OutputEncoder,
        TorqueSensor>
        main_loop = {main_loop_frequency,
            fast_loop,
            position_controller,
            torque_controller,
            impedance_controller,
            velocity_controller,
            state_controller,
            joint_position_controller,
            admittance_controller,
            communication,
            led,
            output_encoder,
            torque_sensor,
            drv,
            param->main_loop_param,
            *calibration};
};

Trace2 trace2;

Logger::CIndex log_index __attribute__((section(".noload")));
char log_queue[LOGGING_MAX_SIZE] __attribute__((section(".noload")));
__attribute__ ((init_priority(LOGGER_INIT_PRIORITY))) Logger logger(log_index, log_queue);
extern const char * const name = "obot_g474_trace2";

extern "C" void usb_interrupt() {
    trace2.usb.interrupt();
}

Actuator<FastLoop<Trace2::PWM, Trace2::MotorEncoder, Calibration>,
    MainLoop<FastLoop<Trace2::PWM, Trace2::MotorEncoder, Calibration>,
        Trace2::Driver,
        PositionController,
        TorqueController,
        ImpedanceController,
        VelocityController,
        StateController,
        JointPositionController,
        AdmittanceController,
        Trace2::Communication,
        Trace2::LED,
        Trace2::OutputEncoder,
        Trace2::TorqueSensor>,
    Calibration>
     actuator_(trace2.fast_loop, trace2.main_loop, param->startup_param, *calibration);

void config_init() {}

void config_maintenance() {}

extern "C" void board_init() {}

extern "C" void system_run() {
    //System<>::run();
}

extern "C" void main_loop_interrupt() {
 //   System::main_loop_interrupt();
}

extern "C" void fast_loop_interrupt() {
  //  System::fast_loop_interrupt();
}

extern "C" void system_log(std::string s) {
 //   System::log(s);
}

extern "C" void system_loop_interrupt() {
  //  System::system_loop();
}
extern "C" void system_init() {}