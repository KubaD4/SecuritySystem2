#include "../../include/STATES.h"
#include "../../include/keypad.h"
#include "../../include/grap.h"
#include "../../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <string.h>
#include <ti/grlib/grlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef PIN_LENGTH
#define PIN_LENGTH 4
#endif

// globalPassword definita in states.c e dichiarata in states.h
extern char globalPassword[PIN_LENGTH + 1];

// In questo stato, assumiamo che la verifica della password corrente sia già avvenuta (da DISARMED)
// e che entriamo direttamente nella fase di inserimento della nuova password.

// Prepara lo stato: visualizza il titolo fisso "INSERT NEW PASSWORD:"
void prepare_change_password(void) {
    back_to_menu = 0;
    keypad_clearBuffer();
    // Visualizza il titolo (questo cancella l'intero display)
    writeLCDMessage("INSERT NEW PASSWORD:");
}

// aggiorna solo l'area dove mostrare i tasti inseriti
void handle_change_password(void) {
    static char lastDisplayed[PIN_LENGTH + 1] = "";
    char newPass[PIN_LENGTH + 1];

    // Aggiorna il buffer dal tastierino
    keypad_update();
    strcpy(newPass, keypad_getBuffer());

    // Se il contenuto non cambiato, esci senza aggiornare display
    if (strcmp(newPass, lastDisplayed) == 0) {
        return;
    }

    // Altrimenti memorizza il nuovo contenuto
    strcpy(lastDisplayed, newPass);

    // Non cancellare l'intero display: lasciamo il titolo fisso in alto
    // Cancella solo l'area di inserimento (ad es. da y = 70 a 110)
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 70, 127, 110});

    // Se buffer vuoto, visualizza il prompt fisso
    if (strlen(newPass) == 0) {
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"INSERT NEW PASSWORD:",
                                    AUTO_STRING_LENGTH, 64, 50, TRANSPARENT_TEXT);
    } else {
        // Altrimenti visualizza il contenuto inserito in rosso
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)newPass,
                                    AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);
    }

    Graphics_flushBuffer(&g_sContext);
}

// Valutazione: se il buffer raggiunge la lunghezza attesa (o viene premuto back_to_menu),
// visualizza "New Passcode enabled", salva la nuova password e torna a DISARMED.
State_t evaluate_change_password(void) {
    if (strlen(keypad_getBuffer()) >= PIN_LENGTH || back_to_menu) {
        // Mostra il messaggio di conferma nell'area di inserimento
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 70, 127, 110});
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"New Passcode enabled", AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);
        Graphics_flushBuffer(&g_sContext);
        __delay_cycles(3000000);

        // Aggiorna la password globale
        strcpy(globalPassword, keypad_getBuffer());
        return DISARMED;
    }
    return CHANGE_PASSWORD;
}

// Finitura: mostra un messaggio finale e resetta il buffer e i flag
void finish_change_password(void) {
    writeLCDMessage("Password Changed");
    __delay_cycles(7000000);
    keypad_clearBuffer();
}
