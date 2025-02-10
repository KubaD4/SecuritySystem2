// adc.c
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "../include/states.h"

uint16_t resultsBuffer[2];
uint8_t joystick_moving = 0;

// Thresholds for joystick movement detection
#define JOYSTICK_THRESHOLD_HIGH 14000
#define JOYSTICK_THRESHOLD_LOW  2000

void _adcInit() {
    /* Configure ADC pins */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);  // X-axis
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);  // Y-axis

    // button on Port 3
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN0);  // Button

    /* Configure ADC module */
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    /* Configure ADC memory for X and Y */
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    ADC14_configureConversionMemory(ADC_MEM0,
        ADC_VREFPOS_AVCC_VREFNEG_VSS,
        ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    ADC14_configureConversionMemory(ADC_MEM1,
        ADC_VREFPOS_AVCC_VREFNEG_VSS,
        ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enable interrupts */
    ADC14_enableInterrupt(ADC_INT1);
    Interrupt_enableInterrupt(INT_ADC14);

    /* Configure button interrupt on Port 3 */
    GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN0);
    GPIO_enableInterrupt(GPIO_PORT_P3, GPIO_PIN0);
    Interrupt_enableInterrupt(INT_PORT3);  // Enable Port 3 interrupt

    Interrupt_enableMaster();

    /* Configure sampling timer */
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Start conversion */
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
}

/* Button interrupt handler*/
void PORT3_IRQHandler(void) {
    if (GPIO_getInterruptStatus(GPIO_PORT_P3, GPIO_PIN0)) {
        GPIO_clearInterruptFlag(GPIO_PORT_P3, GPIO_PIN0);

        password_correct = 1;
    }
}

/* ADC interrupt handler */
void ADC14_IRQHandler(void) {
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    if (status & ADC_INT1) {
        resultsBuffer[0] = ADC14_getResult(ADC_MEM0); // X
        resultsBuffer[1] = ADC14_getResult(ADC_MEM1); // Y

        // Process joystick movement
        if (!joystick_moving) {
            // Check X-axis for left/right movement
            if (resultsBuffer[0] > JOYSTICK_THRESHOLD_HIGH) {  // Right movement
                joystick_moving = 1;
                opened_safe = 1;  // Simulate door sensor
            }
            else if (resultsBuffer[0] < JOYSTICK_THRESHOLD_LOW) {  // Left movement
                joystick_moving = 1;
                opened_critical = 1;  // Simulate critical sensor
            }
        }
        // Reset joystick_moving when joystick returns to center
        else if (resultsBuffer[0] > JOYSTICK_THRESHOLD_LOW &&
                 resultsBuffer[0] < JOYSTICK_THRESHOLD_HIGH) {
            joystick_moving = 0;
        }
    }
}
