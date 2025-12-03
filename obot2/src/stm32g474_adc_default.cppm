export module stm32g474_adc_default;

import stm32g474_adc_init;

using enum ADCNumber;
using enum ADCChannel;
using enum ADCSampleTime;
using enum DRlocation;

constexpr ADCChannelConfig adc_channel_configs[] = {
    {"adc1_ch0", "adc1_ch0", AD1, CH0, CYCLES_2_5, DR},
};