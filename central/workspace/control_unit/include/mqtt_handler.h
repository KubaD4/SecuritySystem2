#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include "states.h"

#define BUTTON_PORT GPIO_PORT_P5
#define BUTTON_PIN GPIO_PIN1

// !! ASSUMING THE FOLLOWING STRUCTURE OF THE MQTT MESSAGE: [identification(1bit) - roomID(2bits)]

#define PIN_ROOM_0        GPIO_PIN5    // P2.5 - Room bit 0
#define PIN_ROOM_1        GPIO_PIN5    // P5.5 - Room bit 1


#define ALL_SENSOR_PINS (PIN_IDENTIFICATION | PIN_ROOM_0 | PIN_ROOM_1 )

// Room names lookup table
static const char* ROOM_NAMES[] = {
    "Box (Lux)",
    "Living Room",
    "Main Door",
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
