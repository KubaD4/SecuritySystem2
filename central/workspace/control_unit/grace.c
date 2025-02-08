#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "states.h"
#include "alarm.h"
#include "grap.h"
#include "led.h"

void prepare_grace(){
    //reset password field
    grace_timer = 30;  // 30 second grace period
    writeLCDMessage("GRACE PERIOD: Enter Password");
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}

void handle_grace(void) {
    char message[32];  // Buffer to write in the screen

    handleLEDGrace();

    // Update display with current time
    sprintf(message, "GRACE PERIOD: %ds remaining", grace_timer);
    writeLCDMessage(message);


    if(password_correct) {
        finish_grace();
        state_code = ALARM_STATE_DISARMED;
        prepare_disarmed();
    }
    else if(grace_timer <= 0) {
        finish_grace();
        state_code = ALARM_STATE_TRIGGERED;
        prepare_triggered();
    }
}

void finish_grace(){
    Timer_A_stopTimer(TIMER_A1_BASE);
}
