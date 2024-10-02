#include "st_device.h"
#include <fcntl.h>
#include <cstring>
#include "../../motorlib/logger.h"
__attribute__ ((init_priority(LOGGER_INIT_PRIORITY))) Logger logger;

extern "C" {
void board_init() {}
void system_loop_interrupt() {}
void fast_loop_interrupt() {}
void main_loop_interrupt() {}
void usb_interrupt() {}
void system_init() {}
void system_run() {}
void _write(int fd, const char *buf, size_t count) {
    logger.log(std::string_view(buf, count));
}
int _fstat (int fd, struct stat * st) {
  memset (st, 0, sizeof (* st));
  st->st_mode = S_IFCHR;
  setbuf(stdout, NULL); // Disable buffering for stdout
  setbuf(stderr, NULL);
  return (0);
}
int _isatty (int fd) {
  return (1);
}

} // extern "C"

void config_init() {}

void config_maintenance() {}

