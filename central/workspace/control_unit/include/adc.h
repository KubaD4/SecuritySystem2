#ifndef ADC_H
#define ADC_H

extern uint16_t resultsBuffer[2];
extern uint8_t joystickMoved;
extern int buttonPreviouslyPressed;

void _adcInit(void);
void ADC14_IRQHandler(void);
void PORT3_IRQHandler(void);

#endif
