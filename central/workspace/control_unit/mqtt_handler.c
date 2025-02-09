#include "mqtt_handler.h"
#include "alarm.h"
#include "grap.h"
#include <stdio.h>

// !! ASSUMING THE FOLLOWING STRUCTURE OF THE MQTT MESSAGE: [identification(1bit) - movement(1bit) - roomID(3bits)]

// Pin definitions on Port 4
#define PIN_IDENTIFICATION  GPIO_PIN0  // P4.0 - Authentication bit
#define PIN_MOVEMENT       GPIO_PIN1  // P4.1 - Movement bit
#define PIN_ROOM_0        GPIO_PIN2  // P4.2 - Room bit 0 (LSB)
#define PIN_ROOM_1        GPIO_PIN3  // P4.3 - Room bit 1
#define PIN_ROOM_2        GPIO_PIN4  // P4.4 - Room bit 2 (MSB)

#define ALL_SENSOR_PINS (PIN_IDENTIFICATION | PIN_MOVEMENT | PIN_ROOM_0 | PIN_ROOM_1 | PIN_ROOM_2)

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

// Get room name from room number
const char* getRoomName(uint8_t room_number) {
    if (room_number < sizeof(ROOM_NAMES)/sizeof(ROOM_NAMES[0])) {
        return ROOM_NAMES[room_number];
    }
    return ROOM_NAMES[0]; // Return "Unknown" for invalid room numbers
}

// Initialize GPIO pins for sensor input
void initSensorGPIO(void) {
    // Configure pins as input with pull-down resistors
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, ALL_SENSOR_PINS);

    // Configure interrupt on rising edge (when ESP sets pins high)
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, ALL_SENSOR_PINS, GPIO_HIGH_TO_LOW_TRANSITION);

    // Clear any previous interrupts
    GPIO_clearInterruptFlag(GPIO_PORT_P4, ALL_SENSOR_PINS);

    // Enable interrupts for all sensor pins
    GPIO_enableInterrupt(GPIO_PORT_P4, ALL_SENSOR_PINS);

    // Enable Port 4 interrupt in NVIC
    Interrupt_enableInterrupt(INT_PORT4);
}

// Read and decode sensor data from GPIO pins
static SensorData readSensorData(void) {
    SensorData data;
    uint8_t pinStates = GPIO_getInputPinValue(GPIO_PORT_P4, ALL_SENSOR_PINS);

    // Extract individual bits
    data.identification = (pinStates & PIN_IDENTIFICATION) ? 1 : 0;
    data.movement = (pinStates & PIN_MOVEMENT) ? 1 : 0;

    // Combine room bits
    data.room = 0;
    if (pinStates & PIN_ROOM_0) data.room |= 1;
    if (pinStates & PIN_ROOM_1) data.room |= 2;
    if (pinStates & PIN_ROOM_2) data.room |= 4;

    return data;
}

// Process sensor data and update system state
void processSensorData(void) {
    SensorData sensor_data = readSensorData();

    // Handle authentication
    if (sensor_data.identification) {
        password_correct = 1;
        return;
    }

    // Only process sensor triggers if system is armed
    if (state_code != ALARM_STATE_ARMED) {
        return;
    }

    // Store trigger information
    setTriggerInfo(sensor_data.room, sensor_data.movement);

    // Handle main door (room 1) specially
    if (sensor_data.room == 1 && !sensor_data.movement) {
        opened_safe = 1;
        return;
    }

    // Handle movement sensors or other entries
    if (sensor_data.movement || sensor_data.room > 1) {
        opened_critical = 1;
        return;
    }
}

// Interrupt handler for Port 4
void PORT4_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);

    if (status & ALL_SENSOR_PINS) {
        // Process the sensor data
        processSensorData();

        // Clear the interrupt flags
        GPIO_clearInterruptFlag(GPIO_PORT_P4, status);
    }
}
