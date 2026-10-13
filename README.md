# ESP32 Motor RPM Meter

A DIY real-time motor RPM meter built using an ESP32, Hall Effect sensor, and SPI OLED display.

## Features
- Real-time motor RPM measurement
- Live RPM display on SPI OLED
- Maximum RPM tracking
- Motor stop detection
- Magnetic sensing using a Hall Effect sensor

## Components Required
- ESP32 development board
- SPI OLED display (SSD1306 128×64)
- Hall Effect sensor module
- Small magnet
- Jumper wires
- Motor for testing

## Wiring

### Hall Effect Sensor to ESP32
| Sensor Pin | ESP32 Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| OUT / DO | GPIO 27 |

### SPI OLED to ESP32
| OLED Pin | ESP32 Pin |
|---|---|
| GND | GND |
| VCC | 5V |
| SCL / CLK | GPIO 18 |
| SDA / MOSI | GPIO 23 |
| RES / RST | GPIO 4 |
| DC | GPIO 16 |
| CS | GPIO 5 |

## Arduino IDE Setup
1. Install the ESP32 board package in Arduino IDE.
2. Install the Adafruit GFX Library.
3. Install the Adafruit SSD1306 Library.
4. Open `ESP32-Motor-RPM-Meter.ino`.
5. Select your ESP32 board and COM port.
6. Upload the code.

## How It Works
A magnet attached to the rotating shaft passes the Hall Effect sensor and generates pulses. The ESP32 calculates the motor speed in RPM and displays the result on the OLED.

**Note:** The code is configured for one magnet per revolution. Change `MAGNETS_PER_REV` if using multiple magnets.

## License
MIT License
