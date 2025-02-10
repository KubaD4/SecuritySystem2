#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ti/grlib/grlib.h>
extern Graphics_Context g_sContext;

void _graphicsInit();
void writeLCDMessage(const char* message);


#endif
