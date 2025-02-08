#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <stdint.h>
#include "states.h"

// Structure to hold decoded sensor data
typedef struct {
    uint8_t identification;  // 1 bit - authentication status
    uint8_t movement;        // 1 bit - movement detection
    uint8_t room;           // 3 bits - room number
} SensorData;

// Function prototypes
void handleMQTTMessage(const uint8_t* payload, uint16_t len);
SensorData decodeSensorData(uint8_t data);
void updateSystemState(const SensorData* sensor_data);
const char* getRoomName(uint8_t room_number);
void setTriggerInfo(uint8_t room, uint8_t movement);

#endif
