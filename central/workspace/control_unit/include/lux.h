#ifndef __LUX_H__
#define __LUX_H__

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>
#include "HAL_I2C.h"
#include "HAL_OPT3001.h"

float lux;

void _lightSensorInit();

#endif
