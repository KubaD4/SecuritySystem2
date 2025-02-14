#include "../../include/states.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/led.h"
//#include "../include/sound.h"


// Bind interrupt handlers for sensor signal
void prepare_armed(){
    _alarmStop();
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
    writeLCDMessage("System Armed");
    handleLEDArmed();
    //enableSoundDetection();
}

void handle_armed(void) {
    if(opened_safe) {
        finish_armed();
        current_state = GRACE;
        prepare_grace();
    }
    else if(opened_critical) {
        finish_armed();
        current_state = TRIGGERED;
        prepare_triggered();

    } else if(password_correct){
        password_correct = 0;
        finish_armed();
        current_state = DISARMED;
        prepare_disarmed();
    }
}

void finish_armed(){
    password_correct = 0;
}
