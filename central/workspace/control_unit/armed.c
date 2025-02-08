#include "states.h"
#include "alarm.h"
#include "grap.h"


// Bind interrupt handlers for sensor signal
void prepare_armed(){
    opened_safe = 0;
    opened_critical = 0;
    writeLCDMessage("System Armed");
}

void handle_armed(void) {
    handleLEDArmed();
    if(opened_safe) {
        finish_armed();
        state_code = ALARM_STATE_GRACE;
        prepare_grace();
    }
    else if(opened_critical) {
        finish_armed();
        state_code = ALARM_STATE_TRIGGERED;
        prepare_triggered();
    }
}

void finish_armed(){

}
