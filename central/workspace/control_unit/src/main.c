#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/* TI Drivers & Board includes */
#include <ti/drivers/Board.h>
#include "../include/ti_drivers_config.h"

/* LCD Driver */
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"

/* Project-specific includes */
#include "../include/grap.h"
#include "../include/adc.h"
#include "../include/timer.h"
#include "../include/keypad.h"
#include "../include/lux.h"
#include "../include/tmp.h"
#include "../include/HAL_I2C.h"
#include "../include/HAL_OPT3001.h"
#include "../include/alarm.h" // _alarmInit to start alarm and _alarmStop to stop it
#include "../include/states.h"
/******************************
 *      Global Variables      *
 ******************************/

// State machine definition
StateMachine_t fsm[] = {
    {DISARMED, prepare_disarmed, handle_disarmed, evaluate_disarmed, finish_disarmed},
    {ARMED, prepare_armed, handle_armed, evaluate_armed, finish_armed},
    {TRIGGERED, prepare_triggered, handle_triggered, evaluate_triggered, finish_triggered},
    {GRACE, prepare_grace, handle_grace, evaluate_grace, finish_grace},
    {MAINTENANCE, prepare_maintenance, handle_maintenance, evaluate_maintenance, finish_maintenance},
    {DELAY, prepare_delay, handle_delay, evaluate_delay, finish_delay},
    {CHANGE_PASSWORD, prepare_change_password, handle_change_password, evaluate_change_password, finish_change_password}
};

/******************************
 *      Main Function         *
 ******************************/

/* Variable for storing temperature value returned from TMP006 */

int main(void) {
    // Initialize hardware
    _hwInit();
    printf("Hardware initialized\n");

    // Initialize TI Drivers
    Board_init();
    printf("Board initialized\n");

    // Set initial state
    current_state = DISARMED;
    prepare_disarmed();

    // Main loop
    while (1) {
        // Read light sensor value
        lux = OPT3001_getLux();
        prevtemp=temp;
        temp = (int) TMP006_getTemp();
        temp = (int) (((temp - 32)*5)/9 - 7);



        environment = (lux > 1000 || temp > 50) ? 1 : 0;
        if(environment==1){
            setTriggerInfo(0);
        }

        // Handle current state
        if (current_state < NUM_STATES) {
            (*fsm[current_state].handle_state)();

            next_state = (*fsm[current_state].evaluate_state)();

            if ( next_state != current_state ){
                (*fsm[current_state].finish_state)();
                current_state = next_state;
                (*fsm[current_state].prepare_state)();
            }
        } else {
            // Handle error: invalid state
            printf("Error: Invalid state\n");
            return -1;
        }
    }
}

/******************************
 *      Hardware Initialization
 ******************************/

void _hwInit() {
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1 */
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initialize Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Initialize peripherals */
    _graphicsInit();
    _adcInit();
    _lightSensorInit();
    keypad_init();
    _temperatureSensorInit();
}
