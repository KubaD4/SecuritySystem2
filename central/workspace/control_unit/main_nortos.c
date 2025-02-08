//#include <stdint.h>
//#include <stddef.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <unistd.h>
//
//#include <NoRTOS.h>
//
//// Driver configuration
//#include <ti/drivers/Board.h>
//#include <ti/drivers/GPIO.h>
//#include <ti/drivers/Timer.h>
//
//#include "ti_drivers_config.h"
//
//#include <ti/devices/msp432p4xx/inc/msp.h>
//#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
//#include <ti/grlib/grlib.h>
//
//
//
//#include "nfc.h"
//#include "mfrc522.h"
//
//
//
//void configureAll(){
//    // Call driver init functions
//    Board_init();
//    // Start NoRTOS
//    NoRTOS_start();
//    //configBuzzer(NULL);
//    configNFC(NULL);
//
//}
//
//void main(void){
//    char card[32] = {0};
//
//
//    WDT_A_holdTimer(); // stop watchdog timer
//    configureAll();
//
//    printf("RFID Reader Initialized. Please present a card...\n");
//
//
//    while(1){
//
//            if(readCardID(NULL, card)){
//                printf("RFID Card detected: %s\n", card);
//            } else {
//                printf("NOT detected\n");
//            }
//            for (volatile uint32_t i = 0; i < 500000; i++);
//    }
//}
