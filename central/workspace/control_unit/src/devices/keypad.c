#include "../../include/keypad.h"
#include "../../include/states.h"
#include <stdio.h>
#include <string.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/drivers/GPIO.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// Il contesto grafico è definito altrove (ad es. in main.c)
extern Graphics_Context g_sContext;

// Mappa del tastierino 4x4
static char keymap[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Buffer per salvare i tasti inseriti
// static char keyBuffer[PIN_LENGTH + 1] = {0};
static uint8_t keyBufferIndex = 0;

// Variabile per evitare di rilevare ripetutamente lo stesso tasto (debounce)
static bool keyAlreadyPressed = false;

//
// Configura i pin per il tastierino
//
void keypad_init(void) {
    // Configura le colonne come uscite e portale HIGH
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);

    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN7);
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN3);

    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);

    // Configura le righe come ingressi con pull-up
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN0);  // Row 0
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN7);  // Row 1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN2);  // Row 2
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN6);  // Row 3

    // Azzeriamo il buffer e lo stato di debounce
    keypad_clearBuffer();
    keyAlreadyPressed = false;
}

//
// Funzione non bloccante per scansionare il tastierino.
// Se viene rilevato un nuovo tasto, lo restituisce; altrimenti, restituisce '\0'.
//
char keypad_poll(void) {
    char detectedKey = '\0';

    // Scansiona ogni colonna
    for (int col = 0; col < 4; col++) {
        // Prima, assicura che tutte le colonne siano HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN7);
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN3);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);

        // Imposta la colonna corrente LOW
        switch (col) {
            case 0:
                GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN6);
                break;
            case 1:
                GPIO_setOutputLowOnPin(GPIO_PORT_P6, GPIO_PIN7);
                break;
            case 2:
                GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN3);
                break;
            case 3:
                GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN5);
                break;
        }
        // Breve attesa per stabilizzare il segnale
        __delay_cycles(500);

        // Scansiona le righe
        for (int row = 0; row < 4; row++) {
            uint8_t pinValue = 1;
            switch (row) {
                case 0:
                    pinValue = GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN0);
                    break;
                case 1:
                    pinValue = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN7);
                    break;
                case 2:
                    pinValue = GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN2);
                    break;
                case 3:
                    pinValue = GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN6);
                    break;
            }
            if (pinValue == 0) {  // Tasto premuto (pul-up)
                detectedKey = keymap[row][col];
                break;
            }
        }
        if (detectedKey != '\0')
            break;
    }

    // Se viene rilevato un tasto e non è già stato registrato, restituiscilo
    if (detectedKey != '\0') {
        if (!keyAlreadyPressed) {
            keyAlreadyPressed = true;
            return detectedKey;
        } else {
            return '\0';
        }
    } else {
        keyAlreadyPressed = false;
        return '\0';
    }
}

//
// Funzione da chiamare periodicamente (es. dal timer interrupt o dal main loop)
// per aggiornare il buffer del tastierino
//
void keypad_update(void) {
    char key = keypad_poll();
    if (key != '\0' && keyBufferIndex < PIN_LENGTH) {
        keyBuffer[keyBufferIndex++] = key;
        keyBuffer[keyBufferIndex] = '\0';
    }
    keypad_updateDisplay();
}

void keypad_clearBuffer(void) {
    memset(keyBuffer, 0, sizeof(keyBuffer));
    keyBufferIndex = 0;
}

const char* keypad_getBuffer(void) {
    return keyBuffer;
}

//
// Verifica se il PIN inserito corrisponde a quello atteso.
// Restituisce true se uguale, false altrimenti.
//
bool keypad_verifyPassword(const char * correctPin) {
    return (strcmp(keyBuffer, correctPin) == 0);
}

//
// Aggiorna l'area bassa dello schermo con gli asterischi che rappresentano
// i tasti inseriti. Si limita ad aggiornare un'area (es. y da 100 a 127) senza
// cancellare l'area superiore (dove possono esserci altri messaggi).
//
void keypad_updateDisplay(void) {
    // Area in cui visualizzare gli asterischi (puoi regolare i valori in base alle tue esigenze)
    int y_start = 100;
    int height = 28;

    // Costruisce la stringa con tanti asterischi quanti sono i caratteri inseriti
    char displayStr[PIN_LENGTH + 1];
    for (uint8_t i = 0; i < keyBufferIndex; i++) {
        displayStr[i] = '*';
    }
    displayStr[keyBufferIndex] = '\0';

    // Usa una variabile statica per ricordare l'ultimo contenuto visualizzato
    static char lastDisplayed[PIN_LENGTH + 1] = "";

    // Se la stringa corrente è identica a quella già mostrata, non fare nulla (evita vibrazioni)
    if (strcmp(displayStr, lastDisplayed) == 0) {
        return;
    }

    // Aggiorna il contenuto memorizzato
    strcpy(lastDisplayed, displayStr);

    // Cancella l'area in cui devono comparire gli asterischi
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, y_start, 127, y_start + height - 1});

    // Disegna la stringa centrata (sia orizzontalmente che verticalmente nell'area dedicata)
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)displayStr, AUTO_STRING_LENGTH, 64, y_start + (height / 2), OPAQUE_TEXT);

    Graphics_flushBuffer(&g_sContext);
}


//
// Funzione che esegue l'aggiornamento del tastierino e, se il buffer è completo, controlla il PIN.
// Restituisce KEYPAD_CORRECT, KEYPAD_ERROR o KEYPAD_NONE.
//

KeypadResult keypad_authenticate(const char * correctPin) {
    keypad_update();
    if (strlen(keypad_getBuffer()) >= PIN_LENGTH) {
        return keypad_checkInput(correctPin);
    }
    return KEYPAD_NONE;
}

//
// Funzione che controlla se il buffer ha raggiunto il PIN completo e lo verifica.
// Se il buffer contiene almeno PIN_LENGTH caratteri, confronta il contenuto con il PIN atteso.
// Se corretto, azzera il buffer e restituisce KEYPAD_CORRECT. Se errato, visualizza un messaggio di errore,
// azzera il buffer e restituisce KEYPAD_ERROR. Se il buffer non è completo, restituisce KEYPAD_NONE.
//
KeypadResult keypad_checkInput(const char * correctPin) {
    if (strlen(keyBuffer) < PIN_LENGTH)
        return KEYPAD_NONE;

    if (keypad_verifyPassword(correctPin)) {
        keypad_clearBuffer();
        return KEYPAD_CORRECT;
    } else {
        // Visualizza "Codice Errato" nell'area bassa
        Graphics_drawStringCentered(&g_sContext, (int8_t*)"Codice Errato", AUTO_STRING_LENGTH, 64, 110, OPAQUE_TEXT);
        Graphics_flushBuffer(&g_sContext);
        keypad_clearBuffer();
        return KEYPAD_ERROR;
    }
}
