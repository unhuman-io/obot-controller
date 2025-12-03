export module stm32g474_adc_init;

import obot_std;

export {

enum class ADCNumber { AD1, AD2, AD3, AD4, AD5 };

enum class ADCChannel {
  CH0,
  CH1,
  CH2,
  CH3,
  CH4,
  CH5,
  CH6,
  CH7,
  CH8,
  CH9,
  CH10,
  CH11,
  CH12,
  CH13,
  CH14,
  CH15,
  CH16,
  CH17,
  CH18
};

enum class ADCSampleTime {
  CYCLES_2_5,
  CYCLES_6_5,
  CYCLES_12_5,
  CYCLES_24_5,
  CYCLES_47_5,
  CYCLES_92_5,
  CYCLES_247_5,
  CYCLES_640_5
};

enum class DRlocation { DR, JDR1, JDR2, JDR3, JDR4 };

struct ADCChannelConfig {
  std::string_view name;
  std::string_view xxx;
  enum ADCNumber adc;
  enum ADCChannel channel;
  enum ADCSampleTime sample_time;
  enum DRlocation dr_location;
};



struct ADCRegConfig {
  uint32_t sqr;
  uint32_t jsqr;
  uint32_t smpr1;
  uint32_t smpr2;
};

struct ADCRegsInit {
  ADCRegConfig adc1, adc2, adc3, adc4, adc5;
};

consteval ADCRegsInit get_adc_regs_init(const ADCChannelConfig* configs, size_t config_count) {
  ADCRegsInit init{};

  for (size_t i = 0; i < config_count; ++i) {
    const auto& cfg = configs[i];
    ADCRegConfig& reg_cfg = [&]() -> ADCRegConfig& {
      switch (cfg.adc) {
        case ADCNumber::AD1:
          return init.adc1;
        case ADCNumber::AD2:
          return init.adc2;
        case ADCNumber::AD3:
          return init.adc3;
        case ADCNumber::AD4:
          return init.adc4;
        case ADCNumber::AD5:
          return init.adc5;
      }
      // should never reach here
      return init.adc1;
    }();
    int channel_index = static_cast<int>(cfg.channel);
    if (channel_index < 10) {
      reg_cfg.smpr1 |= static_cast<uint32_t>(cfg.sample_time) << (3 * channel_index);
    } else {
      reg_cfg.smpr2 |= static_cast<uint32_t>(cfg.sample_time) << (3 * (channel_index - 10));
    }

    if (cfg.dr_location == DRlocation::DR) {
      reg_cfg.sqr |= static_cast<uint32_t>(cfg.channel) << 6; // todo (5 * (i % 6));
    } else {
      //int seq_num = reg_cfg.jsqr & 
      int i = 0;
      reg_cfg.jsqr |= (static_cast<uint32_t>(cfg.channel) & 0x1F) << (5 * (i % 4));
    }
  }

  return init;
}
} // export

//constinit ADCRegsInit adc_regs_init = get_adc_regs_init(adc_channel_configs, sizeof(adc_channel_configs) / sizeof(ADCChannelConfig));
