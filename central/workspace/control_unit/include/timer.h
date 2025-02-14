#ifndef TIMER_H
#define TIMER_H

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>

//Grace constants
extern volatile int grace_timer;

static const Timer_A_UpModeConfig countdownConfig;

void TA3_0_IRQHandler(void);

#endif
