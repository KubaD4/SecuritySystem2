#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "../include/states.h"
#include "../include/alarm.h"
#include "../include/grap.h"
#include "../include/led.h"

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

    if(grace_timer > 0) {
        grace_timer--;
    }
}

void prepare_grace(){
    //reset password field
    password_correct = 0;

    grace_timer = 30;  // 30 second grace period

    writeLCDMessage("GRACE PERIOD:");
    writeLCDsubtitle("Enter Password");
    handleLEDGrace();

    Timer_A_configureUpMode(TIMER_A3_BASE, &countdownConfig);
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
    Interrupt_enableInterrupt(INT_TA3_N);
}

void finish_grace(){
    Timer_A_stopTimer(TIMER_A3_BASE);    // Stop countdown timer
    Timer_A_stopTimer(TIMER_A1_BASE);    // Stop LED blinking timer
    Interrupt_disableInterrupt(INT_TA3_N);
    grace_timer = 0;
    password_correct = 0;
    go_in_armed = 0;
}

void handle_grace(void) {
    char message[32];  // Buffer to write in the screen

    sprintf(message, "%d s", grace_timer);
    clearLCDtime();
    writeLCDtime(message);


    if(password_correct) {
        finish_grace();
        current_state = DISARMED;
        prepare_disarmed();
    } else if(grace_timer <= 0) {
        if(go_in_armed) {
            finish_grace();
            current_state = ARMED;
            prepare_armed();
        } else {
            finish_grace();
            current_state = TRIGGERED;
            prepare_triggered();
        }
    }
}


