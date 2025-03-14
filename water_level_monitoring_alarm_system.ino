// Pin Definitions
#define MOISTURE_SENSOR_PIN A0     // Soil moisture sensor (analog)
#define WATER_LEVEL_SENSOR_PIN A1  // Water level sensor (analog)
#define WATER_LEVEL_PWR 8          // Power pin for water level sensor
#define PUMP_RELAY_PIN 7           // Pump relay control
#define LED_PIN 6                  // LED indicator
#define HORN_PIN 5                 // Buzzer/Horn

// Thresholds
#define MOISTURE_THRESHOLD 20    // Below this, soil is too dry
#define WATER_LEVEL_THRESHOLD 3  // Stop pump at this water level (in cm)

void setup() {
  Serial.begin(9600);

  // Moisture Sensor
  pinMode(MOISTURE_SENSOR_PIN, INPUT);

  // Water Level Sensor
  pinMode(WATER_LEVEL_PWR, OUTPUT);
  pinMode(WATER_LEVEL_SENSOR_PIN, INPUT);
  digitalWrite(WATER_LEVEL_PWR, LOW);  // Turn off sensor initially

  // Pump, LED, and Horn
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(HORN_PIN, OUTPUT);

  digitalWrite(PUMP_RELAY_PIN, LOW);  // Pump OFF initially
  digitalWrite(LED_PIN, LOW);
  digitalWrite(HORN_PIN, LOW);
}

void loop() {
  int moisture = readMoisture();
  int waterLevel = readWaterLevel();

  Serial.print("Moisture Level: ");
  Serial.print(moisture);
  Serial.print(" | Water Level: ");
  Serial.print(waterLevel);
  Serial.println(" cm");

  // Pump control logic
  if (moisture <= MOISTURE_THRESHOLD && waterLevel < WATER_LEVEL_THRESHOLD) {
    activatePump(true);
  } else {
    activatePump(false);
  }

  // If water reaches threshold, trigger LED & Horn
  if (waterLevel >= WATER_LEVEL_THRESHOLD) {
    alertUser(true);
  } else {
    alertUser(false);
  }

  delay(1000);  // Delay to prevent excessive readings
}

// Function to read soil moisture
int readMoisture() {
  int sensorValue = analogRead(MOISTURE_SENSOR_PIN);
  int moistureLevel = map(sensorValue, 0, 1023, 255, 0);  // Map to 8-bit value
  return moistureLevel;
}

// Function to read water level sensor
int readWaterLevel() {
  digitalWrite(WATER_LEVEL_PWR, HIGH);  // Power on sensor
  delay(10);                            // Allow sensor to stabilize
  int level = analogRead(WATER_LEVEL_SENSOR_PIN);
  digitalWrite(WATER_LEVEL_PWR, LOW);  // Power off sensor

  // Convert sensor reading to cm (adjust based on calibration)
  int level_cm = map(level, 0, 1023, 0, 10);
  return level_cm;
}

// Function to activate pump
void activatePump(bool state) {
  digitalWrite(PUMP_RELAY_PIN, state);
}

// Function to trigger LED and Horn
void alertUser(bool state) {
  digitalWrite(LED_PIN, state);
  digitalWrite(HORN_PIN, state);
}
