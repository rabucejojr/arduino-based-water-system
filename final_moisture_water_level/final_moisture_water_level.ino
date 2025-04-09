#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with I2C address 0x27 and 16x2 characters
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Relay is Active HIGH
#define RELAY_ON HIGH
#define RELAY_OFF LOW

// Pin assignments
const int waterLevelPin = A1;    // Water level sensor analog pin
const int redLED = 4;            // Red LED relay pin
const int pump = 3;              // Water pump relay pin
const int horn = 2;              // Horn relay pin
const int greenLED = 5;          // Green LED relay pin

// Threshold values
const int waterLevelThresholdHigh = 3;  // Minimum water level in cm
const int waterLevelThresholdLow = 1;  // Minimum water level in cm

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Configure pin modes
  pinMode(redLED, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(horn, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Default state: all devices OFF, green LED ON (normal condition)
  digitalWrite(redLED, RELAY_OFF);
  digitalWrite(pump, RELAY_OFF);
  digitalWrite(horn, RELAY_OFF);
  digitalWrite(greenLED, RELAY_ON);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
}
bool hornActivated = false;  // track if horn was already activated

void loop() {
  // Read and convert water level
  int waterLevelValue = analogRead(waterLevelPin);
  int waterLevel = map(waterLevelValue, 0, 1023, 0, 10); // in cm

  // LCD Display
  lcd.setCursor(0, 1);
  lcd.print("Water: ");
  lcd.print(waterLevel);
  lcd.print("cm   ");

  // Serial Monitor
  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println("cm");

  // Logic starts here
  if (waterLevel <= 1) {
    // Dangerously low
    setRelays(false, true, true, false); // Pump + Red LED ON
    hornActivated = false; // reset horn trigger
  } 
  else if (waterLevel < 3) {
    // Still low but recovering
    setRelays(false, true, false, false); // Keep Pump ON
    hornActivated = false; // reset horn trigger
  } 
  else if (waterLevel == 3 && !hornActivated) {
    // Reached 3 cm — Turn OFF pump, activate alarm
    setRelays(false, false, true, true); // Red LED + Horn ON
    delay(10000);                        // Horn delay 10s
    digitalWrite(horn, RELAY_OFF);      // Turn OFF horn after delay
    hornActivated = true;               // Prevent repeated horn triggers
  } 
  else {
    // Normal (above 3 cm)
    setRelays(false, false, false, false); // All Relay OFF
    hornActivated = false;               // Reset horn
  }
}

// Relay control helper
void setRelays(bool green, bool pumpOn, bool red, bool hornOn) {
  digitalWrite(greenLED, green ? RELAY_ON : RELAY_OFF);
  digitalWrite(pump, pumpOn ? RELAY_ON : RELAY_OFF);
  digitalWrite(redLED, red ? RELAY_ON : RELAY_OFF);
  digitalWrite(horn, hornOn ? RELAY_ON : RELAY_OFF);
}
