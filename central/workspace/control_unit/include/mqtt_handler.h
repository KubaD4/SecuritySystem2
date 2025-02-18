#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include "states.h"

#define PIN_ROOM_1 GPIO_PIN5    // P5.5 - Room bit 1

// Room names lookup table
static const char* ROOM_NAMES[] = {
    "Box Tampered",
    "Sensor Triggered"
};

// Structure to hold decoded sensor data
typedef struct {
    int room;
} SensorData;

// Function prototypes
void initSensorGPIO(void);
const char* getRoomName(int room_number);
void setTriggerInfo(int room);
SensorData readSensorData(void);

#endif
