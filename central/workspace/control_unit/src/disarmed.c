#include "../include/states.h"
#include "../include/alarm.h"
#include "../include/grap.h"
//#include "../include/sound.h"

void prepare_disarmed(){
    _alarmStop();
    writeLCDMessage("DISARMED menu: up/down");
    writeLCDsubtitle("ARMED/MAINTENANCE");
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
    menu_selection = 0;
    handleLEDDisarmed();
    //disableSoundDetection();
}


void handle_disarmed(void) {
    if(password_correct) {
        finish_disarmed();
        current_state = ARMED;
        //state_code = ALARM_STATE_ARMED;
        prepare_armed();
    } else if (flag) {
        finish_disarmed();
        current_state = TRIGGERED;
        prepare_triggered();
    } else if (go_in_maintenance){
        finish_disarmed();
        current_state = MAINTENANCE;
        prepare_maintenance();
    }
}


void finish_disarmed(){
    menu_selection = 0;
}
