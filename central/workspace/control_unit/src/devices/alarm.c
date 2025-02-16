#include "../../include/alarm.h"
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//red led connected to 2.5
//green led connected to 2.6

/* Timer_A Compare Config for buzzer PWM generation */
static Timer_A_CompareModeConfig compareConfig_PWM = {
   TIMER_A_CAPTURECOMPARE_REGISTER_4,     // CCR4 for P2.7 buzzer pin
   TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,
   TIMER_A_OUTPUTMODE_TOGGLE_SET,         // Toggle mode for sound generation
   10000                                  // PWM duty cycle
};

/* Timer_A0 config for buzzer base frequency */
static const Timer_A_UpModeConfig upConfig = {
   TIMER_A_CLOCKSOURCE_SMCLK,            // 3MHz source
   TIMER_A_CLOCKSOURCE_DIVIDER_12,       // Divide to 250KHz
   20000,                                // Period for sound frequency
   TIMER_A_TAIE_INTERRUPT_DISABLE,
   TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
   TIMER_A_DO_CLEAR
};

/* Timer_A1 config for LED blinking */
static const Timer_A_UpModeConfig upConfigLED = {
   TIMER_A_CLOCKSOURCE_SMCLK,           // 3MHz source
   TIMER_A_CLOCKSOURCE_DIVIDER_12,      // Divide to 250KHz
   30000,                               // LED blink period
   TIMER_A_TAIE_INTERRUPT_ENABLE,       // Enable interrupt for LED toggle
   TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,
   TIMER_A_DO_CLEAR
};

/* Initialize alarm: configure and start LED and buzzer */
void _alarmInit(void) {

    Timer_A_configureUpMode(TIMER_A1_BASE, &upConfigLED);
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
    Interrupt_enableInterrupt(INT_TA1_N);

   /* Setup buzzer on P2.7 with Timer_A0 for PWM sound */
   GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,
                                           GPIO_PRIMARY_MODULE_FUNCTION);
   Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
   Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
   Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM);
}

/* Stop alarm: disable timers and set pins low */
void _alarmStop(void) {
   Timer_A_stop(TIMER_A0_BASE);         // Stop buzzer timer
   Timer_A_stop(TIMER_A1_BASE);         // Stop LED timer
   GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);
   GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);  // Turn off buzzer

}
