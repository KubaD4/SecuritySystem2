#include "../../include/LED.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "../../include/states.h"
#include <ti/devices/msp432p4xx/inc/msp.h>


int countL = 0;
int overL = 0;

/* Timer config for LED blinking */
static const Timer_A_UpModeConfig upConfigLED = {
//    TIMER_A_CLOCKSOURCE_SMCLK,
//    TIMER_A_CLOCKSOURCE_DIVIDER_12,
//    30000,                               // LED blink period
//    TIMER_A_TAIE_INTERRUPT_ENABLE,
//    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
//    TIMER_A_DO_CLEAR

     TIMER_A_CLOCKSOURCE_SMCLK,
     TIMER_A_CLOCKSOURCE_DIVIDER_64,
     4500000,
     TIMER_A_TAIE_INTERRUPT_ENABLE,
     TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
     TIMER_A_DO_CLEAR
};

void setupLEDs(void) {
    // Setup LED pins
    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN6);  // Blu LED
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN6);  // Red LED
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN4);  // Green LED
}

void handleLEDDisarmed(void) {
    stopLEDBlinking();  // Stop any blinking
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);   // Red off
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);  // Blu off
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);  // Green on
}

void handleLEDArmed(void) {
    stopLEDBlinking();  // Stop any blinking
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN6);  // Rosso on
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);   // Blu off
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);   // Green off
}

void handleLEDTriggered(void) {
    //stopLEDBlinking();  // prevent multiple timer starts
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);  // Rosso off
    GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN6);   // Blu off
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN4);   // Green off
    // Start timer for red LED blinking
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfigLED);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    Interrupt_enableInterrupt(INT_TA1_N);
}

// Interrupt handler for LED blinking
void TA1_N_IRQHandler(void) {
    Timer_A_clearInterruptFlag(TIMER_A1_BASE);
    if(current_state == TRIGGERED ) {
        if( countL < 4 ) {
                countL++;
        }else{
            countL = 0;
            overL = 1;
        }

        if(overL == 1){
            GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN6);  // Toggle only red;
            overL = 0;
        }
    }
//    else if( current_state == GRACE) {
//        GPIO_toggleOutputOnPin(GPIO_PORT_P5, GPIO_PIN6);  // Toggle both
//        GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN6);
//    }
}

void handleLEDGrace(void) {
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN6);   // Red off
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN6);  // Blu off
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN4);  // Green on
    // Start timer for both LEDs blinking
    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfigLED);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    Interrupt_enableInterrupt(INT_TA1_N);
}

void stopLEDBlinking(void) {
    Timer_A_stopTimer(TIMER_A1_BASE);
    Interrupt_disableInterrupt(INT_TA1_N);
}
