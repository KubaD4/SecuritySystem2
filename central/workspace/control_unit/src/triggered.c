#include "../include/states.h"
#include "../include/alarm.h"
#include "../include/grap.h"
#include "../include/mqtt_handler.h"  // Added for room name functions

// Added globals to store trigger information
static uint8_t triggered_room = 0;
static uint8_t triggered_movement = 0;

void prepare_triggered() {
    _alarmInit();

    char message[64];
    const char* room_name = getRoomName(triggered_room);
    const char* sensor_type = triggered_movement ? "movement" : "door";

    snprintf(message, sizeof(message),
             "Alarm in %s\nType: %s",
             room_name,
             sensor_type);

    writeLCDMessage(message);
    password_correct = 0;
}

void handle_triggered(void) {
    handleLEDTriggered();
    if(password_correct) {
        finish_triggered();
        state_code = ALARM_STATE_DISARMED;
        prepare_disarmed();
    }
}

void finish_triggered() {
    _alarmStop();
    opened_safe = 0;
    opened_critical = 0;
    triggered_room = 0;
    triggered_movement = 0;
}

void setTriggerInfo(uint8_t room, uint8_t movement) {
    triggered_room = room;
    triggered_movement = movement;
}
