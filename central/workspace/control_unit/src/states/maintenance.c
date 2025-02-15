#include "../../include/states.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"


void prepare_maintenance(){
    _alarmStop();
    writeLCDMessage("System Maintenance");
    writeLCDsubtitle("<- to go back");
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
    go_in_maintenance = 0;
    back_to_menu = 0;
}

void handle_maintenance(){
}

void finish_maintenance(){
    _alarmStop();
    opened_safe = 0;
    opened_critical = 0;
    go_in_maintenance = 0;
    go_in_armed = 0;
    password_correct = 0;
    flag = 0;
    back_to_menu = 0;
}

State_t evaluate_maintenance(){
    if(back_to_menu) {
        return DISARMED;
    }
    return MAINTENANCE;
}
