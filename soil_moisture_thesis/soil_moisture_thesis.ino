// Define sensor and actuator pins
#define SOIL_MOISTURE A0     // Soil moisture sensor connected to analog pin A0
#define WATER_LEVEL A1    // Water level sensor connected to analog pin A1
#define RELAY_PUMP 2       // Water pump relay connected to digital pin 2
#define RELAY_HORN 3           // RELAY_HORN (Horn) connected to digital pin 3
#define RELAY_GREEN 4        // Green LED (indicates dry soil) connected to digital pin 4
#define RELAY_RED 5         // Red LED (indicates water full) connected to digital pin 5

// Set threshold values for moisture and water level
int dryThreshold = 20;    // Adjust this value based on soil moisture sensor readings
int waterThreshold = 300;  // Adjust this value based on water level sensor readings

void setup() {
    // Set pin modes
    pinMode(SOIL_MOISTURE, INPUT);
    pinMode(WATER_LEVEL, INPUT);
    pinMode(RELAY_PUMP, OUTPUT);
    pinMode(RELAY_HORN, OUTPUT);
    pinMode(RELAY_GREEN, OUTPUT);
    pinMode(RELAY_RED, OUTPUT);
    
    // Ensure all actuators are off initially
    digitalWrite(RELAY_PUMP, LOW);  
    digitalWrite(RELAY_HORN, LOW);
    digitalWrite(RELAY_GREEN, LOW);
    digitalWrite(RELAY_RED, LOW);

    // Initialize serial communication for debugging
    Serial.begin(9600);
}

void loop() {
    // Read values from sensors
    int soilMoisture = analogRead(SOIL_MOISTURE); // Read soil moisture level
    int waterLevel = analogRead(WATER_LEVEL);  // Read water level

    // Print sensor values to the serial monitor for debugging
    Serial.print("Soil Moisture: ");
    Serial.print(soilMoisture);
    Serial.print(" | Water Level: ");
    Serial.println(waterLevel);

    // Condition to check if soil is dry
    if (soilMoisture > dryThreshold) {
        // Dry soil detected, turn on the water pump
        digitalWrite(RELAY_PUMP, HIGH);
        
        // Activate green LED and RELAY_HORN for 30 seconds
        digitalWrite(RELAY_GREEN, HIGH);
        digitalWrite(RELAY_HORN, HIGH);
        delay(30000);  // Wait for 30 seconds
        digitalWrite(RELAY_HORN, LOW);
        digitalWrite(RELAY_GREEN, LOW);
    }

    // Condition to check if water level reaches 3cm
    if (waterLevel > waterThreshold) {
        // Water level reached the threshold, turn off the water pump
        digitalWrite(RELAY_PUMP, LOW);
        
        // Activate red LED and RELAY_HORN for 30 seconds
        digitalWrite(RELAY_RED, HIGH);
        digitalWrite(RELAY_HORN, HIGH);
        delay(30000);  // Wait for 30 seconds
        digitalWrite(RELAY_HORN, LOW);
        digitalWrite(RELAY_RED, LOW);
    }

    // Small delay to prevent excessive sensor readings
    delay(500);  // Wait for 1 second before reading again
}
