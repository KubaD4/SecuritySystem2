#include "../../include/states.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifndef TEST_BUILD

#include "../../include/keypad.h"
#include "../../include/grap.h"
#include "../../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/grlib/grlib.h>

// globalPassword defined in states.c and declared in states.h

// In this state, we assume that the verification of the current password has already occurred (from DISARMED)
// and that we directly enter the phase of entering the new password.

// Prepares the state: displays the fixed title "INSERT NEW PASSWORD:"
void prepare_change_password() {
    back_to_menu = 0;
    keypad_clearBuffer();
    // Display the title (this clears the entire display)
    writeLCDMessage("INSERT NEW PASSWORD:");
}

// Updates only the area where the entered keys are shown
void handle_change_password() {
    static char lastDisplayed[PIN_LENGTH + 1] = "";
    char newPass[PIN_LENGTH + 1];

    // Update the buffer from the keypad
    keypad_update();
    strcpy(newPass, keypad_getBuffer());

    // Update the display only if the content has changed
    if (strcmp(newPass, lastDisplayed) == 0) {
        return;
    }
    strcpy(lastDisplayed, newPass);

    // Do not clear the fixed title ("INSERT NEW PASSWORD:")
    // Clear only the area intended for the entered keys (e.g., from y=70 to y=110)
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 70, 127, 110});

    if (strlen(newPass) == 0) {
        // If the buffer is empty, do nothing (the title remains)
    } else {
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)newPass, AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);
    }
    Graphics_flushBuffer(&g_sContext);
}

// Finishing: displays a final message and resets the buffer and flags
void finish_change_password() {
    writeLCDMessage("Password Changed");
    __delay_cycles(7000000);
    keypad_clearBuffer();
}

#endif

// Evaluation: if the buffer reaches the expected length (or back_to_menu is pressed),
// displays "New Passcode enabled", saves the new password, and returns to DISARMED.
State_t evaluate_change_password() {
    if (strlen(keyBuffer) >= PIN_LENGTH || back_to_menu) {
        // Save the new password without interacting with the display
        strcpy(globalPassword, keyBuffer);
        return DISARMED;
    }
    return CHANGE_PASSWORD;
}
