#include "../../include/states.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/mqtt_handler.h"  // Added for room name functions

// Added globals to store trigger information
static uint8_t triggered_room = 0;
static uint8_t triggered_movement = 0;

void prepare_triggered() {
    _alarmInit();

    char message[64];
    const char* room_name = getRoomName(triggered_room);

    snprintf(message, sizeof(message),
             "Alarm in %s",
             room_name);

    writeLCDMessage(message);
    password_correct = 0;
}

void handle_triggered(void) {
    handleLEDTriggered();
}

void finish_triggered() {
    _alarmStop();
    opened_safe = 0;
    opened_critical = 0;
    triggered_room = 0;
    triggered_movement = 0;
    flag = 0;
    go_in_maintenance = 0;
}

void setTriggerInfo(uint8_t room) {
    triggered_room = room;
}

State_t evaluate_triggered(){
    if(password_correct) {
        finish_triggered();
        current_state = DISARMED;
        prepare_disarmed();
    }
    return TRIGGERED;
}
