#include "../param/param_obot_g474.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include "../../motorlib/gpio.h"
#include <algorithm>
#include "../../motorlib/peripheral/stm32g4/pin_config.h"
#define COMMS   COMMS_CAN
#define CAN_NUM CAN::CAN3

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;

struct InitCode {
    InitCode() {
        GPIO_SETL(B, 3, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 11); // can3 rx
        GPIO_SETL(B, 4, GPIO_MODE::ALT_FUN, GPIO_SPEED::VERY_HIGH, 11); // can3 tx

        RCC->APB1ENR1 |= RCC_APB1ENR1_FDCANEN;

        // Only if not reset
        // FDCAN3->CCCR |= FDCAN_CCCR_INIT;
        // while (!(FDCAN3->CCCR & FDCAN_CCCR_INIT));
        FDCAN3->CCCR |= FDCAN_CCCR_CCE;

        // DTSEG1 + DTSEG2 + 3 = 170/12 -> 14
        //FDCAN3->DBTP = 8 << FDCAN_DBTP_DTSEG1_Pos | 3 << FDCAN_DBTP_DTSEG2_Pos | 2 << FDCAN_DBTP_DSJW_Pos;

        // // DTSEG1 + DTSEG2 + 3 = 170/5 -> 34
        FDCAN3->DBTP = 21 << FDCAN_DBTP_DTSEG1_Pos | 10 << FDCAN_DBTP_DTSEG2_Pos | 4 << FDCAN_DBTP_DSJW_Pos | FDCAN_DBTP_TDC | 0 << FDCAN_DBTP_DBRP_Pos;
        FDCAN3->TDCR = 9 << FDCAN_TDCR_TDCO_Pos | 5 << FDCAN_TDCR_TDCF_Pos; // normal delay 180 ns, glitch filter 150ns*170MHz = 25

        // // DTSEG1 + DTSEG2 + 3 = 170/2/2 -> 42.5 
        // Note seems to have problems when dBRP > 2
        FDCAN3->DBTP = 30 << FDCAN_DBTP_DTSEG1_Pos | 10 << FDCAN_DBTP_DTSEG2_Pos | 4 << FDCAN_DBTP_DSJW_Pos | FDCAN_DBTP_TDC | 1 << FDCAN_DBTP_DBRP_Pos;
        FDCAN3->TDCR = 9 << FDCAN_TDCR_TDCO_Pos | 5 << FDCAN_TDCR_TDCF_Pos; // normal delay 180 ns, glitch filter 150ns*170MHz = 25

        FDCAN3->DBTP = 23 << FDCAN_DBTP_DTSEG1_Pos | 8 << FDCAN_DBTP_DTSEG2_Pos | 8 << FDCAN_DBTP_DSJW_Pos | FDCAN_DBTP_TDC | 0 << FDCAN_DBTP_DBRP_Pos;
        FDCAN3->TDCR = 9 << FDCAN_TDCR_TDCO_Pos | 5 << FDCAN_TDCR_TDCF_Pos; // normal delay 180 ns, glitch filter 150ns*170MHz = 25
        
        // // DTSEG1 + DTSEG2 + 3 = 170/5/2 -> 17
        // FDCAN3->DBTP = 8 << FDCAN_DBTP_DTSEG1_Pos | 6 << FDCAN_DBTP_DTSEG2_Pos | 4 << FDCAN_DBTP_DSJW_Pos | FDCAN_DBTP_TDC | 4 << FDCAN_DBTP_DBRP_Pos;
        // FDCAN3->TDCR = 12 << FDCAN_TDCR_TDCO_Pos | 1 << FDCAN_TDCR_TDCF_Pos;

        FDCAN3->CCCR |= FDCAN_CCCR_BRSE | FDCAN_CCCR_FDOE | FDCAN_CCCR_DAR; // bit rate switch, fd mode, disable automatic retransmission


        // NTSEG1 + NTSEG2 + 3 = 170
        FDCAN3->NBTP = 42 << FDCAN_NBTP_NSJW_Pos | 125 << FDCAN_NBTP_NTSEG1_Pos | 42 << FDCAN_NBTP_NTSEG2_Pos; // 10 time quanta, 3 time quanta before sample point
        // 2Mbps
        FDCAN3->NBTP = 20 << FDCAN_NBTP_NSJW_Pos | 62 << FDCAN_NBTP_NTSEG1_Pos | 20 << FDCAN_NBTP_NTSEG2_Pos; // 10 time quanta, 3 time quanta before sample point


        FDCAN3->TSCC = 1 << FDCAN_TSCC_TSS_Pos; // start counter
        //FDCAN3->TDCR ?
        FDCAN3->RXGFC = 3 << FDCAN_RXGFC_LSS_Pos | FDCAN_RXGFC_ANFS | FDCAN_RXGFC_ANFE | FDCAN_RXGFC_RRFS | FDCAN_RXGFC_RRFE; // 3 acceptance filters, reject everything else
        FDCAN3->TXBC |= FDCAN_TXBC_TFQM; // transmit fifo request queue mode


        FDCAN3->CCCR &= ~FDCAN_CCCR_INIT;
        
    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 50000;
    InitCode init_code;

    MotorEncoder motor_encoder;
    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;
};

#include "../../motorlib/boards/config_obot_g474_motor.cpp"

void config_init() {}

void config_maintenance() {}