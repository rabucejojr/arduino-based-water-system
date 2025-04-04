#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with I2C address 0x27 and 16x2 characters
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Relay is Active HIGH
#define RELAY_ON HIGH
#define RELAY_OFF LOW

// Pin assignments
const int soilMoisturePin = A0;  // Soil moisture sensor analog pin
const int waterLevelPin = A1;    // Water level sensor analog pin
const int redLED = 4;            // Red LED relay pin
const int pump = 3;              // Water pump relay pin
const int horn = 2;              // Horn relay pin
const int greenLED = 5;          // Green LED relay pin

// Threshold values
const int moistureThreshold = 58;   // Minimum soil moisture in %
const int waterLevelThreshold = 3;  // Minimum water level in cm

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

void loop() {
  // Read analog sensor values
  int moistureValue = analogRead(soilMoisturePin);
  int waterLevelValue = analogRead(waterLevelPin);

  // Convert raw values to percentage or centimeters
  int soilMoisture = map(moistureValue, 0, 1023, 0, 100);  // e.g., 0–100%
  int waterLevel = map(waterLevelValue, 0, 1023, 0, 10);   // e.g., 0–10 cm

  // Display readings on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moist: ");
  lcd.print(soilMoisture);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Water: ");
  lcd.print(waterLevel);
  lcd.print("cm");

  // Print readings to Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.println("%");
  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println("cm");

// Condition: Either soil is too dry OR water level is too low
if (soilMoisture < moistureThreshold || waterLevel < waterLevelThreshold) {
  // Turn ON red LED to indicate problem
  digitalWrite(redLED, RELAY_ON);
  
  // Horn alert pattern (2 short beeps)
  for (int i = 0; i < 2; i++) {
    digitalWrite(horn, RELAY_ON);
    delay(300);
    digitalWrite(horn, RELAY_OFF);
    delay(200);
  }

  // Only run pump when BOTH:
  // - Soil is dry (below threshold)
  // - Water level is still below 3cm
  if (soilMoisture < moistureThreshold && waterLevel < waterLevelThreshold) {
    digitalWrite(pump, RELAY_ON);  // Pump ON
  } else {
    digitalWrite(pump, RELAY_OFF); // Stop pump if water is sufficient
  }

  // Green LED OFF during abnormal condition
  digitalWrite(greenLED, RELAY_OFF);

} else {
  // All normal: turn OFF alerts, turn ON green LED
  digitalWrite(redLED, RELAY_OFF);
  digitalWrite(pump, RELAY_OFF);
  digitalWrite(horn, RELAY_OFF);
  digitalWrite(greenLED, RELAY_ON);
}

}