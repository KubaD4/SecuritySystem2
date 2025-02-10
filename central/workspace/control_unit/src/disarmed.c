#include "../include/states.h"
#include "../include/alarm.h"
#include "../include/grap.h"

void prepare_disarmed(){
    _alarmStop();
    writeLCDMessage("System Disarmed");
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
    handleLEDDisarmed();
}


void handle_disarmed(void) {
    if(password_correct) {
        finish_disarmed();
        state_code = ALARM_STATE_ARMED;
        prepare_armed();
    }
}


void finish_disarmed(){

}
