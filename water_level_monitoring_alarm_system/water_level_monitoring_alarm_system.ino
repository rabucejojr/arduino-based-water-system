#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD I2C address (0x27 or 0x3F are common addresses)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16x2 LCD
// Pin Definitions
#define MOISTURE_SENSOR_PIN A0     // Soil moisture sensor (analog input)
#define WATER_LEVEL_SENSOR_PIN A1  // Water level sensor (analog input)
#define HORN_PIN 2                 // Buzzer/Horn output
#define PUMP_RELAY_PIN 3           // Relay control for the water pump
#define RED_LED 4                  // Red LED indicator for water level alert
#define GREEN_LED 5                // Green LED indicator for normal operation

// Threshold Values
#define DRY_THRESHOLD 30         // Soil moisture below this value means dry soil
#define WET_THRESHOLD 60         // Soil moisture above this value means wet soil (turn off pump)
#define WATER_LEVEL_THRESHOLD 3  // Pump stops when water level reaches this value (in cm)
#define MOISTURE_HYSTERESIS 5    // Prevents rapid pump switching (hysteresis)

// Variable to track pump state to prevent frequent ON/OFF switching
bool pumpActive = false;

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud rate

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Initializing");
  delay(2000);
  lcd.clear();

  // Set sensor pins as input
  pinMode(MOISTURE_SENSOR_PIN, INPUT);
  pinMode(WATER_LEVEL_SENSOR_PIN, INPUT);

  // Set actuator pins as output
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(HORN_PIN, OUTPUT);

  // Initialize all outputs to LOW (OFF state)
  digitalWrite(PUMP_RELAY_PIN, LOW);  // Ensure pump is OFF initially
  digitalWrite(RED_LED, LOW);         // LED OFF initially
  digitalWrite(GREEN_LED, LOW);       // LED OFF initially
  digitalWrite(HORN_PIN, LOW);        // Buzzer OFF initially
}

void loop() {
  // Read sensor values
  int moisture = readMoisture();      // Get soil moisture level
  int waterLevel = readWaterLevel();  // Get water level

  // Display readings on Serial Monitor for debugging
  Serial.print("Moisture Level: ");
  Serial.print(moisture);
  Serial.println("%");  // Display percentage

  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println(" cm");  // Display in centimeters

  // Display readings on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Moisture: ");
  lcd.print(moisture);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Water: ");
  lcd.print(waterLevel);
  lcd.print(" cm");

  // Check pump control based on conditions
  checkPumpControl(moisture, waterLevel);

  delay(1000);  // Delay to prevent excessive readings
}

// Function to read soil moisture level
int readMoisture() {
  int sensorValue = analogRead(MOISTURE_SENSOR_PIN);  // Read raw analog value
  // Map raw sensor value (0-1023) to percentage (0-100%)
  int moistureLevel = map(sensorValue, 1023, 0, 0, 100);
  return moistureLevel;
}

// Function to read water level sensor and convert to cm
int readWaterLevel() {
  int level = analogRead(WATER_LEVEL_SENSOR_PIN);  // Read raw analog value
  // Convert sensor reading (0-1023) to water level in cm (adjust based on calibration)
  int level_cm = map(level, 0, 1023, 0, 10);
  return level_cm;
}

// Function to activate or deactivate pump
void activatePump(bool state) {
  digitalWrite(HORN_PIN, state ? HIGH : LOW);        // Activate buzzer when pump turns ON
  digitalWrite(PUMP_RELAY_PIN, state ? HIGH : LOW);  // Adjust HIGH/LOW based on relay type

  lcd.setCursor(0, 1);
  lcd.print(state ? "Pump: ON " : "Pump: OFF");
}

// Function to check and control the pump based on moisture and water level
void checkPumpControl(int moisture, int waterLevel) {
  // Conditions to check for alerts
  bool soilDryAlert = (moisture <= DRY_THRESHOLD);               // True if soil is too dry
  bool waterLevelAlert = (waterLevel >= WATER_LEVEL_THRESHOLD);  // True if water reaches 3cm

  // If soil is too dry and water level is below threshold, activate pump
  if (soilDryAlert && !waterLevelAlert) {
    if (!pumpActive) {
      activatePump(true);
      pumpActive = true;
      Serial.println("Pump Activated: Soil is too dry!");
    }
  }
  // If soil moisture is above WET_THRESHOLD, deactivate pump
  else if (moisture > WET_THRESHOLD) {
    if (pumpActive) {
      activatePump(false);
      pumpActive = false;
      Serial.println("Pump Deactivated: Soil is sufficiently moist.");
    }
  }

  // Trigger alerts based on conditions
  alertUser(soilDryAlert, waterLevelAlert);
}

// Function to alert user via LED and buzzer based on soil and water conditions
void alertUser(bool soilDryAlert, bool waterLevelAlert) {
  if (soilDryAlert) {
    // Soil is too dry, turn ON Red LED and buzzer
    digitalWrite(RED_LED, HIGH);   // Turn ON Red LED
    digitalWrite(GREEN_LED, LOW);  // Turn OFF Green LED
    digitalWrite(HORN_PIN, HIGH);  // Activate Buzzer

    lcd.setCursor(0, 1);
    lcd.print("Alert: Dry Soil  ");

    delay(2000);  // Alert duration (2 seconds)

    digitalWrite(HORN_PIN, LOW);  // Turn OFF Buzzer after delay
  } else if (waterLevelAlert) {
    // Water level reached 3cm, turn ON Green LED and buzzer
    digitalWrite(GREEN_LED, HIGH);  // Turn ON Green LED
    digitalWrite(RED_LED, LOW);     // Turn OFF Red LED
    digitalWrite(HORN_PIN, HIGH);   // Activate Buzzer
    
    lcd.setCursor(0, 1);
    lcd.print("Alert: Water High");
    
    delay(2000);  // Alert duration (2 seconds)

    digitalWrite(HORN_PIN, LOW);  // Turn OFF Buzzer after delay
  } else {
    // Normal condition, turn OFF all alerts
    digitalWrite(RED_LED, LOW);    // Turn OFF Red LED
    digitalWrite(GREEN_LED, LOW);  // Turn OFF Green LED
    digitalWrite(HORN_PIN, LOW);   // Ensure Buzzer is OFF
  }
}
