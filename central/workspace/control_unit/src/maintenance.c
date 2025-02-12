#include "../include/states.h"
#include "../include/alarm.h"
#include "../include/grap.h"


void prepare_maintenance(){
    _alarmStop();
    writeLCDMessage("System Maintenance");
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
    go_in_maintenance = 0;
 //   ADC14_disableInterrupt(ADC_INT0);  // Disable joystick ADC interrupt
}

void handle_maintenance(){
    if(password_correct) {
            finish_triggered();
            finish_maintenance();
            //state_code = ALARM_STATE_DISARMED;
            current_state = DISARMED;
            prepare_disarmed();
     }
}

void finish_maintenance(){
 //   ADC14_enableInterrupt(ADC_INT0);
    _alarmStop();
    opened_safe = 0;
    opened_critical = 0;
    go_in_maintenance = 0;
    go_in_armed = 0;
    password_correct = 0;
    flag = 0;
}
