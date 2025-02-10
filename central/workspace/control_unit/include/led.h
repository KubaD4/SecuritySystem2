#ifndef LED_H_
#define LED_H_

void setupLEDs(void);
void handleLEDDisarmed(void);   // Verde on
void handleLEDArmed(void);      // Rosso fisso
void handleLEDTriggered(void);  // Rosso lampeggia
void handleLEDGrace(void);      // Entrambi lampeggiano
void stopLEDBlinking(void);

#endif
