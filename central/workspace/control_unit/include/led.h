#ifndef LED_H_
#define LED_H_

void setupLEDs(void);
void handleLEDDisarmed(void);   // Green on
void handleLEDArmed(void);      // Red steady
void handleLEDTriggered(void);  // Red blinking
void handleLEDGrace(void);      // Both blinking
void stopLEDBlinking(void);

#endif
