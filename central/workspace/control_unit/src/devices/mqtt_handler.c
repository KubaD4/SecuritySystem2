#include "../../include/mqtt_handler.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/states.h"
#include <stdio.h>

// Get room name from room number
const char* getRoomName(int room_number) {
    return ROOM_NAMES[room_number];
}

// Initialize GPIO pin for sensor input
void initSensorGPIO(void) {

    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, PIN_ROOM_1);

    // Configure interrupts
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, PIN_ROOM_1, GPIO_LOW_TO_HIGH_TRANSITION);


    // Clear any previous interrupts
    GPIO_clearInterruptFlag(GPIO_PORT_P5, PIN_ROOM_1);
    // Enable interrupts for sensor pins
    GPIO_enableInterrupt(GPIO_PORT_P5, PIN_ROOM_1);

    // Enable Port interrupts in NVIC
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT5);

    NVIC_SetPriority(PORT5_IRQn, 0);
}



void PORT5_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P5);

    if (status & PIN_ROOM_1) {
        printf("Entrato in P5 handler\n");

        // Clear the interrupt flags for the triggered pins
        GPIO_clearInterruptFlag(GPIO_PORT_P5, status & PIN_ROOM_1);

        // Only process sensor triggers if system is armed
            if (current_state != ARMED) {
                return;
            }


            if (GPIO_getInputPinValue(GPIO_PORT_P5, PIN_ROOM_1)) {
                opened_critical = 0;
                opened_safe = 1;
                setTriggerInfo(1);
                return;
            }
    }
}







