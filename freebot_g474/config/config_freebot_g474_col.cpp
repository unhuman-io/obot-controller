Communication System::communication_ = {config::usb};
void usb_interrupt() {
    config::usb.interrupt();
}
Actuator System::actuator_ = {config::fast_loop, config::main_loop, param->startup_param};

float get_va() {
    ADC1->CR |= ADC_CR_JADSTART;
    ns_delay(1000);
    return V_A_DR * 3.0/4096*(18+2)/2.0;
}
float get_vb() {
    ADC1->CR |= ADC_CR_JADSTART;
    ns_delay(1000);
    return V_A_DR * 3.0/4096*(18+2)/2.0;
}
float get_vc() {
    ADC1->CR |= ADC_CR_JADSTART;
    ns_delay(1000);
    return V_A_DR * 3.0/4096*(18+2)/2.0;
}
void set_v(float f) {}

int32_t index_mod = 0;

void system_init() {
    if (config::motor_encoder.init()) {
        System::log("Motor encoder init success");
    } else {
        System::log("Motor encoder init failure");
    }
    if (config::output_encoder.init()) {
        System::log("Output encoder init success");
    } else {
        System::log("Output encoder init failure");
    }
    config::torque_sensor.init();

    System::api.add_api_variable("index_mod", new APIInt32(&index_mod));
    System::api.add_api_variable("vam", new APICallbackFloat(get_va, set_v));
    System::api.add_api_variable("vbm", new APICallbackFloat(get_vb, set_v));
    System::api.add_api_variable("vcm", new APICallbackFloat(get_vc, set_v));


    for (auto regs : std::vector<ADC_TypeDef*>{ADC1, ADC3, ADC4, ADC5}) {
        regs->CR = ADC_CR_ADVREGEN;
        ns_delay(20000);
        regs->CR |= ADC_CR_ADCAL;
        while(regs->CR & ADC_CR_ADCAL);
        ns_delay(100);

        regs->ISR = ADC_ISR_ADRDY;
        regs->CR |= ADC_CR_ADEN;
    }
    while(!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->CR |= ADC_CR_JADSTART;
    while(ADC1->CR & ADC_CR_JADSTART);


    config_init();

    ADC1->CR |= ADC_CR_ADSTART;
    ADC5->CR |= ADC_CR_JADSTART;
    ADC5->IER |= ADC_IER_JEOCIE;
    ADC4->CR |= ADC_CR_JADSTART;
    ADC3->CR |= ADC_CR_JADSTART;
   
    TIM1->CR1 = TIM_CR1_CEN; // start main loop interrupt
    config::usb.connect();
    HRTIM1->sMasterRegs.MCR = HRTIM_MCR_TDCEN + HRTIM_MCR_TECEN + HRTIM_MCR_TFCEN; // start high res timer
}

FrequencyLimiter temp_rate = {8};

void system_maintenance() {
    if (temp_rate.run()) {
        config::temp_sensor.read();
    }
    config_maintenance();
}

#include "../../motorlib/system.cpp"
