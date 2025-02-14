#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>
#include "../../include/states.h"

// Add timer interrupt handler
void TA3_N_IRQHandler(void) {
    Timer_A_clearInterruptFlag(TIMER_A3_BASE);
    if( timer > 0 ) {
        timer--;
    }
}
