#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "../../include/states.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/led.h"
#include "../../include/timer.h"

void prepare_grace(){
    //reset password field
    password_correct = 0;

    timer = 30;  // 30 second grace period

    writeLCDMessage("GRACE PERIOD:");
    writeLCDsubtitle("Enter Password");
    handleLEDGrace();

    Timer_A_UpModeConfig conf = get_countdown_config();

    Timer_A_configureUpMode(TIMER_A3_BASE, &conf);
    Timer_A_startCounter(TIMER_A3_BASE, TIMER_A_UP_MODE);
    Interrupt_enableInterrupt(INT_TA3_N);
}

void finish_grace(){
    Timer_A_stopTimer(TIMER_A3_BASE);    // Stop countdown timer
    Timer_A_stopTimer(TIMER_A1_BASE);    // Stop LED blinking timer
    Interrupt_disableInterrupt(INT_TA3_N);
    timer = 0;
    password_correct = 0;
    go_in_armed = 0;
}

void handle_grace(void) {
    char message[32];  // Buffer to write in the screen

    sprintf(message, "%d s", timer);
    clearLCDtime();
    writeLCDtime(message);


    if(password_correct) {
        finish_grace();
        current_state = DISARMED;
        prepare_disarmed();
    } else if(timer <= 0 || light) {
        finish_grace();
        current_state = TRIGGERED;
        prepare_triggered();
    }
}


