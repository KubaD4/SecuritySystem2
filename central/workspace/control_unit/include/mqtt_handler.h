#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include "states.h"

#define BUTTON_PORT GPIO_PORT_P5
#define BUTTON_PIN GPIO_PIN1

// !! ASSUMING THE FOLLOWING STRUCTURE OF THE MQTT MESSAGE: [identification(1bit) - roomID(2bits)]

#define PIN_IDENTIFICATION  GPIO_PIN7  // P4.7 - Authentication bit
#define PIN_ROOM_0        GPIO_PIN4    // P5.4 - Room bit 0
#define PIN_ROOM_1        GPIO_PIN5    // P5.5 - Room bit 1


#define ALL_SENSOR_PINS (PIN_IDENTIFICATION | PIN_ROOM_0 | PIN_ROOM_1 )

// Room names lookup table
static const char* ROOM_NAMES[] = {
    "Main Door",
    "Living Room",
};

// Structure to hold decoded sensor data
typedef struct {
    uint8_t identification;  // 1 bit - authentication status
    uint8_t room;           // 2 bits - room number
} SensorData;

// Function prototypes
void initSensorGPIO(void);
const char* getRoomName(uint8_t room_number);
void setTriggerInfo(uint8_t room);
SensorData readSensorData(void);

#endif
