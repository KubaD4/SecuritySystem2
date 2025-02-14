#ifndef TIMER_H
#define TIMER_H

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>

//Grace constants
extern volatile int grace_timer;

static inline Timer_A_UpModeConfig get_countdown_config(void) {
  return (Timer_A_UpModeConfig){
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_64,
    4500000,
    TIMER_A_TAIE_INTERRUPT_ENABLE,
    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
    TIMER_A_DO_CLEAR
  };
}

void TA3_0_IRQHandler(void);

#endif
