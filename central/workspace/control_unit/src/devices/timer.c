#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>
#include "../../include/states.h"

int count = 0;
int over = 0;
// Add timer interrupt handler
void TA3_N_IRQHandler(void) {
    Timer_A_clearInterruptFlag(TIMER_A3_BASE);
    if( count < 4 ) {
        count++;
    }else{
        count = 0;
        over = 1;
    }

    if(timer > 0 && over == 1){
        timer--;
        over = 0;
    }
}

//==========================Timer Bilal===================================//

// Configurazione del timer: usiamo ad es. TIMER_A2
static Timer_A_UpModeConfig keypadTimerConfig =
{
    TIMER_A_CLOCKSOURCE_SMCLK,
    TIMER_A_CLOCKSOURCE_DIVIDER_64,
    46875, // Imposta il periodo: ad esempio 46875 cicli per ottenere circa 100ms (verifica in base al tuo clock)
    TIMER_A_TAIE_INTERRUPT_ENABLE,
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
    TIMER_A_DO_CLEAR
};

void keypadTimer_init(void) {
    // Configura il timer in modalità Up
    Timer_A_configureUpMode(TIMER_A2_BASE, &keypadTimerConfig);
    // Abilita l'interrupt per il timer
    Interrupt_enableInterrupt(INT_TA2_0);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
}

// Interrupt handler per il Timer_A2 CCR0
void TA2_0_IRQHandler(void) {
    // Pulisci il flag di interrupt
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    // Chiama la funzione di polling del tastierino
    keypad_poll();
}
