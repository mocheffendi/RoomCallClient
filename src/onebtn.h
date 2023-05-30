#include "OneButton.h"

#define PIN_INPUT D3
#define PIN_LED D4

// Setup a new OneButton on pin PIN_INPUT
// The 2. parameter activeLOW is true, because external wiring sets the button to LOW when pressed.
OneButton button(PIN_INPUT, true);

// current LED state, staring with LOW (0)
int ledState = LOW;

// save the millis when a press has started.
unsigned long pressStartTime;

IRAM_ATTR void checkTicks()
{
    // include all buttons here to be checked
    button.tick(); // just call tick() to check the state.
}

// this function will be called when the button was pressed 2 times in a short timeframe.
void doubleClick()
{
    Serial.println("doubleClick() detected.");

    ledState = !ledState; // reverse the LED
    digitalWrite(PIN_LED, ledState);
} // doubleClick
