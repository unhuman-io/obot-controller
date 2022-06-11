#include "../param/param_obot_g474.h"
#include "../st_device.h"
#include "../../motorlib/qep_encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/ma732_encoder.h"
#include "../../motorlib/peripheral/stm32g4/pin_config.h"


#define AD1_DR ADC2->JDR1
#define AD2_DR ADC2->JDR2
class ResolverEncoder : public EncoderBase {
 public:
    int32_t read() { return 10000/2/M_PI * 
        atan2f((int32_t) AD1_DR - 0x800, (int32_t) AD2_DR - 0x800); } // 10000 seems like an appropriate cpr
};


using TorqueSensor = TorqueSensorBase;
using MotorEncoder = ResolverEncoder;
using OutputEncoder = MA732Encoder;

extern "C" void SystemClock_Config();
void pin_config_obot_g474_motor_r0();

struct InitCode {
    InitCode() {
      SystemClock_Config();
      pin_config_obot_g474_motor_r0();
      SPI3->CR2 = (15 << SPI_CR2_DS_Pos);   // 16 bit
      // ORDER DEPENDANCE SPE set last
      SPI3->CR1 = SPI_CR1_MSTR | (3 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM | SPI_CR1_SPE;    // baud = clock/16
    
      // ADCs for differential read
      GPIO_SETL(C, 3, GPIO_MODE::ANALOG, GPIO_SPEED::LOW, 0);
      ADC2->DIFSEL = ADC_DIFSEL_DIFSEL_6 | ADC_DIFSEL_DIFSEL_7 | ADC_DIFSEL_DIFSEL_8 | ADC_DIFSEL_DIFSEL_9;
      ADC2->JSQR = 1 << ADC_JSQR_JL_Pos | 6 << ADC_JSQR_JSQ1_Pos | 8 << ADC_JSQR_JSQ2_Pos | 1 << ADC_JSQR_JEXTEN_Pos | 19 << ADC_JSQR_JEXTSEL_Pos; // trig 19 hrtim_adc_trg2 (injected)
      ADC2->CFGR = ADC_CFGR_JQDIS;
      ADC2->SMPR1 = 4 << ADC_SMPR1_SMP6_Pos |
                    4 << ADC_SMPR1_SMP7_Pos |
                    4 << ADC_SMPR1_SMP8_Pos |
                    4 << ADC_SMPR1_SMP9_Pos;  // 47.5 cycles
      ADC2->CFGR2 =  0;

      // remove 6 and 7 from single ended samples
      ADC1->JSQR = 1 << ADC_JSQR_JL_Pos | 16 << ADC_JSQR_JSQ1_Pos | 18 << ADC_JSQR_JSQ2_Pos; // internal temperature, vrefint
    
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    ResolverEncoder motor_encoder;
    TorqueSensor torque_sensor;
    GPIO output_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    MA732Encoder output_encoder(*SPI3, output_encoder_cs);
};

#include "config_obot_g474_motor.cpp"

void config_init() {
    System::api.add_api_variable("jbct", new APICallbackUint32([](){ return config::output_encoder.get_bct(); },
                    [](uint32_t u){ config::output_encoder.set_bct(u); }));
    System::api.add_api_variable("jet", new APICallbackUint32([](){ return config::output_encoder.get_et(); },
                    [](uint32_t u){ config::output_encoder.set_et(u); }));
    System::api.add_api_variable("jmgt", new APICallbackUint32([](){ return config::output_encoder.get_magnetic_field_strength(); },
                    [](uint32_t u){ config::output_encoder.set_mgt(u); }));
    System::api.add_api_variable("jfilt", new APICallbackUint32([](){ return config::output_encoder.get_filt(); }, 
                [](uint32_t u){ config::output_encoder.set_filt(u); }));
    System::api.add_api_variable("AD1", new const APIUint32(&AD1_DR));
    System::api.add_api_variable("AD2", new const APIUint32(&AD2_DR));
}

void config_maintenance() {}