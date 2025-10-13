import trace_blinker;
//#include <cstdint>
//import <cstdint>;

TraceBlinker trace_blinker;

int main() {
    while(1) {
        trace_blinker.blink();
    }
}

extern "C" {
void _close() {}
void _lseek() {}
void _read() {}
void _write() {}
void _kill() {}
void _getpid() {}
void _exit() {}
}