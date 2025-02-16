#include "../../include/states.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#ifndef TEST_BUILD

#include "../../include/keypad.h"
#include "../../include/grap.h"
#include "../../LcdDriver/Crystalfontz128x128_ST7735.h"
#include <ti/grlib/grlib.h>


// globalPassword definita in states.c e dichiarata in states.h

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

    // Aggiorna il display solo se il contenuto è cambiato
    if (strcmp(newPass, lastDisplayed) == 0) {
        return;
    }
    strcpy(lastDisplayed, newPass);

    // Non cancellare il titolo fisso ("INSERT NEW PASSWORD:")
    // Cancella solo l'area destinata ai tasti inseriti (ad es. da y=70 a y=110)
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, 70, 127, 110});

    if (strlen(newPass) == 0) {
        // Se il buffer è vuoto, non fare nulla (il titolo rimane)
    } else {
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_drawStringCentered(&g_sContext, (int8_t *)newPass, AUTO_STRING_LENGTH, 64, 90, OPAQUE_TEXT);
    }
    Graphics_flushBuffer(&g_sContext);
}

// Finitura: mostra un messaggio finale e resetta il buffer e i flag
void finish_change_password(void) {
    writeLCDMessage("Password Changed");
    __delay_cycles(7000000);
    keypad_clearBuffer();
}

#endif

// Valutazione: se il buffer raggiunge la lunghezza attesa (o viene premuto back_to_menu),
// visualizza "New Passcode enabled", salva la nuova password e torna a DISARMED.
State_t evaluate_change_password(void) {
    if (strlen(keyBuffer) >= PIN_LENGTH || back_to_menu) {
        // Salva la nuova password senza interagire con il display
        strcpy(globalPassword, keyBuffer);
        return DISARMED;
    }
    return CHANGE_PASSWORD;
}

