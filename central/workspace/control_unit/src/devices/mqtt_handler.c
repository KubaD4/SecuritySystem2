#include "../../include/mqtt_handler.h"
#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/states.h"
#include <stdio.h>



// Get room name from room number
const char* getRoomName(int room_number) {
    return ROOM_NAMES[room_number];
}

// Initialize GPIO pins for sensor input
void initSensorGPIO(void) {

    // Configure room bits (assuming they are in Port 5)
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P2, PIN_ROOM_0);
    GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P5, PIN_ROOM_1);

    // Configure interrupts
    GPIO_interruptEdgeSelect(GPIO_PORT_P5, PIN_ROOM_1, GPIO_LOW_TO_HIGH_TRANSITION);
    GPIO_interruptEdgeSelect(GPIO_PORT_P2, PIN_ROOM_0, GPIO_LOW_TO_HIGH_TRANSITION);


    // Clear any previous interrupts
    GPIO_clearInterruptFlag(GPIO_PORT_P5, PIN_ROOM_1);
    GPIO_clearInterruptFlag(GPIO_PORT_P2, PIN_ROOM_0);

    // Enable interrupts for sensor pins
    GPIO_enableInterrupt(GPIO_PORT_P5, PIN_ROOM_1);
    GPIO_enableInterrupt(GPIO_PORT_P2, PIN_ROOM_0);

    // Enable Port interrupts in NVIC
    Interrupt_enableInterrupt(INT_PORT4);
    Interrupt_enableInterrupt(INT_PORT5);
    Interrupt_enableInterrupt(INT_PORT2);
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

            setTriggerInfo(1);


            if (GPIO_getInputPinValue(GPIO_PORT_P5, PIN_ROOM_1)) {
                opened_critical = 1;
                opened_safe = 0;
                return;
            }
    }

    if(status & BUTTON_PIN) {
        password_correct = 1;
        GPIO_clearInterruptFlag(BUTTON_PORT, BUTTON_PIN);
    }
}

void PORT2_IRQHandler(void) {
    uint32_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);

    if (status & PIN_ROOM_0) {
        printf("Entrato in P2 handler\n");

        // Clear the interrupt flags for the triggered pins
        GPIO_clearInterruptFlag(GPIO_PORT_P2, status & PIN_ROOM_0);

        // Only process sensor triggers if system is armed
        if (current_state != ARMED) {
            return;
        }

        setTriggerInfo(2);

        if (GPIO_getInputPinValue(GPIO_PORT_P2, PIN_ROOM_0)) {
            printf("Tua mamma\n");
            opened_safe = 1;
            opened_critical = 0;
        }
    }
}





