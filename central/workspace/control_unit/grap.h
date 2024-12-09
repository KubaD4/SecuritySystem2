#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

extern Graphics_Context g_sContext;

void _graphicsInit();
void displayMenu(Graphics_Context *g_sContext, uint8_t selected_option);


#endif
