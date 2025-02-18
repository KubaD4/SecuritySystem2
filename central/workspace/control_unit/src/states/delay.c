#include "../../include/states.h"

#ifndef TEST_BUILD

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/led.h"
#include "../../include/timer.h"

int prev_time;

void prepare_delay() {
    timer = 15;  // 15 second delay period
    password_correct = 0;
    environment = 0;
    prev_time = 15;

    writeLCDMessage("DELAY PERIOD:");
    writeLCDsubtitle("Exit the house");
    handleLEDGrace();

    Timer_A_UpModeConfig conf = get_countdown_config();
    Timer_A_configureUpMode(TIMER_A3_BASE, &conf);
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
    Interrupt_enableInterrupt(INT_TA3_N);
}

void handle_delay(void) {
    char message[32];  // Buffer to write in the screen

    if (prev_time != timer) {
        sprintf(message, "%d s", timer);
        clearLCDtime();
        writeLCDtime(message);
        prev_time = timer;
    }
}

void finish_delay() {
    timer = 0;
    password_correct = 0;
    environment = 0;

    Timer_A_stopTimer(TIMER_A3_BASE);  // Stop countdown timer
    Timer_A_stopTimer(TIMER_A1_BASE);  // Stop LED blinking timer
    Interrupt_disableInterrupt(INT_TA3_N);
}

#endif

State_t evaluate_delay() {
    if (timer <= 0) {
        return ARMED;
    } else if (password_correct) {
        return DISARMED;
    } else if (environment) {
        return TRIGGERED;
    }
    return DELAY;
}
