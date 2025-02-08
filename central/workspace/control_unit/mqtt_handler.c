#include "mqtt_handler.h"
#include "alarm.h"
#include "grap.h"
#include <stdio.h>

// !! ASSUMING THE FOLLOWING STRUCTURE OF THE MQTT MESSAGE: [identification(1bit) - movement(1bit) - roomID(3bits)]

// Room names lookup table
static const char* ROOM_NAMES[] = {
    "Unknown",
    "Main Door",
    "Living Room",
    "Kitchen",
    "Bedroom 1",
    "Bedroom 2",
    "Bathroom",
    "Garage"
};

// Decode 5-bit sensor data into structured format
SensorData decodeSensorData(uint8_t data) {
    SensorData decoded;
    decoded.identification = (data >> 4) & 0x01;  // Most significant bit
    decoded.movement = (data >> 3) & 0x01;        // Second bit
    decoded.room = data & 0x07;                   // Last 3 bits
    return decoded;
}

// Get room name from room number
const char* getRoomName(uint8_t room_number) {
    if (room_number < sizeof(ROOM_NAMES)/sizeof(ROOM_NAMES[0])) {
        return ROOM_NAMES[room_number];
    }
    return ROOM_NAMES[0]; // Return "Unknown" for invalid room numbers
}

// Update system state based on sensor data
void updateSystemState(const SensorData* sensor_data) {
    // Handle authentication
    if (sensor_data->identification) {
        password_correct = 1;
        return;
    }

    // Process sensor triggers if system is armed
    if (state_code != ALARM_STATE_ARMED) {
        return;
    }

    // Store trigger information
    setTriggerInfo(sensor_data->room, sensor_data->movement);

    // Handle main door (room 1) specially
    if (sensor_data->room == 1 && !sensor_data->movement) {
        opened_safe = 1;
        return;
    }

    // Handle movement sensors or other entries
    if (sensor_data->movement || sensor_data->room > 1) {
        opened_critical = 1;
        return;
    }
}

// Main MQTT message handler
void handleMQTTMessage(const uint8_t* payload, uint16_t len) {
    if (len != 1) {
        return; // Invalid message length
    }

    // Decode the 5-bit sensor data
    SensorData sensor_data = decodeSensorData(payload[0]);

    // Update system state based on decoded data
    updateSystemState(&sensor_data);
}
