#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <ti/grlib/grlib.h>
extern Graphics_Context g_sContext;

void _graphicsInit();
void writeLCDMessage(const char* message);
void writeLCDsubtitle(const char* message);
void clearLCDsubtitle();
void clearLCDtime();
void writeLCDtime(const char* message);
void updateSelection(uint8_t selected_option);
void displayDisarmedMenu(uint8_t selected_option);

#endif
