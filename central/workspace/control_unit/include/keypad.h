#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

// Lunghezza del PIN atteso (es. 4 cifre)
#define PIN_LENGTH 4

// Risultati della verifica del tastierino
typedef enum {
    KEYPAD_NONE = 0,    // Non sono ancora stati inseriti abbastanza caratteri
    KEYPAD_CORRECT,     // Il PIN inserito è corretto
    KEYPAD_ERROR        // Il PIN inserito è errato (e il messaggio d'errore è stato mostrato)
} KeypadResult;

// Prototipi delle funzioni del modulo tastierino
void keypad_init(void);         // Configura i pin del tastierino
void keypad_update(void);       // Effettua una scansione non bloccante; da chiamare periodicamente
const char* keypad_getBuffer(void);  // Restituisce il buffer corrente
void keypad_clearBuffer(void);       // Azzera il buffer
bool keypad_verifyPassword(const char * correctPin);  // Restituisce true se il buffer corrisponde al PIN atteso
void keypad_updateDisplay(void); // Aggiorna l'area bassa dello schermo con gli asterischi

// Funzione che controlla se il PIN è stato inserito e lo verifica; restituisce il risultato
KeypadResult keypad_checkInput(const char * correctPin);
KeypadResult keypad_authenticate(const char * correctPin);

#endif // KEYPAD_H
