# Automated Irrigation System with LCD Display

## Overview
This project is an automated irrigation system that monitors soil moisture and water levels and controls a water pump accordingly. A 16x2 LCD with I2C is used to display real-time sensor readings and system status. The system also includes alert mechanisms using LEDs and a horn.

## Features
- **Soil Moisture Monitoring**: Reads soil moisture levels and determines if watering is needed.
- **Water Level Detection**: Monitors the water level.
- **Automated Pump Control**: Activates and deactivates the pump based on soil moisture conditions.
- **Visual and Audio Alerts**: Uses LEDs and a horn for alerts.
- **LCD Display**: Shows real-time moisture and water levels, along with system status.

## Components
- Arduino Board (Uno/Nano)
- Soil Moisture Sensor
- Water Level Sensor
- 4-Channel Relay Module (to control water pump)
- 16x2 LCD Display with I2C Module
- Horn
- LEDs (Red and Green)
- Connecting Wires
- Power Supply

## Wiring Connections
### 1. **16x2 LCD Display with I2C**
| LCD Pin  | Arduino Pin |
|----------|------------|
| VCC      | 5V         |
| GND      | GND        |
| SDA      | SDA         |
| SCL      | SCL         |

### 2. **Soil Moisture Sensor**
| Sensor Pin | Arduino Pin |
|------------|------------|
| VCC        | 5V         |
| GND        | GND        |
| A0         | A0         |

### 3. **Water Level Sensor**
| Sensor Pin | Arduino Pin |
|------------|------------|
| VCC        | 5V         |
| GND        | GND        |
| A1         | A1         |

### 4. **4-Channel Relay Module**
| Relay Pin | Arduino Pin |
|-----------|------------|
| VCC       | 5V         |
| GND       | GND        |
| IN1       | D2 (Horn)  |
| IN2       | D3 (Pump)  |
| IN3       | D4 (Red LED) |
| IN4       | D5 (Green LED) |

## LCD Test Code
To ensure that the LCD display is properly wired and working, upload the following test code to your Arduino:
```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("LCD Test Successful!");
    lcd.setCursor(0, 1);
    lcd.print("I2C Working!");
}

void loop() {
}
```
Upload this code and check if the message "LCD Test Successful!" appears on the display.

## How It Works
1. The system continuously reads soil moisture and water level sensor values.
2. If the soil is dry the pump activate, then stops when water level reaches 3cm.
3. The LCD displays the soil moisture, and water level.
4. If the water level is too high or soil is too dry or vice versa, an alert is triggered using LEDs and a horn.

## Notes
- Ensure that the I2C address of the LCD module is correct. If the LCD doesn't display anything, use an I2C scanner sketch to find the correct address.
- Adjust the sensor threshold values based on environmental conditions.

## Future Improvements
- Add a real-time clock (RTC) to schedule watering cycles.
- Implement a wireless monitoring system using an ESP8266 or Bluetooth module.
- Store sensor data and pump activity logs using an SD card module.

Happy Building! 🚀

