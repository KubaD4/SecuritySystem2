#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "../include/button1.h"
#include <stdio.h>
#include "../include/states.h"

#define BUTTON_PORT GPIO_PORT_P5
#define BUTTON_PIN GPIO_PIN1

void _initButton(void) {
    GPIO_setAsInputPinWithPullUpResistor(BUTTON_PORT, BUTTON_PIN);

    GPIO_interruptEdgeSelect(BUTTON_PORT, BUTTON_PIN, GPIO_HIGH_TO_LOW_TRANSITION);

    GPIO_clearInterruptFlag(BUTTON_PORT, BUTTON_PIN);


    GPIO_enableInterrupt(BUTTON_PORT, BUTTON_PIN);

    Interrupt_enableInterrupt(INT_PORT5);

    Interrupt_setPriority(INT_PORT5, 2);
}


