int blue = 2;
unsigned long intervalBlue = 1000; // how long to delay in millis
unsigned long previousBlue = 0;
int blueState = LOW;

#define OFF 1
#define ON 0

void fancyled()
{
    for (int i = 0; i < 30; i++)
    {
        analogWrite(blue, (i * 100) % 1001);
        delay(50);
    }
}

void led_indicator()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousBlue >= intervalBlue)
    {
        // save this reading!
        previousBlue = currentMillis;

        // figure out if you should turn the LED on or off
        if (blueState == LOW)
        {
            blueState = HIGH;
        }
        else
        {
            blueState = LOW;
        }
        digitalWrite(blue, blueState);
    }
}

// Makes the LED blink once every 1 second.
void blinkOnce()
{
    pinMode(blue, OUTPUT);
    // Turning ON.
    digitalWrite(blue, ON);
    delay(1000);
    // Turning OFF.
    digitalWrite(blue, OFF);
    delay(1000);
}

// Makes the LED blink twice every 1 second.
void blinkTwice()
{
    pinMode(blue, OUTPUT);
    // Turning ON.
    digitalWrite(blue, ON);
    delay(500);
    // Turning OFF.
    digitalWrite(blue, OFF);
    delay(500);
}

// Makes the LED blink n-times every 1 second.
void blinkN(size_t n)
{
    pinMode(blue, OUTPUT);
    // Turning ON.
    digitalWrite(blue, ON);
    delay(1000 / n);
    // Turning OFF.
    digitalWrite(blue, OFF);
    delay(1000 / n);
}

// Makes the LED stop blinking.
void blinkStop()
{
    pinMode(blue, OUTPUT);
    digitalWrite(blue, OFF);
}

// Makes the LED fade in and out once every 1 second.
void fadeOnce()
{
    pinMode(blue, OUTPUT);
    // Fading in.
    for (int i = 0; i < 1024; i += 1024 / 20)
    {
        analogWrite(blue, i);
        delay(100);
    }
    // Fading out.
    for (int i = 1023; i > 0; i -= 1024 / 20)
    {
        analogWrite(blue, i);
        delay(100);
    }
}

// Makes the LED fade in and out twice every 1 second.
void fadeTwice()
{
    pinMode(blue, OUTPUT);
    // Fading in.
    for (int i = 0; i < 1024; i += 1024 / 20)
    {
        analogWrite(blue, i);
        delay(50);
    }
    // Fading out.
    for (int i = 1023; i > 0; i -= 1024 / 20)
    {
        analogWrite(blue, i);
        delay(50);
    }
}

// Makes the LED fade in and out n-times every 1 second.
void fadeN(size_t n)
{
    pinMode(blue, OUTPUT);
    // Fading in.
    for (int i = 0; i < 1024; i += 1024 / 20)
    {
        analogWrite(blue, i);
        delay(100 / n);
    }
    // Fading out.
    for (int i = 1023; i > 0; i -= 1024 / 20)
    {
        analogWrite(blue, i);
        delay(100 / n);
    }
}

// define directions for LED fade
#define UP 0
#define DOWN 1

// constants for min and max PWM
const int minPWM = 0;
const int maxPWM = 255;

// State Variable for Fade Direction
byte fadeDirection = UP;

// Global Fade Value
// but be bigger than byte and signed, for rollover
int fadeValue = 0;

// How smooth to fade?
byte fadeIncrement = 5;

// millis() timing Variable, just for fading
unsigned long previousFadeMillis;

// How fast to increment?
unsigned long fadeInterval = 50;

void doTheFade(unsigned long thisMillis)
{
    // is it time to update yet?
    // if not, nothing happens
    if (thisMillis - previousFadeMillis >= fadeInterval)
    {
        // yup, it's time!
        if (fadeDirection == UP)
        {
            fadeValue = fadeValue + fadeIncrement;
            if (fadeValue >= maxPWM)
            {
                // At max, limit and change direction
                fadeValue = maxPWM;
                fadeDirection = DOWN;
            }
        }
        else
        {
            // if we aren't going up, we're going down
            fadeValue = fadeValue - fadeIncrement;
            if (fadeValue <= minPWM)
            {
                // At min, limit and change direction
                fadeValue = minPWM;
                fadeDirection = UP;
            }
        }
        // Only need to update when it changes
        analogWrite(blue, fadeValue);

        // reset millis for the next iteration (fade timer only)
        previousFadeMillis = thisMillis;
    }
}