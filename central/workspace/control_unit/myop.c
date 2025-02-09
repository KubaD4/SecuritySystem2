//#include <ti/devices/msp432p4xx/inc/msp.h>
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//#include <ti/grlib/grlib.h>
//#include "LcdDriver/Crystalfontz128x128_ST7735.h"
//#include <stdio.h>
//#include "myop.h"
//
///*
// * Aggiorna i LED in base alla selezione
// */
//void updateLEDs(uint8_t option) {
//    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2); // Spegni tutti i LED
//
//    if (option == 0) {
//        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0); // LED Rosso
//    } else if (option == 1) {
//        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1); // LED Verde
//    } else if (option == 2) {
//        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2); // LED Blu
//    }
//}
