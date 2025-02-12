// adc.c
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "../include/states.h"

uint16_t resultsBuffer[2];
uint8_t joystick_moving = 0;


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

}

void ADC14_IRQHandler(void) {
    uint64_t status = ADC14_getEnabledInterruptStatus();
    static int buttonPreviouslyPressed = 0;
    static int joystickMoved = 0; // Flag to track if movement was already registered

    ADC14_clearInterruptFlag(status);

    if(status & ADC_INT1) {
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

        // Joystick Direction Handling
        if (current_state == DISARMED) {
            if (!joystickMoved) { // Only allow movement if joystick was previously neutral
                if (resultsBuffer[1] > 10000 || resultsBuffer[1] < 4000) {  // movement
                    if (menu_selection == 1){
                        menu_selection = 0;
                    } else if (menu_selection == 0){
                        menu_selection = 1;
                    }

                    joystickMoved = 1; // Mark that movement happened
                }

            }

            // Reset flag when joystick is released (neutral zone)
            if (resultsBuffer[1] >= 5000 && resultsBuffer[1] <= 6000) {
                joystickMoved = 0;
            }
        }

        // Joystick Button Handling
        if (current_state == DISARMED) {
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
        }
    }

}

/*
void ADC14_IRQHandler(void) {
    uint64_t status = ADC14_getEnabledInterruptStatus();
    static int buttonPreviouslyPressed = 0;
    ADC14_clearInterruptFlag(status);

    if(status & ADC_INT1) {
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1);

        // Joystick Direction
        if(current_state == DISARMED) {

            if (resultsBuffer[1] > 12000) {  // Up movement
                menu_selection = (menu_selection - 1 + menu_options) % menu_options;
            }
            else if (resultsBuffer[1] < 2000) {  // Down movement
                menu_selection = (menu_selection + 1) % menu_options;
            }
        }

        // Joystick Button
        if(current_state == DISARMED) {
            int buttonPressed = !(P4IN & GPIO_PIN1);
            if (buttonPressed && !buttonPreviouslyPressed) {
                if ( menu_selection == 0 ){
                    go_in_maintenance = 0;
                    go_in_armed = 1;
                } else if ( menu_selection == 1 ) {
                    go_in_maintenance = 1;
                    go_in_armed = 0;
                }
            }
            buttonPreviouslyPressed = buttonPressed;
        }
    }
}
*/

