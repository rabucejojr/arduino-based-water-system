#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup (Assuming 0x27 address; change to 0x3F if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Relay is Active High
#define RELAY_ON LOW
#define RELAY_OFF HIGH

// Pin assignments
const int soilMoisturePin = A0;
const int waterLevelPin = A1;
const int redLED = 4;
const int pump = 3;
const int horn = 2;
const int greenLED = 5;

// Thresholds
const int moistureThreshold = 58;   // 58% optimal palay growth
const int waterLevelThreshold = 3;  // 3cm



void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(horn, OUTPUT);
  pinMode(greenLED, OUTPUT);

  digitalWrite(redLED, RELAY_OFF);
  digitalWrite(pump, RELAY_OFF);
  digitalWrite(horn, RELAY_OFF);
  digitalWrite(greenLED, RELAY_OFF);  // Default green LED ON for normal state

  lcd.init();
  lcd.backlight();
}

void loop() {
  Serial.begin(9600);
  // Read sensors
  int moistureValue = analogRead(soilMoisturePin);
  int waterLevelValue = analogRead(waterLevelPin);

  // Convert raw values (adjust scaling based on your sensor output)
  int soilMoisture = map(moistureValue, 0, 1023, 0, 100);  // Convert to percentage
  int waterLevel = map(waterLevelValue, 0, 1023, 0, 10);   // Convert to cm (adjust if needed)

  // Display readings
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moist: ");
  lcd.print(soilMoisture);
  lcd.println("%");
  lcd.setCursor(0, 1);
  lcd.print("Water: ");
  lcd.print(waterLevel);
  lcd.println("cm");

  Serial.print("Moist: ");
  Serial.print(soilMoisture);
  Serial.println("%");
  Serial.print("Water: ");
  Serial.print(waterLevel);
  Serial.println("cm");

  // Check conditions
if ( waterLevel < waterLevelThreshold || soilMoisture < moistureThreshold ) {
    digitalWrite(redLED, RELAY_ON);
    digitalWrite(horn, RELAY_ON);
    digitalWrite(pump, RELAY_ON);

    delay (2000);
} else {
    digitalWrite(redLED, RELAY_OFF);
    digitalWrite(horn, RELAY_OFF);
    digitalWrite(pump, RELAY_OFF);
    digitalWrite(greenLED, RELAY_ON);
}


  delay(5000);  // Wait before next reading
}
