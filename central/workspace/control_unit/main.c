#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "grap.h"
#include "adc.h"
#include "myop.h"


/* Prototipi */

void _hwInit();




/*
 * Main function
 */
int main(void)
{
    _hwInit();

    while (1)
    {
        /* Attendi il risultato dell'ADC */
        PCM_gotoLPM0();

        /* Controlla il joystick */
        if (resultsBuffer[1] > 14000) { // Su
            if (!joystick_moving) {
                joystick_moving = 1;  // Joystick è in movimento
                if (current_option == 0) {
                    current_option = 2; // Vai all'ultimo
                } else {
                    current_option--;  // Scendi
                }
            }
        } else if (resultsBuffer[1] < 1000) { // Giù
            if (!joystick_moving) {
                joystick_moving = 1;  // Joystick è in movimento
                if (current_option == 2) {
                    current_option = 0; // Torna al primo
                } else {
                    current_option++;  // Salta
                }
            }
        } else {
            joystick_moving = 0;  // Joystick fermo
        }

        /* Aggiorna solo il testo del menu senza cancellare l'intero schermo */
        displayMenu(&g_sContext, current_option);


    }
}




/*
 * Inizializzazione Hardware
 */
void _hwInit() {
    WDT_A_holdTimer();
    Interrupt_disableMaster();

    PCM_setCoreVoltageLevel(PCM_VCORE1);
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Configura GPIO per LED */
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2); // LED Rosso, Verde e Blu
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);

    /* Configura il pulsante per l'interrupt */
        GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4, GPIO_PIN1);  // Pulsante su P4.1
        GPIO_clearInterruptFlag(GPIO_PORT_P4, GPIO_PIN1);  // Pulisce eventuali flag di interrupt
        GPIO_enableInterrupt(GPIO_PORT_P4, GPIO_PIN1);  // Abilita l'interrupt per P4.1

        NVIC_SetPriority(PORT4_IRQn, 1);
        NVIC_SetPriority(ADC14_IRQn, 2);

    _graphicsInit();
    _adcInit();
}









