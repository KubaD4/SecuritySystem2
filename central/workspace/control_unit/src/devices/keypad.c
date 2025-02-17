#include "../../include/keypad.h"
#include "../../include/states.h"
#include <stdio.h>
#include <string.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/drivers/GPIO.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// The graphic context is defined elsewhere (e.g., in main.c)
extern Graphics_Context g_sContext;

// 4x4 keypad mapping
static char keymap[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Buffer to store the entered keys
// static char keyBuffer[PIN_LENGTH + 1] = {0};
static uint8_t keyBufferIndex = 0;

// Variable to avoid repeatedly detecting the same key (debounce)
static bool keyAlreadyPressed = false;

//
// Configures the pins for the keypad
//
void keypad_init(void) {
    // Configure columns as outputs and set them HIGH
    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);

    GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN7);
    GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN3);

    GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN5);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);

    // Configure rows as inputs with pull-up resistors
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN0);  // Row 0
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN7);  // Row 1
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN2);  // Row 2
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P3, GPIO_PIN6);  // Row 3

    // Clear the buffer and debounce state
    keypad_clearBuffer();
    keyAlreadyPressed = false;
}

//
// Non-blocking function to scan the keypad.
// If a new key is detected, it returns the key; otherwise, it returns '\0'.
//
char keypad_poll(void) {
    char detectedKey = '\0';

    // Scan each column
    for (int col = 0; col < 4; col++) {
        // First, ensure all columns are HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN6);
        GPIO_setOutputHighOnPin(GPIO_PORT_P6, GPIO_PIN7);
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN3);
        GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN5);

        // Set the current column LOW
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
        // Short delay to stabilize the signal
        __delay_cycles(500);

        // Scan the rows
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
            if (pinValue == 0) {  // Key pressed (pull-up)
                detectedKey = keymap[row][col];
                break;
            }
        }
        if (detectedKey != '\0')
            break;
    }

    // If a key is detected and it hasn't been registered yet, return it
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
// Function to be called periodically (e.g., from a timer interrupt or main loop)
// to update the keypad buffer
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
// Checks if the entered PIN matches the expected one.
// Returns true if they match, false otherwise.
//
bool keypad_verifyPassword(const char * correctPin) {
    return (strcmp(keyBuffer, correctPin) == 0);
}

//
// Updates the lower area of the screen with asterisks representing
// the entered keys. It only updates a specific area (e.g., y from 100 to 127)
// without clearing the upper area (where other messages might be displayed).
//
void keypad_updateDisplay(void) {
    // Area to display the asterisks (you can adjust the values as needed)
    int y_start = 100;
    int height = 28;

    // Builds a string with as many asterisks as the entered characters
    char displayStr[PIN_LENGTH + 1];
    for (uint8_t i = 0; i < keyBufferIndex; i++) {
        displayStr[i] = '*';
    }
    displayStr[keyBufferIndex] = '\0';

    // Use a static variable to remember the last displayed content
    static char lastDisplayed[PIN_LENGTH + 1] = "";

    // If the current string is identical to the one already shown, do nothing (avoid flickering)
    if (strcmp(displayStr, lastDisplayed) == 0) {
        return;
    }

    // Update the stored content
    strcpy(lastDisplayed, displayStr);

    // Clear the area where the asterisks should appear
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_fillRectangle(&g_sContext, &(Graphics_Rectangle){0, y_start, 127, 112});

    // Draw the string centered (both horizontally and vertically in the dedicated area)
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)displayStr, AUTO_STRING_LENGTH, 64, 100, OPAQUE_TEXT);

    Graphics_flushBuffer(&g_sContext);
}

//
// Function that performs keypad updates and, if the buffer is complete, checks the PIN.
// Returns KEYPAD_CORRECT, KEYPAD_ERROR, or KEYPAD_NONE.
//
KeypadResult keypad_authenticate(const char * correctPin) {
    keypad_update();
    if (strlen(keypad_getBuffer()) >= PIN_LENGTH) {
        return keypad_checkInput(correctPin);
    }
    return KEYPAD_NONE;
}

//
// Function that checks if the buffer has reached the full PIN length and verifies it.
// If the buffer contains at least PIN_LENGTH characters, it compares the content with the expected PIN.
// If correct, it clears the buffer and returns KEYPAD_CORRECT. If incorrect, it displays an error message,
// clears the buffer, and returns KEYPAD_ERROR. If the buffer is not complete, it returns KEYPAD_NONE.
//
KeypadResult keypad_checkInput(const char * correctPin) {
    if (strlen(keyBuffer) < PIN_LENGTH)
        return KEYPAD_NONE;

    if (keypad_verifyPassword(correctPin)) {
        keypad_clearBuffer();
        return KEYPAD_CORRECT;
    } else {
        // Display "Wrong Code" in the lower area
        Graphics_drawStringCentered(&g_sContext, (int8_t*)"Wrong Code", AUTO_STRING_LENGTH, 64, 110, OPAQUE_TEXT);
        Graphics_flushBuffer(&g_sContext);
        keypad_clearBuffer();
        return KEYPAD_ERROR;
    }
}
