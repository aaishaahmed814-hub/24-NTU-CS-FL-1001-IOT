
#include <Arduino.h>

// LED GPIO pins
constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 18;

// PWM channel numbers
constexpr uint8_t LED1_CHANNEL = 0;
constexpr uint8_t LED2_CHANNEL = 8;

// Different PWM frequencies
constexpr uint32_t LED1_FREQUENCY = 5000;
constexpr uint32_t LED2_FREQUENCY = 1000;

// PWM resolution
constexpr uint8_t PWM_RESOLUTION = 8;
constexpr uint16_t PWM_MAX_DUTY = 255;

// Duty values available for Task 2 OLED
uint16_t led1Duty = 0;
uint16_t led2Duty = 0;

// Fading directions
int8_t led1Direction = 1;
int8_t led2Direction = 1;

// Independent timing
unsigned long previousLED1Time = 0;
unsigned long previousLED2Time = 0;
unsigned long previousPrintTime = 0;

constexpr unsigned long LED1_INTERVAL = 10;
constexpr unsigned long LED2_INTERVAL = 5;

void setup()
{
    Serial.begin(115200);

    // Configure LED 1: 5000 Hz
    ledcSetup(LED1_CHANNEL, LED1_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(LED1_PIN, LED1_CHANNEL);

    // Configure LED 2: 1000 Hz
    ledcSetup(LED2_CHANNEL, LED2_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(LED2_PIN, LED2_CHANNEL);

    // Start at minimum brightness
    ledcWrite(LED1_CHANNEL, 0);
    ledcWrite(LED2_CHANNEL, 0);

    Serial.println("Dual LED PWM Fade Started");
    Serial.println("LED1: 5000 Hz | LED2: 1000 Hz");
}

void loop()
{
    unsigned long currentTime = millis();

    // Fade LED 1
    if (currentTime - previousLED1Time >= LED1_INTERVAL)
    {
        previousLED1Time = currentTime;

        led1Duty += led1Direction;

        if (led1Duty >= PWM_MAX_DUTY)
        {
            led1Duty = PWM_MAX_DUTY;
            led1Direction = -1;
        }
        else if (led1Duty == 0)
        {
            led1Direction = 1;
        }

        ledcWrite(LED1_CHANNEL, led1Duty);
    }

    // Fade LED 2 independently
    if (currentTime - previousLED2Time >= LED2_INTERVAL)
    {
        previousLED2Time = currentTime;

        led2Duty += led2Direction;

        if (led2Duty >= PWM_MAX_DUTY)
        {
            led2Duty = PWM_MAX_DUTY;
            led2Direction = -1;
        }
        else if (led2Duty == 0)
        {
            led2Direction = 1;
        }

        ledcWrite(LED2_CHANNEL, led2Duty);
    }

    // Serial output for testing and Task 2 preparation
    if (currentTime - previousPrintTime >= 500)
    {
        previousPrintTime = currentTime;

        float led1Percent =
            (led1Duty * 100.0f) / PWM_MAX_DUTY;

        float led2Percent =
            (led2Duty * 100.0f) / PWM_MAX_DUTY;

        Serial.printf(
            "LED1: %.1f%% | 5000 Hz || LED2: %.1f%% | 1000 Hz\n",
            led1Percent,
            led2Percent
        );
    }
}
