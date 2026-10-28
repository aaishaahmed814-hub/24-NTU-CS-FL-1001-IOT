# IoT Homework 2 — ESP32 PWM Control

## Task 1: Dual-LED PWM Control

This task uses an ESP32 to control two LEDs independently using Pulse Width Modulation (PWM). The brightness of both LEDs gradually increases and decreases.

**Features:**
- Two independently controlled LEDs.
- Different PWM frequencies: 1000 Hz and 5000 Hz.
- 8-bit PWM resolution (0–255).
- Smooth LED fading effect.

## Task 2: Dual-LED PWM with OLED Monitoring

This task extends Task 1 by adding an SSD1306 I²C OLED display to monitor the LEDs' PWM values.

**Features:**
- Displays the duty-cycle percentage of both LEDs.
- Displays the configured PWM frequency of each LED.
- Updates duty-cycle values as the LEDs fade.
- Uses I²C communication between the ESP32 and OLED.

## Hardware Components

- ESP32 DevKit
- Two LEDs
- Two 220 Ω or 330 Ω current-limiting resistors
- SSD1306 I²C OLED display (Task 2)
- Breadboard and jumper wires
- USB data cable

## Pin Configuration

| Component | ESP32 Pin |
|---|---|
| LED 1 PWM | GPIO 2 |
| LED 2 PWM | GPIO 18 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| OLED VCC | 3V3 (if supported by module) |
| Common Ground | GND |

## Software and Tools

- Visual Studio Code
- PlatformIO
- Wokwi Simulator
- Arduino Framework
- Adafruit SSD1306 Library
- Adafruit GFX Library

## Testing

The project should be tested in the Wokwi simulator and on physical ESP32 hardware. Verify LED fading, OLED initialization, and duty-cycle updates. Add actual screenshots and hardware photographs after testing.

## Conclusion

This project demonstrates ESP32 PWM-based LED brightness control at different frequencies. Task 2 enhances the system by displaying the current duty-cycle percentages and configured PWM frequencies on an OLED screen.