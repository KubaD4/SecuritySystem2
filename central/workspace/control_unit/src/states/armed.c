#include "../../include/states.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/led.h"
#include "../../include/keypad.h"
//#include "../include/sound.h"

// Bind interrupt handlers for sensor signal
void prepare_armed(){
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
    light = 0;
    writeLCDMessage("System Armed");
    handleLEDArmed();
    keypad_clearBuffer();
}

void handle_armed(void) {
    if (keypad_authenticate(globalPassword) == KEYPAD_CORRECT) {
        password_correct = 1;
    }
}

State_t evaluate_armed() {
    if (opened_safe) {
        return GRACE;
    }
    if (opened_critical || light) {
        return TRIGGERED;
    }
    if (password_correct) {
        return DISARMED;
    }
    return ARMED;
}

void finish_armed(){
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
    light = 0;
}
