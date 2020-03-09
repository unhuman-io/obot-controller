void system_init() {
    SystemConfig::init();
}

void system_run() {
    SystemConfig::run();
}

void main_loop_interrupt() {
    SystemConfig::main_loop_interrupt();
}

void fast_loop_interrupt() {
    SystemConfig::fast_loop_interrupt();
}

void usb_interrupt() {
    SystemConfig::usb_interrupt();
}
