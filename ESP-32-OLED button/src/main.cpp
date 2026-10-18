
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C

// ESP32 pins
const int OLED_SDA = 21;
const int OLED_SCL = 22;
const int BUTTON_PIN = 18;

// Debounce configuration
const unsigned long DEBOUNCE_MS = 50;

// OLED object
Adafruit_SSD1306 display(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    &Wire,
    -1
);

// Button state variables
int lastRawState = HIGH;
int stableButtonState = HIGH;

unsigned long lastStateChangeTime = 0;

// Accepted press counter
unsigned long pressCount = 0;

// Draw the current status on the OLED
void updateDisplay() {
    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("ESP32 BUTTON STATUS");
    display.println("--------------------");

    display.setCursor(0, 24);
    display.print("Button: ");

    if (stableButtonState == LOW) {
        display.println("PRESSED");
    } else {
        display.println("RELEASED");
    }

    display.setCursor(0, 44);
    display.print("Press count: ");
    display.println(pressCount);

    display.display();
}

void setup() {
    Serial.begin(115200);

    // Initialize I2C using the selected ESP32 pins
    Wire.begin(OLED_SDA, OLED_SCL);

    // Initialize the OLED
    if (!display.begin(
            SSD1306_SWITCHCAPVCC,
            OLED_ADDRESS)) {
        Serial.println("OLED initialization failed!");
        while (true) {
            // Stop if the OLED cannot initialize
        }
    }

    // The button connects GPIO 18 to GND when pressed
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Read the initial button state
    lastRawState = digitalRead(BUTTON_PIN);
    stableButtonState = lastRawState;
    lastStateChangeTime = millis();

    updateDisplay();

    Serial.println("ESP32 OLED Button Monitor Started");
    Serial.println("Press count: 0");
}

void loop() {
    // Read the button without blocking the program
    int currentRawState = digitalRead(BUTTON_PIN);

    // Detect a change in the raw electrical signal
    if (currentRawState != lastRawState) {
        lastRawState = currentRawState;
        lastStateChangeTime = millis();
    }

    // Accept the new state only after it remains stable
    // for at least DEBOUNCE_MS milliseconds
    if ((millis() - lastStateChangeTime >= DEBOUNCE_MS) &&
        (stableButtonState != lastRawState)) {

        stableButtonState = lastRawState;

        if (stableButtonState == LOW) {
            // Count only a confirmed press, not the release
            pressCount++;

            Serial.print("Accepted press count: ");
            Serial.println(pressCount);
        } else {
            Serial.println("Button released");
        }

        // Refresh the display after an accepted state change
        updateDisplay();
    }

    // No long delay(): the ESP32 remains responsive
}
