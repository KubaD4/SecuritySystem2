#include "../../include/states.h"

#ifndef TEST_BUILD

#include <stdio.h>
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/led.h"
#include "../../include/keypad.h"
#include "../../include/mqtt_handler.h"  // Added for room name functions

// Added globals to store trigger information
static uint8_t triggered_room = 0;

void prepare_triggered() {
    password_correct = 0;

    char message[64];
    const char* room_name = getRoomName(triggered_room);
    snprintf(
        message,
        sizeof(message),
        "Alarm in %s",
        room_name
    );
    writeLCDMessage(message);

    _alarmInit();
    keypad_clearBuffer();
}

void handle_triggered(void) {
    handleLEDTriggered();
    if (keypad_authenticate(globalPassword) == KEYPAD_CORRECT) {
        password_correct = 1;
    }
}

void finish_triggered() {
    _alarmStop();
    password_correct = 0;
}

void setTriggerInfo(uint8_t room) {
    triggered_room = room;
}

#endif

State_t evaluate_triggered(){
    if(password_correct) {
        return DISARMED;
    }
    return TRIGGERED;
}
