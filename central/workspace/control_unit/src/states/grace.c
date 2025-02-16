#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "../../include/states.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/led.h"
#include "../../include/timer.h"
#include "../../include/keypad.h"

void prepare_grace(){
    password_correct = 0;
    timer = 30;  // 30 second grace period
    light = 0;

    writeLCDMessage("GRACE PERIOD:");
    writeLCDsubtitle("Enter Password");
    handleLEDGrace();

    Timer_A_UpModeConfig conf = get_countdown_config();
    Timer_A_configureUpMode(TIMER_A3_BASE, &conf);
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
    Interrupt_enableInterrupt(INT_TA3_N);
    keypad_clearBuffer();
}

void handle_grace(void) {
    char message[32];  // Buffer to write in the screen

    sprintf(message, "%d s", timer);
    clearLCDtime();
    writeLCDtime(message);
    if (keypad_authenticate("1234") == KEYPAD_CORRECT) {
        password_correct = 1;
    }
}

State_t evaluate_grace() {
    if(password_correct) {
        return DISARMED;
    } else if(timer <= 0 || light) {
        return TRIGGERED;
    }
    return GRACE;
}

void finish_grace(){
    password_correct = 0;
    timer = 0;  // 30 second grace period
    light = 0;

    Timer_A_stopTimer(TIMER_A3_BASE);    // Stop countdown timer
    Timer_A_stopTimer(TIMER_A1_BASE);    // Stop LED blinking timer
    Interrupt_disableInterrupt(INT_TA3_N);
}


