#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "../../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <stdio.h>

/* Graphic library context */
Graphics_Context g_sContext;

/*
 * LCD Initialization
 */
void _graphicsInit() {
    /* Initialize the display */
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Configure the graphic context */
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

// Writes specified message on a lower area of the screen
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

// Clears the subtitle zone
void clearLCDsubtitle() {
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE); // Set to background color
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 60, 127, 75});
}

// Writes specified message on an area dedicate to the time
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

// Clears the time zone
void clearLCDtime() {
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE); // Set to background color
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 70, 127, 85});
}

void displayDisarmedMenu(uint8_t selected_option) {
    // Clear the display
    Graphics_clearDisplay(&g_sContext);

    const int CENTER_X = 64;
    // Update the array with the 3 options
    const char* MENU_ITEMS[] = {
        "Armed",
        "Maintenance",
        "Change Password"
    };
    // Set vertical positions for the 3 options
    const int MENU_Y_POSITIONS[] = {45, 65, 85};

    // Set black color for the text
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);

    // Draw the title and instructions
    Graphics_drawStringCentered(&g_sContext, (int8_t*)"DISARMED MENU", AUTO_STRING_LENGTH, CENTER_X, 20, TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, (int8_t*)"Up/Down to select", AUTO_STRING_LENGTH, CENTER_X, 35, TRANSPARENT_TEXT);

    // Draw the menu options
    for (int i = 0; i < 3; i++) {
        // Draw each option in black
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawStringCentered(&g_sContext, (int8_t*)MENU_ITEMS[i], AUTO_STRING_LENGTH, CENTER_X, MENU_Y_POSITIONS[i], TRANSPARENT_TEXT);
        // If the option is selected, redraw it in red (with opaque background)
        if (selected_option == i) {
            Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
            Graphics_drawStringCentered(&g_sContext, (int8_t*)MENU_ITEMS[i], AUTO_STRING_LENGTH, CENTER_X, MENU_Y_POSITIONS[i], OPAQUE_TEXT);
        }
    }
}

// Updates the selected option to be in red
void updateSelection(uint8_t selected_option) {
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE); // Set background color
    // Update the area (e.g., from y = 90 to 105) to clear the old text
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 90, 127, 105});

    const int CENTER_X = 64;
    // Now we use 3 options
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
