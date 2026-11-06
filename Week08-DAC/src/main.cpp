
#include <Arduino.h>

#define DAC_PIN 25
#define PWM_PIN 14

#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

void setup()
{
    Serial.begin(115200);

    // Configure PWM
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PWM_PIN, PWM_CHANNEL);

    Serial.println("Simultaneous DAC and PWM Test");
}

void loop()
{
    // Increase brightness
    for (int value = 0; value <= 255; value++)
    {
        dacWrite(DAC_PIN, value);
        ledcWrite(PWM_CHANNEL, value);

        Serial.print("DAC/PWM Value: ");
        Serial.println(value);

        delay(20);
    }

    // Decrease brightness
    for (int value = 255; value >= 0; value--)
    {
        dacWrite(DAC_PIN, value);
        ledcWrite(PWM_CHANNEL, value);

        Serial.print("DAC/PWM Value: ");
        Serial.println(value);

        delay(20);
    }
}
