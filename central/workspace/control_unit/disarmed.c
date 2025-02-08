#include "states.h"
#include "alarm.h"
#include "grap.h"

void prepare_disarmed(){
    writeLCDMessage("System Disarmed");
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
}


void handle_disarmed(void) {
    handleLEDDisarmed();
    if(password_correct) {
        finish_disarmed();
        state_code = ALARM_STATE_ARMED;
        prepare_armed();
    }
}


void finish_disarmed(){

}
