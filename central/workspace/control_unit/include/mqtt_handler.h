#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include "states.h"

// Structure to hold decoded sensor data
typedef struct {
    uint8_t identification;  // 1 bit - authentication status
    uint8_t movement;        // 1 bit - movement detection
    uint8_t room;           // 3 bits - room number
} SensorData;

// Function prototypes
void initSensorGPIO(void);
void processSensorData(void);
const char* getRoomName(uint8_t room_number);
void setTriggerInfo(uint8_t room, uint8_t movement);

#endif
