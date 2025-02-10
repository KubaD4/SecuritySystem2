#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

/* Graphic library context */
Graphics_Context g_sContext;

/*
 * Inizializzazione LCD
 */
void _graphicsInit() {
    /* Inizializza il display */
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Configura il contesto grafico */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
}

// Writes specified message on the screen
void writeLCDMessage(const char* message) {
    // Clear previous display
    Graphics_clearDisplay(&g_sContext);

    // Set text color to black
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

    // Draw the message centered on screen
    Graphics_drawStringCentered(&g_sContext,
                              (int8_t *)message,
                              AUTO_STRING_LENGTH,
                              64,  // X center position
                              50,  // Y position
                              TRANSPARENT_TEXT);
}

/*
 * Vecchia
 */
//void displayMenu(Graphics_Context *g_sContext, uint8_t selected_option) {
//    Graphics_clearDisplay(&g_sContext);
//
//    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);
//
//    if (selected_option == 1) {
//        Graphics_drawStringCentered(g_sContext,
//                                    (int8_t *)"ALLARME ATTIVO",
//                                    AUTO_STRING_LENGTH,
//                                    64,
//                                    50,
//                                    TRANSPARENT_TEXT);
//    } else if (selected_option == 0) {
//        Graphics_drawStringCentered(g_sContext,
//                                    (int8_t *)"ALLARME DISATTIVATO",
//                                    AUTO_STRING_LENGTH,
//                                    64,
//                                    50,
//                                    OPAQUE_TEXT);
//    }
//}


