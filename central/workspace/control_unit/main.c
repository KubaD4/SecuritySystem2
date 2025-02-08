#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "grap.h"
#include "adc.h"
#include "myop.h"
#include <stdint.h>
#include <stdbool.h>

/* TI Drivers & Board includes */
#include <ti/drivers/Board.h>
#include "ti_drivers_config.h"

/* NFC module includes */
#include "nfc.h"

/* Alarm module includes */
#include "alarm.h" //(_alarmInit to start alarm and _alarmStop to stop it)

/* States module includes*/
#include "states.h"

int main(void)
{
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
