#include "OneButton.h"

#define PIN_INPUT 2
#define PIN_LED 5

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
