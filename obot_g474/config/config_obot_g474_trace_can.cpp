#include "../param/param_obot_g474_can.h"
#include "st_device.h"
#include "../../motorlib/encoder.h"
#include "../../motorlib/torque_sensor.h"
#include <coroutine>
#include "task.h"
#include "../../motorlib/peripheral/stm32g4/spi_dma.h"

#define COMMS   COMMS_CAN

using TorqueSensor = TorqueSensorBase;
using MotorEncoder = EncoderBase;
using OutputEncoder = EncoderBase;

struct InitCode {
    InitCode() {
        DMAMUX1_Channel0->CCR =  DMA_REQUEST_SPI1_TX;
        DMAMUX1_Channel1->CCR =  DMA_REQUEST_SPI1_RX;

    }
};

namespace config {
    const uint32_t main_loop_frequency = 10000;    
    const uint32_t pwm_frequency = 30000;
    InitCode init_code;

    TorqueSensor torque_sensor;
    OutputEncoder output_encoder;
    MotorEncoder motor_encoder;
};

#include "../../motorlib/boards/config_obot_g474_trace.cpp"

Task<> init_sensor(CycleScheduler& sched) {
    co_await sched.async_delay_us(500);

    GPIO cs1 {*GPIOA, 4, GPIO::OUTPUT};
    SPIDMA spi1 {SPIDMA::SP1, cs1, DMA1_CH1, DMA1_CH2, 0, 50, 50, SPI_CR1_MSTR | (7 << SPI_CR1_BR_Pos) | SPI_CR1_SSI | SPI_CR1_SSM};
    uint8_t data_out[16], data_in[16];
    logger.log("sensor mid1");
    bool success = co_await spi1.readwrite_async(sched, data_out, data_in, 16);
    logger.log_printf("sensor mid2, %d", success);
    co_await sched.async_delay_us(500);
    logger.log("sensor done");
}

Task<> init_adc(CycleScheduler& sched) {
    co_await sched.async_delay_us(1200); 
    logger.log("adc done");
}

void config_init() {
    CycleScheduler init_scheduler;

    auto sensor_task = init_sensor(init_scheduler);
    auto adc_task = init_adc(init_scheduler);

    logger.log("init start");
    while (!sensor_task.is_done() || !adc_task.is_done()) {
        init_scheduler.poll();
    }
    logger.log("init done");
}

void config_maintenance() {}

