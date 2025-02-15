#include "../../include/states.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/adc.h"
#include "../../include/led.h"

void prepare_disarmed(){
    password_correct = 0;
    go_in_maintenance = 0;
    go_in_armed = 0;
    light = 0;

    menu_selection = 0;
    menu_done = 0;
    last_selection = 0;

    displayDisarmedMenu(menu_selection);
    handleLEDDisarmed();
    updateSelection(menu_selection);
}


void handle_disarmed(void) {
    if ( go_in_maintenance + go_in_armed <= 0 ){
        if ( menu_done == 0 ){
            displayDisarmedMenu(menu_selection);
            updateSelection(menu_selection);
            menu_done = 1;
        }else{
            if ( menu_selection != last_selection ) {
                updateSelection(menu_selection);
            }
            last_selection = menu_selection;
        }
    }else{
        if ( menu_done == 0 ){
            writeLCDMessage(" Enter Password ");
            writeLCDsubtitle("<- to go back");
            menu_done = 1;
        }
    }
}

State_t evaluate_disarmed() {
    if ( light ) {
        printf("AAAAAAAAAAAAAAA\n");
        return TRIGGERED;
    } else if ( password_correct && go_in_maintenance ) {
        return MAINTENANCE;
    } else if ( password_correct && go_in_armed ) {
        return DELAY;
    }
    return DISARMED;
}


void finish_disarmed(){
    password_correct = 0;
    go_in_maintenance = 0;
    go_in_armed = 0;
    light = 0;

    menu_selection = 0;
    menu_done = 0;
    last_selection = 0;
}
