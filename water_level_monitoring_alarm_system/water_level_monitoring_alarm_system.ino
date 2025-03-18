// Pin Definitions
#define MOISTURE_SENSOR_PIN A0     // Soil moisture sensor (analog input)
#define WATER_LEVEL_SENSOR_PIN A1  // Water level sensor (analog input)
#define HORN_PIN 2                 // Buzzer/Horn output
#define PUMP_RELAY_PIN 3           // Relay control for the water pump
#define RED_LED 4                  // Red LED indicator for water level alert
#define GREEN_LED 5                // Green LED indicator for normal operation

// Threshold Values
#define MOISTURE_THRESHOLD 20      // Soil moisture level below this value means dry soil
#define WATER_LEVEL_THRESHOLD 3    // Pump stops when water level reaches this value (in cm)
#define MOISTURE_HYSTERESIS 5      // Prevents rapid pump switching (hysteresis)

// Variable to track pump state to prevent frequent ON/OFF switching
bool pumpActive = false;

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud rate

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
  int moisture = readMoisture();      // Get moisture level
  int waterLevel = readWaterLevel();  // Get water level

  // Display readings on Serial Monitor for debugging
  Serial.print("Moisture Level: ");
  Serial.print(moisture);
  Serial.println("%"); // Display percentage

  Serial.print("Water Level: ");
  Serial.print(waterLevel);
  Serial.println(" cm"); // Display in centimeters

  // Pump control logic with hysteresis
  if (moisture <= MOISTURE_THRESHOLD && waterLevel < WATER_LEVEL_THRESHOLD) {
    if (!pumpActive) {  // Only activate pump if it was previously OFF
      activatePump(true);
      pumpActive = true;
    }
  } else if (moisture > MOISTURE_THRESHOLD + MOISTURE_HYSTERESIS) {
    if (pumpActive) {  // Only deactivate pump if it was previously ON
      activatePump(false);
      pumpActive = false;
    }
  }

  // If water level reaches or exceeds threshold, trigger LED & Buzzer
  if (waterLevel >= WATER_LEVEL_THRESHOLD) {
    alertUser(true);
  } else {
    alertUser(false);
  }

  delay(1000);  // Delay to prevent excessive readings
}

// Function to read soil moisture level
int readMoisture() {
  int sensorValue = analogRead(MOISTURE_SENSOR_PIN); // Read raw analog value
  // Map raw sensor value (0-1023) to percentage (0-100%)
  int moistureLevel = map(sensorValue, 1023, 0, 0, 100);  
  return moistureLevel;
}

// Function to read water level sensor and convert to cm
int readWaterLevel() {
  int level = analogRead(WATER_LEVEL_SENSOR_PIN); // Read raw analog value
  // Convert sensor reading (0-1023) to water level in cm (adjust based on calibration)
  int level_cm = map(level, 0, 1023, 0, 10);
  return level_cm;
}

// Function to activate or deactivate pump
void activatePump(bool state) {
  digitalWrite(PUMP_RELAY_PIN, state ? HIGH : LOW); // Adjust HIGH/LOW based on relay type
}

// Function to trigger LED and Buzzer for water level alert
void alertUser(bool state) {
  digitalWrite(RED_LED, state);   // Turn on Red LED for alert
  digitalWrite(GREEN_LED, !state); // Green LED is opposite of alert state
  digitalWrite(HORN_PIN, state);  // Activate Buzzer/Horn when in alert mode
}
