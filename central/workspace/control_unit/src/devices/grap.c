#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "../../LcdDriver/Crystalfontz128x128_ST7735.h"
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

// Writes specified message on the screen
void writeLCDsubtitle(const char* message) {

    // Set text color to black
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

    // Draw the message centered on screen
    Graphics_drawStringCentered(&g_sContext,
                              (int8_t *)message,
                              AUTO_STRING_LENGTH,
                              64,  // X center position
                              64,  // Y position
                              TRANSPARENT_TEXT);
}

void clearLCDsubtitle() {
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE); // Set to background color
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 60, 127, 75});
}

// Writes specified message on the screen
void writeLCDtime(const char* message) {

    // Set text color to black
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

    // Draw the message centered on screen
    Graphics_drawStringCentered(&g_sContext,
                              (int8_t *)message,
                              AUTO_STRING_LENGTH,
                              64,  // X center position
                              78,  // Y position
                              TRANSPARENT_TEXT);
}

void clearLCDtime() {
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE); // Set to background color
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 70, 127, 85});
}

void displayDisarmedMenu(uint8_t selected_option) {
    // Pulisce il display
    Graphics_clearDisplay(&g_sContext);

    const int CENTER_X = 64;
    // Aggiorniamo l'array con le 3 opzioni
    const char* MENU_ITEMS[] = {
        "Armed",
        "Maintenance",
        "Change Password"
    };
    // Impostiamo le posizioni verticali per le 3 opzioni
    const int MENU_Y_POSITIONS[] = {45, 65, 85};

    // Imposta il colore nero per il testo
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

    // Disegna il titolo e le istruzioni
    Graphics_drawStringCentered(&g_sContext, (int8_t*)"DISARMED MENU", AUTO_STRING_LENGTH, CENTER_X, 20, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, (int8_t*)"Up/Down to select", AUTO_STRING_LENGTH, CENTER_X, 35, TRANSPARENT_TEXT);

    // Disegna le opzioni di menu
    for (int i = 0; i < 3; i++) {
        // Disegna ogni opzione in nero
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawStringCentered(&g_sContext, (int8_t*)MENU_ITEMS[i], AUTO_STRING_LENGTH, CENTER_X, MENU_Y_POSITIONS[i], TRANSPARENT_TEXT);
        // Se l'opzione è quella selezionata, la ridisegna in rosso (con sfondo opaco)
        if (selected_option == i) {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
            Graphics_drawStringCentered(&g_sContext, (int8_t*)MENU_ITEMS[i], AUTO_STRING_LENGTH, CENTER_X, MENU_Y_POSITIONS[i], OPAQUE_TEXT);
        }
    }
}

void updateSelection(uint8_t selected_option) {
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE); // Imposta il colore di sfondo
    // Aggiorna l'area (ad esempio, da y = 90 a 105) per cancellare il vecchio testo
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 90, 127, 105});

    const int CENTER_X = 64;
    // Ora usiamo 3 opzioni
    const char* MENU_ITEMS[] = {
        "Armed",
        "Maintenance",
        "Change Password"
    };
    const int MENU_Y_POSITIONS[] = {45, 65, 85};

    for (int i = 0; i < 3; i++) {
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawStringCentered(&g_sContext, (int8_t*)MENU_ITEMS[i], AUTO_STRING_LENGTH, CENTER_X, MENU_Y_POSITIONS[i], TRANSPARENT_TEXT);
        if (selected_option == i) {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
            Graphics_drawStringCentered(&g_sContext, (int8_t*)MENU_ITEMS[i], AUTO_STRING_LENGTH, CENTER_X, MENU_Y_POSITIONS[i], OPAQUE_TEXT);
        }
    }
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


