#include "../include/mqtt_handler.h"
#include "../include/alarm.h"
#include "../include/grap.h"
#include "../include/states.h"
#include <stdio.h>



// Get room name from room number
const char* getRoomName(uint8_t room_number) {
    if (room_number < sizeof(ROOM_NAMES)/sizeof(ROOM_NAMES[0])) {
        return ROOM_NAMES[room_number];
    }
    return ROOM_NAMES[0]; // Return "Unknown" for invalid room numbers
}

// Initialize GPIO pins for sensor input
void initSensorGPIO(void) {
    // Configure PIN_IDENTIFICATION as input with pull-down resistor
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, PIN_IDENTIFICATION);

    // Configure room bits (assuming they are in Port 5)
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, PIN_ROOM_0);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, PIN_ROOM_1);

    // Configure interrupts
    GPIO_interruptEdgeSelect(GPIO_PORT_P4, PIN_IDENTIFICATION, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, PIN_ROOM_0 | PIN_ROOM_1, GPIO_LOW_TO_HIGH_TRANSITION);


    // Clear any previous interrupts
    GPIO_clearInterruptFlag(GPIO_PORT_P4, PIN_IDENTIFICATION);
    GPIO_clearInterruptFlag(GPIO_PORT_P5, PIN_ROOM_0 | PIN_ROOM_1);

    // Enable interrupts for sensor pins
    GPIO_enableInterrupt(GPIO_PORT_P4, PIN_IDENTIFICATION);
    GPIO_enableInterrupt(GPIO_PORT_P5, PIN_ROOM_0 | PIN_ROOM_1);

    // Enable Port interrupts in NVIC
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT5);
}


// Read and decode sensor data from GPIO pins
SensorData readSensorData(void) {
    SensorData data;

    data.identification = GPIO_getInputPinValue(GPIO_PORT_P4, PIN_IDENTIFICATION);

    data.room = 0;
    if (GPIO_getInputPinValue(GPIO_PORT_P5, PIN_ROOM_0)) data.room |= 1;
    if (GPIO_getInputPinValue(GPIO_PORT_P5, PIN_ROOM_1)) data.room |= 2;

    return data;
}



void PORT5_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);

    if (status & (PIN_ROOM_0 | PIN_ROOM_1)) {
        printf("Entrato in P5 handler\n");

        SensorData sensor_data = readSensorData();

        // Clear the interrupt flags for the triggered pins
        GPIO_clearInterruptFlag(GPIO_PORT_P5, status & (PIN_ROOM_0 | PIN_ROOM_1));

        // Only process sensor triggers if system is armed
            if (current_state != ARMED) {
                return;
            }

            // Store trigger information
            setTriggerInfo(sensor_data.room);

            // Handle main door (room 1) specially
            if (sensor_data.room == 1) {
                opened_safe = 1;
                return;
            }

            // Handle movement sensors or other entries
            if (sensor_data.room > 1) {
                opened_critical = 1;
                return;
            }


    }


        if(status & BUTTON_PIN) {

            password_correct = 1;

            GPIO_clearInterruptFlag(BUTTON_PORT, BUTTON_PIN);
        }
}





