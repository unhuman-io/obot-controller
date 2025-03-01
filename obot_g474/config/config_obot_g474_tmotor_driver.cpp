#include "../param/param_obot_g474_can.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"
#include "../../motorlib/sensors/encoders/ma7xx_encoder.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_CAN_USB
#define CAN_ARB_DATA_RATE CAN::ArbitrationBaudRate::ARB_2M, CAN::DataBaudRate::DATA_10M
#define CAN_NUM CAN::CAN3
#define TMR0

template<class Encoder>
class EncoderGearRatio : public EncoderBase {
 public:
    EncoderGearRatio(Encoder &encoder, float gear_ratio) : 
        EncoderBase(), encoder_(encoder) {
            transmission_ratio_ = 1/gear_ratio;
        }
    int32_t read() {
        return get_value();
    }
    int32_t get_value() {
        return encoder_.get_value()*transmission_ratio_;
    }
    bool index_received() {
        return encoder_.index_received();
    }
 private:
    Encoder &encoder_;
    float transmission_ratio_;
};

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = MA732Encoder;
using OutputEncoder = EncoderGearRatio<MotorEncoder>;

// note can: sudo ip link set can0 up type can bitrate 2000000 dbitrate 10000000 fd on one-shot on restart-ms 100

struct InitCode {
    InitCode() {
        GPIO_SETH(A, 8, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 11); // can3 rx
        GPIO_SETL(B, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 11); // can3 tx

        RCC->APB1ENR1 |= RCC_APB1ENR1_FDCANEN;

      // if defined(INTERFACE_BBS)
      // PA0 TCS
      GPIO_SETL(A, 0, GPIO_MODE::OUTPUT, GPIO_SPEED::MEDIUM, 0); 
      GPIOA->BSRR = GPIO_BSRR_BS0;
      GPIOC->BSRR = GPIO_BSRR_BS3;  // hdr17 (1CS2), output encoder cs
        
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    GPIO motor_encoder_cs(*GPIOD, 2, GPIO::OUTPUT);
    MA732Encoder motor_encoder(*SPI3, motor_encoder_cs, SPIDMA::spi_pause[SPIDMA::SP3]);
    TorqueSensor torque_sensor;
    OutputEncoder output_encoder(motor_encoder, 6);
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {}

void config_maintenance() {}