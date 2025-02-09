//#include <ti/devices/msp432p4xx/inc/msp.h>
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//#include <ti/grlib/grlib.h>
//#include "LcdDriver/Crystalfontz128x128_ST7735.h"
//#include <stdio.h>
//#include "myop.h"
//
//uint16_t resultsBuffer[2];
//uint8_t current_option = 0;
//uint8_t joystick_moving = 0;
//
//
///*
// * Inizializzazione ADC
// */
//void _adcInit() {
//    /* Configura i pin ADC */
//    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
//    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);
//
//    /* Configurazione modulo ADC */
//    ADC14_enableModule();
//    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);
//
//    /* Configura memoria ADC per X e Y */
//    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
//    ADC14_configureConversionMemory(ADC_MEM0,
//        ADC_VREFPOS_AVCC_VREFNEG_VSS,
//        ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);
//
//    ADC14_configureConversionMemory(ADC_MEM1,
//        ADC_VREFPOS_AVCC_VREFNEG_VSS,
//        ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);
//
//    /* Abilita interruzioni */
//    ADC14_enableInterrupt(ADC_INT1);
//    Interrupt_enableInterrupt(INT_ADC14);
//    Interrupt_enableMaster();
//    Interrupt_enableInterrupt(INT_PORT4);
//
//    /* Configura il timer per il campionamento */
//    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
//
//    /* Avvia la conversione */
//    ADC14_enableConversion();
//    ADC14_toggleConversionTrigger();
//}
//
//
///*
// * Gestione dell'interrupt per il pulsante
// */
//void PORT4_IRQHandler(void) {
//    // Verifica se l'interrupt � stato causato dal pulsante
//    if (GPIO_getInterruptStatus(GPIO_PORT_P4, GPIO_PIN1)) {
//        GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);  // Pulisce il flag dell'interrupt
//        updateLEDs(current_option);  // Aggiorna i LED in base all'opzione selezionata
//    }
//}
//
///*
// * Gestione dell'interrupt ADC
// */
//void ADC14_IRQHandler(void) {
//    uint64_t status = ADC14_getEnabledInterruptStatus();
//    ADC14_clearInterruptFlag(status);
//
//    if (status & ADC_INT1) {
//        resultsBuffer[0] = ADC14_getResult(ADC_MEM0); // X
//        resultsBuffer[1] = ADC14_getResult(ADC_MEM1); // Y
//    }
//}
//
//
//
//
//
//
//
