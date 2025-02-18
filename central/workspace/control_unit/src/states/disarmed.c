#include "../../include/states.h"

#ifndef TEST_BUILD

#include "../../include/alarm.h"
#include "../../include/grap.h"
#include "../../include/adc.h"
#include "../../include/keypad.h"
#include "../../include/led.h"

void prepare_disarmed() {
    _alarmStop();
    opened_safe = 0;
    opened_critical = 0;
    password_correct = 0;
    menu_selection = 0;
    go_in_maintenance = 0;
    go_in_armed = 0;
    go_in_change_password = 0;
    menu_done = 0;
    displayDisarmedMenu(menu_selection);
    handleLEDDisarmed();
    updateSelection(menu_selection);
    last_selection = 0;
    keypad_clearBuffer();
}

void handle_disarmed(void) {
    if (!menu_done) {
        // PASSWORD ENTRY MODE
        if (go_in_maintenance + go_in_armed + go_in_change_password > 0) {
            writeLCDMessage(" Enter Password ");
            writeLCDsubtitle("<- to go back");
            menu_done = 1;
        } else {
            if (menu_selection != last_selection) {
                updateSelection(menu_selection);
            }
            last_selection = menu_selection;
        }
    }

    /* Display temperature */
    if (temp != prevtemp) {
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 119, 127, 127});
        char string[10];
        sprintf(string, "%d°C", temp);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)string, AUTO_STRING_LENGTH, 64, 119, OPAQUE_TEXT);
    }

    if (back_to_menu) {
        finish_disarmed();
        prepare_disarmed();
        back_to_menu = 0;
    }

    if (menu_done) {
        if (menu_selection == 2) {  // "Change Password" SELECTED
            // VERIFY CURRENT PASSWORD
            if (keypad_authenticate(globalPassword) == KEYPAD_CORRECT) {
                password_correct = 1;
                go_in_change_password = 1;
            }
        } else {
            if (keypad_authenticate(globalPassword) == KEYPAD_CORRECT) {
                password_correct = 1;
            }
        }
    }
}

void finish_disarmed() {
    menu_selection = 0;
    menu_done = 0;
    buttonPreviouslyPressed = 0;
    password_correct = 0;
}

#endif

State_t evaluate_disarmed() {
    if (environment) {
        return TRIGGERED;
    } else if (password_correct && go_in_maintenance) {
        return MAINTENANCE;
    } else if (password_correct && go_in_armed) {
        return DELAY;
    } else if (password_correct && go_in_change_password) {
        return CHANGE_PASSWORD;
    }
    return DISARMED;
}
