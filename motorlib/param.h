
#ifndef PARAM_H
#define PARAM_H

#include "messages.h"

#ifdef __cplusplus
extern "C" {
#endif

Param *param();
void init_param_from_flash();
void save_param_to_flash();

#ifdef __cplusplus
}
#endif

#endif