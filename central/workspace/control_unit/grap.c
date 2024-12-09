#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>
#include "myop.h"

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

/*
 * Mostra il menu sul display
 */
void displayMenu(Graphics_Context *g_sContext, uint8_t selected_option) {
    // Aggiorna il menu con tre opzioni
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);

    if (selected_option == 1 || selected_option == 2) {
        Graphics_drawStringCentered(g_sContext,
                                    (int8_t *)"Modalita' LED Rosso",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    30,
                                    TRANSPARENT_TEXT);
    }
    if (selected_option == 0 || selected_option == 2) {
        Graphics_drawStringCentered(g_sContext,
                                    (int8_t *)"Modalita' LED Verde",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    50,
                                    TRANSPARENT_TEXT);
    }

    if (selected_option == 0 || selected_option == 1) {
        Graphics_drawStringCentered(g_sContext,
                                    (int8_t *)"Modalita' LED Blu",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    70,
                                    TRANSPARENT_TEXT);
    }

    // Evidenziazione
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_RED);
    if (selected_option == 0) {
        Graphics_drawStringCentered(g_sContext,
                                    (int8_t *)"Modalita' LED Rosso",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    30,
                                    OPAQUE_TEXT);
    } else if (selected_option == 1) {
        Graphics_drawStringCentered(g_sContext,
                                    (int8_t *)"Modalita' LED Verde",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    50,
                                    OPAQUE_TEXT);
    } else if (selected_option == 2) {
        Graphics_drawStringCentered(g_sContext,
                                    (int8_t *)"Modalita' LED Blu",
                                    AUTO_STRING_LENGTH,
                                    64,
                                    70,
                                    OPAQUE_TEXT);
    }
}

