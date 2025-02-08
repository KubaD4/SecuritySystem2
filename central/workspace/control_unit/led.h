#ifndef LED_H_
#define LED_H_

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void setupLEDs(void);
void handleLEDDisarmed(void);   // Verde on
void handleLEDArmed(void);      // Rosso fisso
void handleLEDTriggered(void);  // Rosso lampeggia
void handleLEDGrace(void);      // Entrambi lampeggiano
void stopLEDBlinking(void);

#endif
