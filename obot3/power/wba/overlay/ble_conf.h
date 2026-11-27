// As ST's, with room for one handler of the events that are not vendor
// events: the advertising reports go to ours (port.c) through it.
#ifndef POWER_BLE_CONF_H
#define POWER_BLE_CONF_H
#include_next "ble_conf.h"
#undef BLE_CFG_MAX_NBR_CB
#define BLE_CFG_MAX_NBR_CB (1)
#endif
