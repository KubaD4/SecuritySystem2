//#include <ti/devices/msp432p4xx/inc/msp.h>
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//#include <ti/grlib/grlib.h>
//#include "LcdDriver/Crystalfontz128x128_ST7735.h"
//#include <stdio.h>
//#include "../include/states.h"
//#include "../include/alarm.h"
//
//// Define sound threshold for triggering
//#define SOUND_THRESHOLD 20000  // Adjust this value based on testing
//#define SAMPLES_ABOVE_THRESHOLD 5  // Number of consecutive samples needed
//#define SAMPLING_PERIOD_MS 100  // Time between samples in milliseconds
//
//static volatile int16_t micReading;
//static volatile uint8_t samplesAboveThreshold = 0;
//static volatile uint8_t soundDetectionEnabled = 0;
//
///* Timer configuration for regular sampling */
//const Timer_A_UpModeConfig soundSamplingTimer = {
//    TIMER_A_CLOCKSOURCE_SMCLK,           // Use SMCLK as clock source
//    TIMER_A_CLOCKSOURCE_DIVIDER_64,      // Divide clock source by 64
//    46875,                               // Period for 100ms sampling
//    TIMER_A_TAIE_INTERRUPT_ENABLE,       // Enable timer interrupt
//    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, // Disable CCR0 interrupt
//    TIMER_A_DO_CLEAR                     // Clear counter
//};
//
//void initSoundDetection() {
//    ADC14_enableModule();
//    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, ADC_NOROUTE);
//
//    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN3,
//                                             GPIO_TERTIARY_MODULE_FUNCTION);
//
//    ADC14_configureSingleSampleMode(ADC_MEM0, true);
//    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
//                                  ADC_INPUT_A10, false);
//
//    ADC14_setResultFormat(ADC_SIGNED_BINARY);
//
//    ADC14_enableInterrupt(ADC_INT0);
//    Interrupt_enableInterrupt(INT_ADC14);
//
//    Timer_A_configureUpMode(TIMER_A2_BASE, &soundSamplingTimer);
//    Interrupt_enableInterrupt(INT_TA2_N);
//}
//
//void enableSoundDetection() {
//    soundDetectionEnabled = 1;
//    samplesAboveThreshold = 0;
//
//    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
//
//    ADC14_enableConversion();
//    ADC14_toggleConversionTrigger();
//}
//
//void disableSoundDetection() {
//    soundDetectionEnabled = 0;
//    Timer_A_stopTimer(TIMER_A2_BASE);
//    ADC14_disableConversion();
//}
//
///* ADC Interrupt Handler */
//void ADC14_IRQHandler(void) {
//    uint64_t status = ADC14_getEnabledInterruptStatus();
//    ADC14_clearInterruptFlag(status);
//
//    if (status & ADC_INT0) {
//        micReading = ADC14_getResult(ADC_MEM0);
//
//        if (soundDetectionEnabled && state_code == ALARM_STATE_ARMED) {
//            if (abs(micReading) > SOUND_THRESHOLD) {
//                samplesAboveThreshold++;
//                if (samplesAboveThreshold >= SAMPLES_ABOVE_THRESHOLD) {
//                    opened_critical = 1;
//                    samplesAboveThreshold = 0;  // Reset counter
//                }
//            } else {
//                samplesAboveThreshold = 0;
//            }
//        }
//    }
//}
//
//void TA2_N_IRQHandler(void) {
//    Timer_A_clearInterruptFlag(TIMER_A2_BASE);
//
//    if (soundDetectionEnabled) {
//        ADC14_toggleConversionTrigger();
//    }
//}
