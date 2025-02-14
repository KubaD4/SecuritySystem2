#include "../../include/states.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/adc.h"
//#include "../include/sound.h"

void prepare_disarmed(){
    _alarmStop();
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
    menu_selection = 0;
    go_in_maintenance = 0;
    go_in_armed = 0;
    menu_done = 0;
    displayDisarmedMenu(menu_selection);
    handleLEDDisarmed();
    updateSelection(menu_selection);
    //disableSoundDetection();
    last_selection = 0;
}


void handle_disarmed(void) {

    if (!menu_done){
        if( go_in_maintenance + go_in_armed > 0){
           writeLCDMessage(" Enter Password ");
           writeLCDsubtitle("<- to go back");
           menu_done = 1;
        }else {
            if ( menu_selection != last_selection ) {
                updateSelection(menu_selection);
            }
            last_selection = menu_selection;
        }
    }

    if (back_to_menu) {
            finish_disarmed();
            prepare_disarmed();
            back_to_menu = 0;
    }

    if (light) {
        finish_disarmed();
        current_state = TRIGGERED;
        prepare_triggered();
    } else if ( password_correct && go_in_maintenance ) {
        finish_disarmed();
        current_state = MAINTENANCE;
        prepare_maintenance();
    } else if ( password_correct && go_in_armed ) {
        finish_disarmed();
        current_state = DELAY;
        prepare_delay();
        //current_state = ARMED;
        //prepare_armed();
    }



}


void finish_disarmed(){
    menu_selection = 0;
    menu_done = 0;
    buttonPreviouslyPressed = 0;
    password_correct = 0;
}
