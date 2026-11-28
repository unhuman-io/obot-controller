// The configuration of ST's BLE_BeaconHCI example (STM32CubeWBA), which the
// radio firmware of the power board and of the remote is built on, with what
// is different here. ST's own file is included unchanged from the package
// (#include_next) and the differences follow it.
#ifndef POWER_APP_CONF_H
#define POWER_APP_CONF_H
#include_next "app_conf.h"

// ST's application (app_ble.c) starts one of its beacons at the end of its
// start-up; asked for the iBeacon it calls IBeacon_Process(), and that is
// ours (port.c): it starts our application instead.
#undef CFG_BEACON_TYPE
#define CFG_BEACON_TYPE (CFG_IBEACON_TYPE)

// The STM32WBA5MMG module has no external power amplifier or SMPS variant to
// choose: table 0, as ST's projects for its module board (B-WBA5M-WPAN).
#undef CFG_RF_TX_POWER_TABLE_ID
#define CFG_RF_TX_POWER_TABLE_ID (0)

// Sleep between radio events and timers in Stop 1: the pins keep their
// levels (the bus kill line, the converter enables, the display's lines) and
// every peripheral its settings. Standby would lose both.
#undef CFG_LPM_STANDBY_SUPPORTED
#define CFG_LPM_STANDBY_SUPPORTED (0U)
#undef CFG_LPM_STDBY_SUPPORTED
#define CFG_LPM_STDBY_SUPPORTED (0U)

#endif
