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
#include "../../motorlib/peripheral/stm32g4/rtc.h"
#include "../../motorlib/round_robin_logger.h"


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

    FastLoop<PWM, MotorEncoder, Calibration> fast_loop {(int32_t) pwm_frequency, motor_pwm, motor_encoder, param->fast_loop_param, *calibration, &I_A_DR, &I_B_DR, &I_C_DR, &V_BUS_DR};

    BoardFun board_fun;
    PositionController position_controller {(float) (1.0/main_loop_frequency)};
    TorqueController torque_controller = {(float) (1.0/main_loop_frequency)};
    ImpedanceController impedance_controller = {(float) (1.0/main_loop_frequency)};
    VelocityController velocity_controller = {(float) (1.0/main_loop_frequency)};
    StateController state_controller = {(float) (1.0/main_loop_frequency)};
    JointPositionController joint_position_controller{1.0/main_loop_frequency};
    AdmittanceController admittance_controller = {1.0/main_loop_frequency};
    MainLoop<FastLoop<PWM, MotorEncoder, Calibration>,
        Driver,
        BoardFun,
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
        main_loop {(int32_t) main_loop_frequency,
            fast_loop,
            board_fun,
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

// Actuator<FastLoop<Trace2::PWM, Trace2::MotorEncoder, Calibration>,
//     MainLoop<FastLoop<Trace2::PWM, Trace2::MotorEncoder, Calibration>,
//         Trace2::Driver,
//         BoardFun,
//         PositionController,
//         TorqueController,
//         ImpedanceController,
//         VelocityController,
//         StateController,
//         JointPositionController,
//         AdmittanceController,
//         Trace2::Communication,
//         Trace2::LED,
//         Trace2::OutputEncoder,
//         Trace2::TorqueSensor>,
//     Calibration>
   Actuator actuator_(trace2.fast_loop, trace2.main_loop, param->startup_param, *calibration);

void config_init() {}

void config_maintenance() {}

extern "C" void board_init() {}

using Sys = System<Trace2::Communication, decltype(actuator_), RTClock>;

template<>
uint32_t Sys::count_ = 0;
template<>
ParameterAPI Sys::api = {};
template<>
uint32_t Sys::current_api_timeout_us_ = 0;
template<>
Trace2::Communication Sys::communication_ = trace2.communication;
template<>
decltype(actuator_) Sys::actuator_ = actuator_;
RoundRobinLogger round_robin_logger;

extern "C" void system_run() {
    Sys::run();
}

extern "C" void main_loop_interrupt() {
    Sys::main_loop_interrupt();
}

extern "C" void fast_loop_interrupt() {
    Sys::fast_loop_interrupt();
}

extern "C" void system_log(std::string s) {
    Sys::log(s);
}

extern "C" void system_loop_interrupt() {
    Sys::system_loop();
}



extern "C" void system_init() {


    DMAMUX1_Channel6->CCR =  DMA_REQUEST_I2C1_TX;
    DMAMUX1_Channel7->CCR =  DMA_REQUEST_I2C1_RX;
    DMAMUX1_Channel2->CCR =  DMA_REQUEST_SPI1_TX;
    DMAMUX1_Channel3->CCR =  DMA_REQUEST_SPI1_RX;
    if (trace2.motor_encoder.init()) {
        Sys::log("Motor encoder init success");
    } else {
        Sys::log("Motor encoder init failure");
        //init_failure |= 1;
    }
    if (trace2.output_encoder.init()) {
        Sys::log("Output encoder init success");
    } else {
        Sys::log("Output encoder init failure");
       // init_failure |= 1;
    }
    // if (drv_regs_error) {
    //     System::log("drv configure failure");
    //     init_failure |= 1;
    // } else {
    //     System::log("drv configure success");
    // }
    // if (config::torque_sensor.init()) {
    //     System::log("torque sensor init success");
    // } else {
    //     System::log("torque sensor init failure");
    //     init_failure |= 1;
    // }




   // Sys::api.add_api_variable("3v3", new APIFloat(&v3v3));
    Sys::api.add_api_variable("Tmicro", new APICallbackFloat([]{ return trace2.temp_sensor.get_value(); },
        [](float f){ trace2.temp_sensor.set_value(f); }));
    // System::api.add_api_variable("index_mod", new APIInt32(&index_mod));
    // System::api.add_api_variable("pwm_mult", new APICallbackUint8([](){return config::motor_pwm.get_frequency_multiplier();}, [](uint8_t mult){ config::motor_pwm.set_frequency_multiplier(mult);}));
    // System::api.add_api_variable("drv_err", new const APICallbackUint32([](){ return config::drv.get_drv_status(); }));
    // System::api.add_api_variable("drv_reset", new const APICallback([](){
    //     System::set_one_time_api_timeout_us(30 * 1000);
    //     return config::drv.drv_reset(); }));
    // System::api.add_api_variable("A1", new const APICallbackUint32([](){ return A1_DR; }));
    // System::api.add_api_variable("A2", new const APICallbackUint32([](){ return A2_DR; }));
    // System::api.add_api_variable("A3", new const APICallbackUint32([](){ return A3_DR; }));
    // System::api.add_api_variable("IA0", new const APIUint32(&ADC3->DR));
    // System::api.add_api_variable("IB0", new const APIUint32(&ADC4->DR));
    // System::api.add_api_variable("IC0", new const APIUint32(&ADC5->DR));
    // System::api.add_api_variable("IA", new const APIUint32(&ADC3->JDR1));
    // System::api.add_api_variable("IB", new const APIUint32(&ADC4->JDR1));
    // System::api.add_api_variable("IC", new const APIUint32(&ADC5->JDR1));
    // System::api.add_api_variable("usb_err", new APIUint32(&config::usb.error_count_));
    // System::api.add_api_variable("usb_reset_count", new APIUint32(&config::usb.reset_count_));
    // System::api.add_api_variable("hsi48_trim", new const APICallbackInt8([](){ return (int8_t) (((CRS->CR & CRS_CR_TRIM) >> CRS_CR_TRIM_Pos) - 64); }));
    // System::api.add_api_variable("shutdown", new const APICallback([](){
    //     // requires power cycle to return 
    //     setup_sleep();
    //     SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    //     PWR->CR1 |= 0b100 << PWR_CR1_LPMS_Pos;
    //     __WFI();
    //     return std::string();
    // }));
    // System::api.add_api_variable("deadtime", new APICallbackUint16([](){ 
    //     return config::motor_pwm.deadtime_ns_; }, [](uint16_t u) {config::motor_pwm.set_deadtime(u); }));
    //     System::api.add_api_variable("idelay", new APICallbackUint16([](){ 
    //         return config::motor_pwm.get_current_sample_delay(); }, [](uint16_t u) {config::motor_pwm.set_current_sample_delay(u); }));

    // System::api.add_api_variable("mcmp", new APIUint32(&HRTIM1->sMasterRegs.MCMP1R));
    // System::api.add_api_variable("t1cmp", new APIUint32(&TIM1->CCR1));

    // System::api.add_api_variable("flash_cal", new const APICallback([]{
    //     System::set_one_time_api_timeout_us(100 * 1000);
    //     void * adr = &_eccmram; // End of ccmram is an empty ram space. The linker script ensures that there is enough 
    //                             // space for the calibration to reside here temporarily
    //     Calibration *cal = (Calibration *) adr;
    //     std::memcpy(adr, calibration, sizeof(Calibration));
    //     cal->motor_encoder_bias = System::actuator_.startup_motor_bias_;
    //     cal->torque_sensor.bias = config::main_loop.torque_sensor_bias_;
    //     cal->torque_sensor.gain = config::main_loop.torque_sensor_.gain_;
    //     //cal->joint_encoder_bias;
    //     cal->output_encoder_bias = config::main_loop.output_encoder_bias_;
    //     if (std::isfinite(config::fast_loop.motor_index_electrical_offset_measured_)) {
    //         cal->motor_encoder_index_electrical_offset_pos = config::fast_loop.motor_index_electrical_offset_measured_;
    //     }
    //     config::flash.write((uint32_t) calibration, (uint32_t*) cal, sizeof(Calibration));
    //     return std::string("ok");
    // }));

    for (auto regs : std::vector<ADC_TypeDef*>{ADC1, ADC2, ADC3, ADC4, ADC5}) {
        regs->CR = ADC_CR_ADVREGEN;
        ns_delay(20000);
        regs->CR |= ADC_CR_ADCAL;
        while(regs->CR & ADC_CR_ADCAL);
        ns_delay(100);
        regs->CR |= ADC_CR_ADCALDIF;
        regs->CR |= ADC_CR_ADCAL;
        while(regs->CR & ADC_CR_ADCAL);
        ns_delay(100);

        regs->ISR = ADC_ISR_ADRDY;
        regs->CR |= ADC_CR_ADEN;
        while(!(regs->ISR & ADC_ISR_ADRDY));
    }

    ADC1->CR |= ADC_CR_JADSTART;
    while(ADC1->CR & ADC_CR_JADSTART);

    float v3v3 =  *((uint16_t *) (0x1FFF75AA)) * 3.0 / V_REF_DR;
    Sys::log("3v3: " + std::to_string(v3v3));
    Sys::log("obias: " +  std::to_string(calibration->output_encoder_bias));
    Sys::log("tbias: " + std::to_string(calibration->torque_sensor.bias));
    Sys::log("tgain: " + std::to_string(calibration->torque_sensor.gain));
    Sys::log("offset: " + std::to_string(calibration->motor_encoder_index_electrical_offset_pos));
    Sys::log("mbias: " + std::to_string(calibration->motor_encoder_bias));

    ADC1->GCOMP = v3v3*4096;
    ADC1->CFGR2 |= ADC_CFGR2_GCOMP;
    ADC1->CR |= ADC_CR_ADSTART;
    ADC2->CR |= ADC_CR_JADSTART;
    ADC5->CR |= ADC_CR_JADSTART | ADC_CR_ADSTART;
    ADC5->IER |= ADC_IER_JEOSIE;
    ADC4->CR |= ADC_CR_JADSTART | ADC_CR_ADSTART;
    ADC3->CR |= ADC_CR_JADSTART | ADC_CR_ADSTART;

    //config_init();

    trace2.main_loop.init();

//          regs_.sTimerxRegs[ch].TIMxCR |= HRTIM_TIMCR_PREEN | HRTIM_TIMCR_TRSTU | HRTIM_TIMCR_CONT;

    NVIC_SetPriority(HRTIM1_Master_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0));
    NVIC_EnableIRQ(HRTIM1_Master_IRQn);
    HRTIM1->sMasterRegs.MDIER |= HRTIM_MDIER_MCMP1IE; // interrupt on MCMP1
   
    HRTIM1->sMasterRegs.MCMP1R = 400;
    static_assert(main_loop_frequency > CPU_FREQUENCY_HZ/4/65536, "Main loop frequency too low");
    HRTIM1->sMasterRegs.MPER = CPU_FREQUENCY_HZ/4/main_loop_frequency;
    HRTIM1->sMasterRegs.MCR = 0 << HRTIM_MCR_SYNC_SRC_Pos | 2 << HRTIM_MCR_SYNC_OUT_Pos | HRTIM_MCR_CONT | HRTIM_MCR_PREEN | HRTIM_MCR_MREPU | 7 << HRTIM_MCR_CK_PSC_Pos; // CPU_FREQUENCY * 32 / 2^7 = 42.5 MHz
    trace2.usb.connect();

    HRTIM1->sMasterRegs.MCR |= HRTIM_MCR_MCEN + HRTIM_MCR_TACEN + HRTIM_MCR_TDCEN + HRTIM_MCR_TECEN + HRTIM_MCR_TFCEN; // start high res timer, also triggers TIM1
    //TraceBoard::ABC2 abc2;
}

