import trace_blinker;
//#include <cstdint>
//import <cstdint>;

TraceBlinker trace_blinker;

int main() {
    trace_blinker.run();
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