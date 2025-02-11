#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "../include/grap.h"
#include "../include/adc.h"
#include <stdint.h>
#include <stdbool.h>

/* TI Drivers & Board includes */
#include <ti/drivers/Board.h>
#include "../include/ti_drivers_config.h"

/* Alarm module includes */
#include "../include/alarm.h" //(_alarmInit to start alarm and _alarmStop to stop it)

/* States module includes*/
#include "../include/states.h"

/* Prototipi */

void _hwInit();


/* ADC results buffer */
static uint16_t resultsBuffer[2];

int main(void)
{
    _hwInit();
    printf("main");
    // Hardware and stuff init
    Board_init();
    printf("board init done");


    // Start in DISARMED state
    state_code = ALARM_STATE_DISARMED;
    prepare_disarmed();

    while (1)
    {
        switch (state_code) {
            case ALARM_STATE_ARMED:
                handle_armed();
                break;
            case ALARM_STATE_DISARMED:
                handle_disarmed();
                break;
            case ALARM_STATE_GRACE:
                handle_grace();
                break;
            case ALARM_STATE_TRIGGERED:
                handle_triggered();
                break;
        }
    }
    return 0;
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
}
