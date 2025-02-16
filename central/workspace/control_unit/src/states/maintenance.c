#include "../../include/states.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/keypad.h"


void prepare_maintenance(){
    back_to_menu = 0;

    writeLCDMessage("System Maintenance");
    writeLCDsubtitle("Press to go back");
}

void handle_maintenance(){
}

void finish_maintenance(){
    back_to_menu = 0;
}

State_t evaluate_maintenance(){
    if(back_to_menu) {
        return DISARMED;
    }
    return MAINTENANCE;
}
