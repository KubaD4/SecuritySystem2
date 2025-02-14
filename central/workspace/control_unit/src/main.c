#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "../include/grap.h"
#include "../include/adc.h"
#include <stdint.h>
#include <stdbool.h>
#include "../include/timer.h"

/* TI Drivers & Board includes */
#include <ti/drivers/Board.h>
#include "../include/ti_drivers_config.h"

/* Alarm module includes */
#include "../include/alarm.h" //(_alarmInit to start alarm and _alarmStop to stop it)

/* States module includes*/
#include "../include/states.h"

#include "../include/button1.h"

#include "../include/sound.h"

void _hwInit();


typedef struct{
    State_t state;
    void (*state_function)(void);
} StateMachine_t;


StateMachine_t fsm[] = {
                      {DISARMED, handle_disarmed},
                      {ARMED, handle_armed},
                      {TRIGGERED, handle_triggered},
                      {GRACE, handle_grace},
                      {MAINTENANCE, handle_maintenance},
                      {DELAY, handle_delay}
};


int main(void){
    _hwInit();
    printf("main");

    // Hardware and stuff init
    Board_init();
    printf("board init done");

    current_state = DISARMED;

    // Start in DISARMED state

    prepare_disarmed();

    while(1){
            if(current_state < NUM_STATES){
                (*fsm[current_state].state_function)();
            }
            else{
                return -1;
                //error
            }

    }


}


/*
 * Inizializzazione Hardware
 */
void _hwInit()
{
    /* Halting WDT and disabling master interrupts */
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    _graphicsInit();
    _adcInit();
    _initButton();
    //initSoundDetection();
}
