#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// LED pins and PWM channels
constexpr uint8_t LED1_PIN = 2;
constexpr uint8_t LED2_PIN = 18;

constexpr uint8_t LED1_CHANNEL = 0;
constexpr uint8_t LED2_CHANNEL = 1;

// Different PWM frequencies
constexpr uint32_t LED1_FREQUENCY = 5000;
constexpr uint32_t LED2_FREQUENCY = 1000;

constexpr uint8_t PWM_RESOLUTION = 8;
constexpr uint16_t PWM_MAX_DUTY = 255;

// OLED settings
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr uint8_t OLED_ADDRESS = 0x3C;

Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

// Duty-cycle values
uint16_t led1Duty = 0;
uint16_t led2Duty = 0;

// Fade directions
int8_t led1Direction = 1;
int8_t led2Direction = 1;

// Timing variables
unsigned long previousLED1Time = 0;
unsigned long previousLED2Time = 0;
unsigned long previousOLEDTime = 0;

// Fading and OLED update intervals
constexpr unsigned long LED1_INTERVAL = 10;
constexpr unsigned long LED2_INTERVAL = 15;
constexpr unsigned long OLED_INTERVAL = 250;

// Convert duty cycle to percentage
uint8_t getPercentage(uint16_t duty)
{
    return (duty * 100 + PWM_MAX_DUTY / 2)
           / PWM_MAX_DUTY;
}

// Initialize hardware
void setup()
{
    Serial.begin(115200);

    // Initialize LED1 PWM
    ledcSetup(
        LED1_CHANNEL,
        LED1_FREQUENCY,
        PWM_RESOLUTION
    );

    ledcAttachPin(LED1_PIN, LED1_CHANNEL);

    // Initialize LED2 PWM
    ledcSetup(
        LED2_CHANNEL,
        LED2_FREQUENCY,
        PWM_RESOLUTION
    );

    ledcAttachPin(LED2_PIN, LED2_CHANNEL);

    // Start both LEDs turned off
    ledcWrite(LED1_CHANNEL, 0);
    ledcWrite(LED2_CHANNEL, 0);

    // Initialize I2C OLED
    Wire.begin(21, 22);

    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS))
    {
        Serial.println("OLED initialization failed!");

        while (true)
        {
            delay(1000);
        }
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Dual LED PWM");
    display.println("OLED Monitoring");
    display.display();

    delay(1000);

    Serial.println("Dual LED PWM Started");
}

// Main program
void loop()
{
    unsigned long currentTime = millis();

    // Fade LED1
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

    // Fade LED2 independently
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

    // Update OLED
    if (currentTime - previousOLEDTime >= OLED_INTERVAL)
    {
        previousOLEDTime = currentTime;

        uint8_t led1Percent = getPercentage(led1Duty);
        uint8_t led2Percent = getPercentage(led2Duty);

        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);

        display.setCursor(0, 0);
        display.println("DUAL LED PWM");

        display.setCursor(0, 16);
        display.print("LED1: ");
        display.print(led1Percent);
        display.println("%");

        display.setCursor(0, 26);
        display.print("Freq: ");
        display.print(LED1_FREQUENCY);
        display.println(" Hz");

        display.setCursor(0, 38);
        display.print("LED2: ");
        display.print(led2Percent);
        display.println("%");

        display.setCursor(0, 48);
        display.print("Freq: ");
        display.print(LED2_FREQUENCY);
        display.println(" Hz");

        display.display();

        Serial.printf(
            "LED1: %u%% (%lu Hz) | LED2: %u%% (%lu Hz)\n",
            led1Percent,
            (unsigned long)LED1_FREQUENCY,
            led2Percent,
            (unsigned long)LED2_FREQUENCY
        );
    }
}