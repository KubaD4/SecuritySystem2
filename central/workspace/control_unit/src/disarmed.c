#include "../include/states.h"
#include "../include/alarm.h"
#include "../include/grap.h"
//#include "../include/sound.h"

void prepare_disarmed(){
    _alarmStop();
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
    menu_selection = 0;
    go_in_maintenance = 0;
    go_in_armed = 0;
    displayDisarmedMenu(menu_selection);
    handleLEDDisarmed();
    //disableSoundDetection();
}


void handle_disarmed(void) {
    static int menu_done = 0;
    static int last_selection = 0;

    if ( flag ) {
        finish_disarmed();
        current_state = TRIGGERED;
        prepare_triggered();
    } else if ( password_correct && go_in_maintenance ) {
        finish_disarmed();
        current_state = MAINTENANCE;
        prepare_maintenance();
    } else if ( password_correct && go_in_armed ) {
        finish_disarmed();
        current_state = ARMED;
        //state_code = ALARM_STATE_ARMED;
        prepare_armed();
    }

    if( go_in_maintenance + go_in_armed > 0 ){
        menu_done = 1;
        writeLCDMessage(" Enter Password ");
        writeLCDsubtitle(" to enter selected state ");
    }else{
        if ( menu_selection != last_selection ) {
            displayDisarmedMenu(menu_selection);
        }
    }
}


void finish_disarmed(){
    menu_selection = 0;
}
