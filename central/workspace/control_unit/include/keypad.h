#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

// Length of the expected PIN (e.g., 4 digits)
#define PIN_LENGTH 4

// Keypad verification results
typedef enum {
    KEYPAD_NONE = 0,    // Not enough characters have been entered yet
    KEYPAD_CORRECT,     // The entered PIN is correct
    KEYPAD_ERROR        // The entered PIN is incorrect (and the error message has been shown)
} KeypadResult;

// Keypad module function prototypes
void keypad_init(void);         // Configures the keypad pins
void keypad_update(void);       // Performs a non-blocking scan; to be called periodically
const char* keypad_getBuffer(void);  // Returns the current buffer
void keypad_clearBuffer(void);       // Clears the buffer
bool keypad_verifyPassword(const char * correctPin);  // Returns true if the buffer matches the expected PIN
void keypad_updateDisplay(void); // Updates the lower area of the screen with asterisks

// Function that checks if the PIN has been entered and verifies it; returns the result
KeypadResult keypad_checkInput(const char * correctPin);
KeypadResult keypad_authenticate(const char * correctPin);

#endif // KEYPAD_H
