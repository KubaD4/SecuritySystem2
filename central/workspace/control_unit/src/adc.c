// adc.c
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "../include/states.h"
#include "../include/mqtt_handler.h"
#include "../include/adc.h"

uint16_t resultsBuffer[2];
uint8_t joystickMoved = 0;
int buttonPreviouslyPressed = 0;


/*
 * Inizializzazione ADC
 */
void _adcInit() {
    /* Configures Pin 6.0 and 4.4 as ADC input */
         GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
         GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

         /* Initializing ADC (ADCOSC/64/8) */
         ADC14_enableModule();
         ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

         /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
              * with internal 2.5v reference */
         ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
         ADC14_configureConversionMemory(ADC_MEM0,
                 ADC_VREFPOS_AVCC_VREFNEG_VSS,
                 ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

         ADC14_configureConversionMemory(ADC_MEM1,
                 ADC_VREFPOS_AVCC_VREFNEG_VSS,
                 ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

         /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
          *  is complete and enabling conversions */
         ADC14_enableInterrupt(ADC_INT1);

         /* Enabling Interrupts */
         Interrupt_enableInterrupt(INT_ADC14);
         Interrupt_enableMaster();

         /* Setting up the sample timer to automatically step through the sequence
          * convert.
          */
         ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

         /* Triggering the start of the sample */
         ADC14_enableConversion();
         ADC14_toggleConversionTrigger();

         GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN1);  // Pulsante su P4.1
         GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);  // Pulisce eventuali flag di interrupt
         GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN1);  // Abilita l'interrupt per P4.1

         NVIC_SetPriority(PORT4_IRQn, 1);
         NVIC_SetPriority(ADC14_IRQn, 2);

}

void ADC14_IRQHandler(void) {
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if(status & ADC_INT1) {
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

        // Joystick Direction Handling
        if (current_state == DISARMED) {

                    if (resultsBuffer[1] > 12000) { // Su
                        if (!joystickMoved) {
                            joystickMoved = 1;  // Joystick è in movimento
                            if (menu_selection == 0) {
                                menu_selection = 1; // Vai all'ultimo
                            } else {
                                menu_selection = 0;  // Scendi
                            }
                        }
                    } else if (resultsBuffer[1] < 3000) { // Giù
                        if (!joystickMoved) {
                            joystickMoved = 1;  // Joystick è in movimento
                            if (menu_selection == 1) {
                                menu_selection = 0; // Torna al primo
                            } else {
                                menu_selection = 1;  // Salta
                            }
                        }
                    } else {
                        joystickMoved = 0;  // Joystick fermo
                    }


                }


    }

}

/*
 * Gestione dell'interrupt per il pulsante
 */
void PORT4_IRQHandler(void) {
    uint64_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P4);
    // Joystick Button Handling
        if (status & GPIO_PIN1 && current_state == DISARMED) {
            int buttonPressed = !(P4IN & GPIO_PIN1);
            if (buttonPressed && !buttonPreviouslyPressed) {
                if (menu_selection == 0) {
                    go_in_maintenance = 0;
                    go_in_armed = 1;
                }
                else if (menu_selection == 1) {
                    go_in_maintenance = 1;
                    go_in_armed = 0;
                }
            }
            buttonPreviouslyPressed = buttonPressed;
            GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);
        } else if (status & GPIO_PIN1 && current_state == MAINTENANCE) {
            int buttonPressed = !(P4IN & GPIO_PIN1);
            if (buttonPressed && !buttonPreviouslyPressed) {
                back_to_menu = 1;
            }
            GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);
        }


        if (status & PIN_IDENTIFICATION) {
            printf("Entrato in P4 handler\n");

            SensorData sensor_data = readSensorData();

            // Handle authentication
            if (sensor_data.identification) {
               // password_correct = 1;
            }


            // Clear the interrupt flags
            GPIO_clearInterruptFlag(GPIO_PORT_P4, PIN_IDENTIFICATION);
            printf("__USCITO__\n");
        }
}


