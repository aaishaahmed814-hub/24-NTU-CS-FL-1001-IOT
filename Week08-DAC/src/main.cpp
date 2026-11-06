#include <Arduino.h>

#define DAC_PIN 25

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    // Increase brightness
    for (int value = 0; value <= 255; value++)
    {
        dacWrite(DAC_PIN, value);
        delay(5);  // Change to 5ms or 50ms
    }

    // Decrease brightness
    for (int value = 255; value >= 0; value--)
    {
        dacWrite(DAC_PIN, value);
        delay(5);  // Change to 5ms or 50ms
    }
}