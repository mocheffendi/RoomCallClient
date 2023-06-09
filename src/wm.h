// needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager

// for LED status
#include <Ticker.h>
Ticker ticker;

unsigned long previousMillis = 0;
unsigned long interval = 30000;

void tick()
{
    // toggle state
    // int state = digitalRead(LED_BUILTIN); // get the current state of GPIO1 pin
    // digitalWrite(LED_BUILTIN, !state);    // set pin to the opposite state
}

// gets called when WiFiManager enters configuration mode
void configModeCallback(WiFiManager *myWiFiManager)
{
    Serial.println("Entered config mode");
    Serial.println(WiFi.softAPIP());
    // if you used auto generated SSID, print it
    Serial.println(myWiFiManager->getConfigPortalSSID());
    // entered config mode, make led toggle faster
    ticker.attach(0.2, tick);
}

void ensureConnect()
{
    // print the Wi-Fi status every 30 seconds
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        switch (WiFi.status())
        {
        case WL_NO_SSID_AVAIL:
            Serial.println("Configured SSID cannot be reached");
            break;
        case WL_CONNECTED:
            Serial.println("Connection successfully established");
            break;
        case WL_CONNECT_FAILED:
            Serial.println("Connection failed");
            break;
        }
        Serial.printf("Connection status: %d\n", WiFi.status());
        Serial.print("RRSI: ");
        Serial.println(WiFi.RSSI());
        Serial.println("IP Address : ");
        Serial.println(WiFi.localIP());
        previousMillis = currentMillis;
    }
}