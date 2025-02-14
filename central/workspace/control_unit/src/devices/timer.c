#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>
#include "../../include/states.h"


static const Timer_A_UpModeConfig countdownConfig = {
    TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK (48MHz)
    TIMER_A_CLOCKSOURCE_DIVIDER_64,     // Divide by 256
    750000,                              // (48MHz/256 = 187.5kHz), count to 187500 for 1 second
    TIMER_A_TAIE_INTERRUPT_ENABLE,
    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
    TIMER_A_DO_CLEAR
};

// Add timer interrupt handler
void TA3_N_IRQHandler(void) {
    Timer_A_clearInterruptFlag(TIMER_A3_BASE);

    if(timer > 0) {
        timer--;
    }
}
